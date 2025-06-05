#ifndef SEBSPNM_AVX512_HPP
#define SEBSPNM_AVX512_HPP

#include <vector>
#include <immintrin.h>
#include <cmath>
#include <algorithm>

class SebspNM {
public:
    int Nt2, Nr2, slen, nm, iterNum;
    float Nv;
    std::vector<std::vector<float>> H;         // 通道矩阵
    std::vector<float> Rx;                     // 接收向量
    std::vector<float> sym;                    // 星座点
    std::vector<std::vector<int>> consIdx;     // 候选星座索引 [Nt2][slen]
    std::vector<std::vector<float>> gamma;     // Soft LLR [Nt2][slen]

    SebspNM(int Nt2_, int Nr2_, int slen_, int nm_, int iterNum_, float Nv_)
        : Nt2(Nt2_), Nr2(Nr2_), slen(slen_), nm(nm_), iterNum(iterNum_), Nv(Nv_),
        H(Nr2_, std::vector<float>(Nt2_)),
        Rx(Nr2_),
        sym(slen_),
        consIdx(Nt2_, std::vector<int>(nm_)),
        gamma(Nt2_, std::vector<float>(slen_, 0.0f)) {
    }

    // AVX512 Softmax（最大支持 16 点）
    inline void softmax_avx512(const float* input, float* output, int len = 16) {
        __m512 in = _mm512_loadu_ps(input);
        float maxval = input[0];
        for (int i = 1; i < len; i++) if (input[i] > maxval) maxval = input[i];
        __m512 maxvec = _mm512_set1_ps(maxval);
        __m512 offset = _mm512_sub_ps(in, maxvec);
        alignas(64) float temp[16];
        _mm512_store_ps(temp, offset);
        float sum = 0.0f;
        for (int i = 0; i < len; i++) temp[i] = expf(temp[i]), sum += temp[i];
        for (int i = 0; i < len; i++) output[i] = temp[i] / sum;
    }

    // 主流程
    void run();

    // LLR补偿
    void applyInfoCompensation(float factor);

    // 输出结果（与MATLAB接口一致）
    std::vector<std::vector<float>> getSymLLR();

private:
    // ymean = sGAI * Hᵀ
    void compute_sGAI_mul_Ht_avx512(const std::vector<std::vector<float>>& sGAI,
        std::vector<std::vector<float>>& ymean);

    // yvar = sGAI_var * (Hᵀ·Hᵀ)
    void compute_sGAI_var_mul_Ht2_avx512(const std::vector<std::vector<float>>& sGAI_var,
        std::vector<std::vector<float>>& yvar);
};

#endif
