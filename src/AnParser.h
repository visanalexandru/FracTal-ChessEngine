#ifndef CHESSENGINE_AN_PARSER_H
#define CHESSENGINE_AN_PARSER_H

#include "Board.h"
#include "MoveGen.h"

namespace Engine {
    namespace AnParser {
        Engine::Move getMove(const std::string &an, Engine::Board &board);
    }
}
#endif
