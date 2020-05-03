#include "Utils.h"

namespace BitEngine{
    uint64_t getLsb(uint64_t to_get) {
        return to_get&(-to_get);
    }
    uint64_t popLsb(uint64_t&to_pop) {
        uint64_t lsb=getLsb(to_pop);
        to_pop-=lsb;
        return lsb;
    }
    int bitScanForward(uint64_t x){
        return __builtin_ffsll(x)-1;
    }
    int bitScanReverse(uint64_t x){
        return 63-__builtin_clzll(x);
    }

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