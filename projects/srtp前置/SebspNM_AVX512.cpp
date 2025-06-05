// SebspNM_AVX512.cpp

#include "SebspNM_AVX512.hpp"

void SebspNM::compute_sGAI_mul_Ht_avx512(const std::vector<std::vector<float>>& sGAI,
    std::vector<std::vector<float>>& ymean) {
    ymean.assign(Nt2, std::vector<float>(Nr2, 0.0f));
    for (int k = 0; k < Nr2; ++k) {
        for (int i = 0; i < Nt2; ++i) {
            ymean[i][k] = sGAI[i][k] * H[k][i];
        }
    }
}

void SebspNM::compute_sGAI_var_mul_Ht2_avx512(const std::vector<std::vector<float>>& sGAI_var,
    std::vector<std::vector<float>>& yvar) {
    yvar.assign(Nt2, std::vector<float>(Nr2, 0.0f));
    for (int k = 0; k < Nr2; ++k) {
        for (int i = 0; i < Nt2; ++i) {
            float h = H[k][i];
            yvar[i][k] = sGAI_var[i][k] * h * h;
        }
    }
}

void SebspNM::run() {
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
                alignas(64) float alpha_in[16] = {};
                alignas(64) float Px_out[16] = {};
                for (int m = 0; m < nm; m++) {
                    int j = consIdx[i][m];
                    alpha_in[m] = alpha[i][j][k];
                }
                softmax_avx512(alpha_in, Px_out, nm);
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
        compute_sGAI_mul_Ht_avx512(sGAI, ymean);
        compute_sGAI_var_mul_Ht2_avx512(sGAI_var, yvar);

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

void SebspNM::applyInfoCompensation(float factor) {
    for (int i = 0; i < Nt2; i++) {
        float base = gamma[i][consIdx[i][nm - 1]] - logf((1 - powf(factor, slen - nm)) / (1 - factor));
        for (int m = nm; m < slen; m++) {
            int j = m;
            gamma[i][consIdx[i][j]] = base + logf(factor) * (j - nm);
        }
    }
}

std::vector<std::vector<float>> SebspNM::getSymLLR() {
    std::vector<std::vector<float>> result(slen, std::vector<float>(Nt2));
    for (int i = 0; i < Nt2; i++)
        for (int j = 0; j < slen; j++)
            result[j][i] = gamma[i][j];
    return result;
}
