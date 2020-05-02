
#ifndef BITENGINE_UTILS_H
#define BITENGINE_UTILS_H

#include <cstdint>
#include <iostream>

namespace BitEngine{
        uint64_t getLsb(uint64_t to_get);
        uint64_t popLsb(uint64_t&to_pop);
        //returns the index of the first set bit  forward/reverse
        //if the parameter is 0 the result is undefined
        int bitScanForward(uint64_t x);
        int bitScanReverse(uint64_t x);


        //just for debugging
        void printBitboard(uint64_t bitboard);
}


#endif
