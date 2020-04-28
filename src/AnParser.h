#ifndef AN_PARSER_H
#define AN_PARSER_H
#include "Board.h"
#include "Move.h"
#include "MoveGen.h"

namespace Engine{
	namespace AnParser{
		Move getMove(const std::string&an,Board&board);
	}
}
#endif
