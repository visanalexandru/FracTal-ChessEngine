#ifndef AN_PARSER_H
#define AN_PARSER_H

#include "BitBoard/Board.h"
#include "BitBoard/MoveGen.h"

namespace Engine {
    namespace AnParser {
        BitEngine::Move getMove(const std::string &an, BitEngine::Board &board);
    }
}
#endif
