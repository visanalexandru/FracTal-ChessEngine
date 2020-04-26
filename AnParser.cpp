#include "AnParser.h"

namespace Engine{
	namespace AnParser{

		Move getMove(const std::string&an,Board&board){//We assume the move is legal
			Position origin(an[0]-'a',an[1]-'1');
			Position destination(an[2]-'a',an[3]-'1');
			Piece piece_moved=board.getPieceAt(origin);
			Piece captured=board.getPieceAt(destination);

			if(captured==Piece::None){
				
			}
		}

	}
}
