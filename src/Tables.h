#ifndef CHESSENGINE_TABLES_H
#define CHESSENGINE_TABLES_H

#include <cstdint>
#include "Utils.h"

namespace Engine {
    namespace Tables {

        const uint64_t ClearRank[] = {0xFFFFFFFFFFFFFF00,
                                      0xFFFFFFFFFFFF00FF,
                                      0xFFFFFFFFFF00FFFF,
                                      0xFFFFFFFF00FFFFFF,
                                      0xFFFFFF00FFFFFFFF,
                                      0xFFFF00FFFFFFFFFF,
                                      0xFF00FFFFFFFFFFFF,
                                      0x00FFFFFFFFFFFFFF
        };
        const uint64_t ClearFile[] = {
                                      0x7F7F7F7F7F7F7F7F,
                                      0xBFBFBFBFBFBFBFBF,
                                      0xDFDFDFDFDFDFDFDF,
                                      0xEFEFEFEFEFEFEFEF,
                                      0xF7F7F7F7F7F7F7F7,
                                      0xFBFBFBFBFBFBFBFB,
                                      0xFDFDFDFDFDFDFDFD,
                                      0xFEFEFEFEFEFEFEFE,
        };
        const uint64_t MaskRank[] = {0xFF,
                                     0xFF00,
                                     0xFF0000,
                                     0xFF000000,
                                     0xFF00000000,
                                     0xFF0000000000,
                                     0xFF000000000000,
                                     0xFF00000000000000
        };

        const uint64_t MaskFile[] = {0x8080808080808080,
                                     0x4040404040404040,
                                     0x2020202020202020,
                                     0x1010101010101010,
                                     0x808080808080808,
                                     0x404040404040404,
                                     0x202020202020202,
                                     0x0101010101010101,
        };

        enum Direction{
            North,
            NorthEast,
            East,
            SouthEast,
            South,
            SouthWest,
            West,
            NorthWest
        };

        extern uint64_t AttackTables[64][8];
        void initializeAttackTables();
    }
}
#endif
