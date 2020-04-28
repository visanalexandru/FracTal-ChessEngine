#include "AnParser.h"

namespace Engine{
	namespace AnParser{

		Move getMove(const std::string&an,Board&board){
            MoveGen movegen(board);
		    std::vector<Move> move_list=movegen.getAllMoves();

		    for(Move a:move_list){
		        if(a.toString()==an)
		            return a;
		    }
		    return Move();//null move;
		}

	}
}
