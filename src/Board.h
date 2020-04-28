#ifndef BOARD_H
#define BOARD_H

#include<cstdint>
#include<iostream>
#include<stack>
#include "GameState.h"
#include "Move.h"
#include "Types.h"
#include<vector>
namespace Engine{

	class Board{
		private:
			Piece pieces[8][8];
			std::stack<GameState> history;
			GameState current_game_state;
			void setPieceAt(Position po,Piece piece);
			void makeNormalMove(Move move);
			void makeQueenSideCastle();
			void makeKingSideCastle();
			void makeEnPassant(Move move);
			void makePromotion(Move move);

			void undoNormalMove(Move move);//use this for promotions also
			void undoQueenSideCastle(Move move);
			void undoKingSideCastle(Move move);
			void undoEnPassant(Move move);

			void initBoard();
		public:

			//These do not check if the move is legal,just provide
			//a way to easily create moves assuming the move is legal
			Board();
			std::string print() const;
			Piece getPieceAt(Position position) const;
            GameState getGamestate() const;
			void makeMove(Move move);
			void undoLastMove();
			Color getTurn() const;
	};
}


#endif
