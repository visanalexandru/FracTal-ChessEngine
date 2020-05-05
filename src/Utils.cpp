#include "Utils.h"

namespace Engine{
    void printBitboard(uint64_t bitboard) {
        for (int i = 63; i >= 0; i--) {
            if ((i + 1) % 8 == 0)
                std::cout << std::endl;
            if (bitboard & (1LL << i)) {
                std::cout << 1 << ' ';
            } else std::cout << 0 << ' ';
        }
        std::cout << std::endl;
    }
}