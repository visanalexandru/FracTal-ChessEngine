
#ifndef CHESSENGINE_UTILS_H
#define CHESSENGINE_UTILS_H

#include <cstdint>
#include <iostream>

namespace Engine{
        inline uint64_t getLsb(uint64_t to_get){
            return to_get&(-to_get);
        }
        inline uint64_t popLsb(uint64_t&to_pop){
            uint64_t lsb=getLsb(to_pop);
            to_pop-=lsb;
            return lsb;
        }
        //returns the index of the first set bit  forward/reverse
        //if the parameter is 0 the result is undefined
        inline int bitScanForward(uint64_t x){
            return __builtin_ffsll(x)-1;
        }
        inline int bitScanReverse(uint64_t x){
            return 63-__builtin_clzll(x);
        }


        //just for debugging
        void printBitboard(uint64_t bitboard);
}


#endif
