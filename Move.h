#ifndef MOVE_H
#define MOVE_H
#include "Position.h"
#include "Types.h"
#include<cstdint>

namespace Engine{



	class Move{
		private:
			const Position origin;
			const Position destination;
			const MoveType move_type;
			const Piece taken;
			const Piece piece_before_promotion;
			const uint16_t previous_game_state;
		public:
			Move(MoveType type,Position start,Position end,uint16_t game_state,Piece piece_taken,Piece before_promotion);
			Position getOrigin() const;
			Position getDestination() const;
			uint16_t getPreviousGameState() const;
			MoveType getType() const;
			Piece getTaken() const;
	};
};

#endif
