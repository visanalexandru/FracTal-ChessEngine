#include "Move.h"



namespace Engine{

	Move::Move(MoveType type,Position start,Position end,uint8_t piece_moved,uint8_t piece_taken,uint8_t promotion):
	move_type(type),
	origin(start),
	destination(end),
	taken(piece_taken),
	promote_to(promotion),
	moved(piece_moved){


	}

	Move::Move():move_type(MoveType::Null),
	taken(makePiece(Piece::None,false))
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
	uint8_t Move::getMoved() const {
	    return moved;
	}
	uint8_t Move::getTaken() const{
		return taken;
	}
	uint8_t Move::getPromotion() const{
		return promote_to;
	}

	std::string Move::toString() const{
		std::string result;
		result+=(char)(origin.x+'a');
		result+=(char)(origin.y+'1');
		
		result+=(char)(destination.x+'a');
		result+=(char)(destination.y+'1');

		if(move_type==MoveType::Promote){
			result+=tolower(getChar(promote_to));
		}


		return result;
	}
}
