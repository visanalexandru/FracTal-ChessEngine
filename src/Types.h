#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <cctype>

namespace Engine{

	enum class MoveType{
		Null,
		Normal,//can be a quiet move or a capture
		QueenSideCastle,
		KingSideCastle,
		DoublePawnPush,
		EnPassant,
		Promote,
	};
	enum class Piece{
	    Pawn,
	    Knight,
	    Bishop,
	    Rook,
	    Queen,
	    King,
		None
	};
	enum ColoredPiece: uint8_t {
        WhiteKing=5,
        BlackKing=13,
        WhitePawn=0,
        BlackPawn=8,
        WhiteBishop=2,
        BlackBishop=10,
        WhiteKnight=1,
        BlackKnight=9,
        WhiteRook=3,
        BlackRook=11,
        WhiteQueen=4,
        BlackQueen=12,
        None=6,
	};
	enum State :uint8_t {
		turnColor=1,//0 for white 1 for black
		canCastleKingSideWhite=1<<1,
		canCastleQueenSideWhite=1<<2,
		canCastleKingSideBlack=1<<3,
		canCastleQueenSideBlack=1<<4,
	};
	enum Color{
		White,
		Black,
	};



    // bits 0-2 piece type
    //bit 3 piece color - unused for empty space

    uint8_t makePiece(Piece type,bool color);
    Piece getPieceType(uint8_t pc);
    Color getPieceColor(uint8_t pc);
    int getPieceValue(uint8_t pc);
    char getChar(uint8_t pc);


}

#endif
