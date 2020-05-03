#ifndef AN_PARSER_H
#define AN_PARSER_H

#include "Board.h"
#include "MoveGen.h"

namespace Engine {
    namespace AnParser {
        BitEngine::Move getMove(const std::string &an, BitEngine::Board &board);
    }
}
#endif
