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
		public:
			Move(MoveType type,Position start,Position end,Piece piece_taken);
			Move();
			Position getOrigin() const;
			Position getDestination() const;
			MoveType getType() const;
			Piece getTaken() const;
	};
};

#endif
