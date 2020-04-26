#include "Move.h"



namespace Engine{

	Move::Move(MoveType type,Position start,Position end,Piece piece_taken):
	move_type(type),
	origin(start),
	destination(end),
	taken(piece_taken){


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
}
