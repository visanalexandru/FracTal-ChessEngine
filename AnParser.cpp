#include "AnParser.h"

namespace Engine{
	namespace AnParser{

		Move getMove(const std::string&an,Board&board){
		    std::vector<Move> move_list=board.getAllMoves();

		    for(Move a:move_list){
		        if(a.toString()==an)
		            return a;
		    }
		    return Move();//null move;
		}

	}
}
