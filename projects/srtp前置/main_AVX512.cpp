#include <iostream>
#include <immintrin.h>
#include <vector>
#include <cmath>
#include "SebspNM_AVX512.hpp"

int main() {
    int TxAntNum = 2;
    int RxAntNum = 2;
    int Nt2 = 2 * TxAntNum;
    int Nr2 = 2 * RxAntNum;
    int slen = 16;
    int nm = 16;  // 匹配 AVX512 softmax 上限
    int iterNum = 3;
    float Nv = 0.1f;
    float factor = 0.2f;

    SebspNM detector(Nt2, Nr2, slen, nm, iterNum, Nv);

    // 初始化 H 和 Rx
    for (int i = 0; i < Nr2; ++i) {
        detector.Rx[i] = 1.0f + i;
        for (int j = 0; j < Nt2; ++j) {
            detector.H[i][j] = 0.1f * (i + j + 1);
        }
    }

    // 初始化 16QAM 星座
    detector.sym = { -2.0f, -1.5f, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 1.5f,
                     2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 5.5f };

    // 初始化 consIdx 和 gamma（slen = 16）
    for (int i = 0; i < Nt2; ++i) {
        detector.consIdx[i].resize(slen);
        for (int j = 0; j < slen; ++j) {
            detector.consIdx[i][j] = j;
        }
        for (int j = 0; j < slen; ++j) {
            detector.gamma[i][j] = 0.0f;
        }
        detector.gamma[i][0] = 7.0f;
    }

    detector.run();
    detector.applyInfoCompensation(factor);

    auto llr = detector.getSymLLR();
    std::cout << "symLLR:" << std::endl;
    for (int j = 0; j < slen; ++j) {
        for (int i = 0; i < Nt2; ++i) {
            std::cout << llr[j][i] << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
