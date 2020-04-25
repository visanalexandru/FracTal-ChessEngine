#ifndef AN_PARSER_H
#define AN_PARSER_H
#include "Board.h"
#include "Move.h"

namespace Engine{
	namespace AnParser{
		bool isUppercase(char a);
		Position parsePosition(const std::string&an,int&cursor);
		Move getMove(const std::string&an,Board&board);
	}
}
#endif
