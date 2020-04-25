#ifndef TYPES_H
#define TYPES_H

namespace Engine{

	enum MoveType{
		Null,
		Quiet,
		QueenSideCastle,
		KingSideCastle,
		DoublePawnPush,
		EnPassant,
		Capture,
		Promote,
		CaptureAndPromote,
	};
	enum class Piece{
		WhitePawn,
		WhiteKnight,
		WhiteBishop,
		WhiteRook,
		WhiteQueen,
		WhiteKing,
		BlackPawn,
		BlackKnight,
		BlackBishop,
		BlackRook,
		BlackQueen,
		BlackKing,
		None
	};
	enum State{
		turnColor=1,//0 for white 1 for black
		canCastleKingSideWhite=1<<2,
		canCastleQueenSideWhite=1<<3,
		canCastleKingSideBlack=1<<4,
		canCastleQueenSideBlack=1<<5,
	};
	enum Color{
		White,
		Black
	};

	inline int getValue(Piece piece){
		switch(piece){
			case Piece::WhitePawn:
				return 1;
			case Piece::WhiteKnight:
				return 3;
			case Piece::WhiteBishop:
				return 3;
			case Piece::WhiteRook:
				return 5;
			case Piece::WhiteQueen:
				return 10;


			case Piece::BlackPawn:
				return 1;
			case Piece::BlackKnight:
				return 3;
			case Piece::BlackBishop:
				return 3;
			case Piece::BlackRook:
				return 5;
			case Piece::BlackQueen:
				return 10;
		}
		return 0;//kings or empty spaces
	}
 	inline char getChar(Piece piece) {
		switch(piece){
			case Piece::WhitePawn:
				return 'P';
			case Piece::WhiteKnight:
				return 'N';
			case Piece::WhiteBishop:
				return 'B';
			case Piece::WhiteRook:
				return 'R';
			case Piece::WhiteQueen:
				return 'Q';
			case Piece::WhiteKing:
				return 'K';


			case Piece::BlackPawn:
				return 'p';
			case Piece::BlackKnight:
				return 'n';
			case Piece::BlackBishop:
				return 'b';
			case Piece::BlackRook:
				return 'r';
			case Piece::BlackQueen:
				return 'q';
			case Piece::BlackKing:
				return 'k';
		}
		return ' ';
	}



}

#endif 
