#include "Move.h"



namespace Engine{

	Move::Move(MoveType type,Position start,Position end,Piece piece_taken,Piece promotion):
	move_type(type),
	origin(start),
	destination(end),
	taken(piece_taken),
	promote_to(promotion){


	}

	Move::Move():move_type(MoveType::Null),
	taken(Piece::None)	
	{



	}


	Position Move::getOrigin() const{
		return origin;
	}
	Position Move::getDestination() const{
		return destination;
	}
	MoveType Move::getType() const{
		return move_type;
	}
	Piece Move::getTaken() const{
		return taken;
	}
	Piece Move::getPromotion() const{
		return promote_to;
	}

	std::string Move::toString() const{
		std::string result;
		result+=(char)(origin.x+'a');
		result+=(char)(origin.y+'1');
		
		result+=(char)(destination.x+'a');
		result+=(char)(destination.y+'1');

		if(move_type==MoveType::Promote || move_type==MoveType::CaptureAndPromote){
			result+=tolower(getChar(promote_to));
		}


		return result;
	}
}
