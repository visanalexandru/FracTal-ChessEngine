

#ifndef CHESSENGINE_ZOBRIST_H
#define CHESSENGINE_ZOBRIST_H

#include <cstdint>
#include <random>

namespace Engine {
    namespace Zobrist {
        extern uint64_t ZobristPieces[12][64];
        //0 king side white 1 queen side white 2 king side black 3 queen side black
        extern uint64_t ZobristCastling[4];
        extern uint64_t  ZobristEnPassant[8];
        extern  uint64_t  ZobristSide;

        void initZobrist();
    }
}
#endif
