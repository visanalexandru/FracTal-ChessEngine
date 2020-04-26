#ifndef MOVE_H
#define MOVE_H
#include "Position.h"
#include "Types.h"
#include<string>
#include<cstdint>
namespace Engine{



	class Move{
		private:
			Position origin;
			Position destination;
			MoveType move_type;
			Piece taken;
			Piece promote_to;
		public:
			Move(MoveType type,Position start,Position end,Piece piece_taken,Piece promotion);
			Move();
			Position getOrigin() const;
			Position getDestination() const;
			MoveType getType() const;
			Piece getTaken() const;
			Piece getPromotion() const;
			std::string toString() const;
	};
};

#endif
