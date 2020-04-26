#ifndef BOARD_H
#define BOARD_H

#include<cstdint>
#include<iostream>
#include<stack>
#include "GameState.h"
#include "Move.h"
#include "Types.h"
namespace Engine{

	class Board{
		private:
			Piece pieces[8][8];
			std::stack<GameState> history;
			GameState current_game_state;
			void initBoard();
		public:

			//These do not check if the move is legal,just provide
			//a way to easily create moves assuming the move is legal
			Move createNormalMove(Position a,Position b) const;
			Move createPromotionMove(Position a,Position b,Piece promote_to) const;
			Move createDoublePawnPushMove(Position a,Position b) const;
			Move createQueenSideCastleMove() const;
			Move createKingSideCastleMove() const;


			Board();
			void print() const;
			Piece getPieceAt(Position position) const;
			void makeMove(Move move);
			void undoLastMove();
			Color getTurn() const;
	};
}


#endif
