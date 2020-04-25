#include "Move.h"



namespace Engine{

	Move::Move(MoveType type,Position start,Position end,uint16_t game_state,Piece piece_taken):
	move_type(type),
	origin(start),
	destination(end),
	previous_game_state(game_state),
	taken(piece_taken){


	}



	Position Move::getOrigin() const{
		return origin;
	}
	Position Move::getDestination() const{
		return destination;
	}
	uint16_t Move::getPreviousGameState() const{
		return previous_game_state;
	}
	MoveType Move::getType() const{
		return move_type;
	}
	Piece Move::getTaken() const{
		return taken;
	}
}
