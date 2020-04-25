#ifndef BOARD_H
#define BOARD_H

#include<cstdint>
#include<iostream>
#include "Move.h"
#include "Types.h"
namespace Engine{

	class Board{
		private:
			Piece pieces[8][8];
			uint16_t game_state;
			void initBoard();
		public:
			Board();
			void print() const;
			Piece getPieceAt(Position position) const;
			void makeMove(Move move);
			void undoMove(Move move);
			Color getTurn() const;
	};
}


#endif
