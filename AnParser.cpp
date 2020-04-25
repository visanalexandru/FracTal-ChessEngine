#include "AnParser.h"

namespace Engine{
	namespace AnParser{
		bool isUppercase(char a){
			return a>='A' && a<='Z';
		}

		Move getMove(const std::string&an,Board&board){
			int cursor=0;
			bool capture=false;
			Piece captured=Piece::None;
			
			if(isUppercase(an[0]))//piece indicator,just ignore it
				cursor++;
			Position origin=parsePosition(an,cursor);
			if(an[cursor]=='x')
				capture=true;
			Position destination=parsePosition(an,cursor);

			if(capture)
				captured=board.getPieceAt(destination);
		}

		Position parsePosition(const std::string&an,int&cursor){
			char file=an[cursor++];
			char rank=an[cursor++];
		
			return Position((int)(file-'a'),(int)(rank-'1'));
		}

	}
}
