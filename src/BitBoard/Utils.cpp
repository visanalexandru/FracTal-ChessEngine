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
        return __builtin_ffsll(x);
    }
    int bitScanReverse(uint64_t x){
        return __builtin_clzll(x);
    }
}