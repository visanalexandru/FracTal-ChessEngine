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

			void generatePawnMoves(Position a,std::vector<Move>&moves);


			void makeNormalMove(Move move);
			void makeQueenSideCastle();
			void makeKingSideCastle();
			void makeEnPassant(Move move);

			void initBoard();
		public:

			//These do not check if the move is legal,just provide
			//a way to easily create moves assuming the move is legal
			Move createNormal(Position a,Position b) const;
			Move createPromotion(Position a,Position b,Piece promote_to) const;
			Move createDoublePawnPush(Position a,Position b) const;

			//position of the pawn moved,position of 
			//the square destination, position of the captured pawn
			Move createEnPassant(Position a,Position b) const;
			Move createQueenSideCastle() const;
			Move createKingSideCastle() const;


			Board();
			void print() const;
			Piece getPieceAt(Position position) const;
			void makeMove(Move move);
			void undoLastMove();
			Color getTurn() const;
	};
}


#endif
