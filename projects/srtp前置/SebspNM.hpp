#ifndef SEBSPNM_HPP
#define SEBSPNM_HPP

#include <vector>
#include <immintrin.h>
#include <cmath>
#include <algorithm>

class SebspNM {
public:
    int Nt2, Nr2, slen, nm, iterNum;
    float Nv;
    std::vector<std::vector<float>> H;       // [Nr2][Nt2]
    std::vector<float> Rx;                   // [Nr2]
    std::vector<float> sym;                  // [slen]
    std::vector<std::vector<int>> consIdx;   // [Nt2][nm]
    std::vector<std::vector<float>> gamma;   // [Nt2][slen]

    SebspNM(int Nt2_, int Nr2_, int slen_, int nm_, int iterNum_, float Nv_)
        : Nt2(Nt2_), Nr2(Nr2_), slen(slen_), nm(nm_), iterNum(iterNum_), Nv(Nv_),
        H(Nr2_, std::vector<float>(Nt2_)),
        Rx(Nr2_),
        sym(slen_),
        consIdx(Nt2_, std::vector<int>(nm_)),
        gamma(Nt2_, std::vector<float>(slen_, 0.0f)) {
    }

    void softmax_avx2(const float* input, float* output) {
        __m256 in = _mm256_loadu_ps(input);
        float maxval = input[0];
        for (int i = 1; i < 8; i++) if (input[i] > maxval) maxval = input[i];
        __m256 maxvec = _mm256_set1_ps(maxval);
        __m256 offset = _mm256_sub_ps(in, maxvec);
        alignas(32) float temp[8];
        _mm256_store_ps(temp, offset);
        for (int i = 0; i < 8; i++) temp[i] = expf(temp[i]);
        float sum = 0.0f;
        for (int i = 0; i < 8; i++) sum += temp[i];
        for (int i = 0; i < 8; i++) output[i] = temp[i] / sum;
    }

    void run() {
        std::vector<std::vector<std::vector<float>>> alpha(Nt2, std::vector<std::vector<float>>(slen, std::vector<float>(Nr2, 0.0f)));
        std::vector<std::vector<std::vector<float>>> Px(Nt2, std::vector<std::vector<float>>(slen, std::vector<float>(Nr2, 0.0f)));
        std::vector<std::vector<std::vector<float>>> beta(Nt2, std::vector<std::vector<float>>(slen, std::vector<float>(Nr2, 0.0f)));
        std::vector<std::vector<float>> sGAI(Nt2, std::vector<float>(Nr2, 0.0f));
        std::vector<std::vector<float>> sGAI_var(Nt2, std::vector<float>(Nr2, 0.0f));

        for (int iter = 0; iter < iterNum; iter++) {
            for (int i = 0; i < Nt2; i++)
                for (int j = 0; j < slen; j++)
                    for (int k = 0; k < Nr2; k++)
                        alpha[i][j][k] = gamma[i][j] - beta[i][j][k];

            for (int i = 0; i < Nt2; i++) {
                for (int k = 0; k < Nr2; k++) {
                    float alpha_in[8]{}, Px_out[8]{};
                    for (int m = 0; m < nm; m++) {
                        int j = consIdx[i][m];
                        alpha_in[m] = alpha[i][j][k];
                    }
                    softmax_avx2(alpha_in, Px_out);
                    for (int m = 0; m < nm; m++) {
                        int j = consIdx[i][m];
                        Px[i][j][k] = Px_out[m];
                    }
                }
            }

            for (int i = 0; i < Nt2; i++) {
                for (int k = 0; k < Nr2; k++) {
                    float mu = 0.0f, var = 0.0f;
                    for (int m = 0; m < nm; m++) {
                        int j = consIdx[i][m];
                        float p = Px[i][j][k], s = sym[j];
                        mu += p * s;
                        var += p * s * s;
                    }
                    sGAI[i][k] = mu;
                    sGAI_var[i][k] = var - mu * mu;
                }
            }

            std::vector<std::vector<float>> ymean, yvar;
            compute_sGAI_mul_Ht_avx2(H, sGAI, ymean);
            compute_sGAI_var_mul_Ht2_avx2(H, sGAI_var, yvar);

            for (int i = 0; i < Nt2; i++) {
                for (int j = 0; j < slen; j++) {
                    for (int k = 0; k < Nr2; k++) {
                        float h = H[k][i];
                        float r = Rx[k];
                        float mu = ymean[i][k];
                        float v = yvar[i][k] + Nv;
                        float d1 = r - mu - h * sym[j];
                        float d0 = r - mu - h * sym[consIdx[i][0]];
                        beta[i][j][k] = (d0 * d0 - d1 * d1) / (2.0f * v);
                    }
                }
            }

            for (int i = 0; i < Nt2; i++)
                for (int j = 0; j < slen; j++) {
                    float sum = 0.0f;
                    for (int k = 0; k < Nr2; k++) sum += beta[i][j][k];
                    gamma[i][j] = sum;
                }
        }
    }

    void applyInfoCompensation(float factor) {
        for (int i = 0; i < Nt2; i++) {
            float base = gamma[i][consIdx[i][nm - 1]] - logf((1 - powf(factor, slen - nm)) / (1 - factor));
            for (int m = nm; m < slen; m++) {
                int j = m;
                gamma[i][consIdx[i][j]] = base + logf(factor) * (j - nm);
            }
        }
    }

    std::vector<std::vector<float>> getSymLLR() {
        std::vector<std::vector<float>> result(slen, std::vector<float>(Nt2));
        for (int i = 0; i < Nt2; i++)
            for (int j = 0; j < slen; j++)
                result[j][i] = gamma[i][j];
        return result;
    }

private:
    void compute_sGAI_mul_Ht_avx2(const std::vector<std::vector<float>>& H,
        const std::vector<std::vector<float>>& sGAI,
        std::vector<std::vector<float>>& ymean) {
        ymean.assign(Nt2, std::vector<float>(Nr2, 0.0f));
        for (int k = 0; k < Nr2; k++)
            for (int i = 0; i < Nt2; i++)
                ymean[i][k] = sGAI[i][k] * H[k][i];
    }

    void compute_sGAI_var_mul_Ht2_avx2(const std::vector<std::vector<float>>& H,
        const std::vector<std::vector<float>>& sGAI_var,
        std::vector<std::vector<float>>& yvar) {
        yvar.assign(Nt2, std::vector<float>(Nr2, 0.0f));
        for (int k = 0; k < Nr2; k++)
            for (int i = 0; i < Nt2; i++) {
                float h = H[k][i];
                yvar[i][k] = sGAI_var[i][k] * h * h;
            }
    }
};

#endif // SEBSPNM_HPP
