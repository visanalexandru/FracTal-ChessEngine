#include "Zobrist.h"

namespace Engine {


    namespace Zobrist {
        uint64_t ZobristPieces[12][64];
        uint64_t ZobristCastling[4];
        uint64_t ZobristEnPassant[8];
        uint64_t ZobristSide;


        void initZobrist() {
            std::mt19937_64 rdGen;
            rdGen.seed(1);

            for (int i = 0; i < 12; i++) {
                for (int k = 0; k < 64; k++) {
                    ZobristPieces[i][k] = rdGen();
                }
            }
            for (int i = 0; i < 4; i++)
                ZobristCastling[i] = rdGen();
            for (int i = 0; i < 8; i++)
                ZobristEnPassant[i] = rdGen();
            ZobristSide = rdGen();

        }
    }

}