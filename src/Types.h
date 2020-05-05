
#ifndef CHESSENGINE_TYPES_H
#define CHESSENGINE_TYPES_H

#include <cstdint>
namespace Engine{
    enum Piece{
        WPawn,
        BPawn,
        WKnight,
        BKnight,
        WBishop,
        BBishop,
        WRook,
        BRook,
        WQueen,
        BQueen,
        WKing,
        BKing,
        None,
    };
    enum class PieceType{
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King,
    };
    enum Color {
        White,
        Black,
    };

    enum State : uint8_t {
        turnColor = 1,//0 for white 1 for black
        canCastleKingSideWhite = 1 << 1,
        canCastleQueenSideWhite = 1 << 2,
        canCastleKingSideBlack = 1 << 3,
        canCastleQueenSideBlack = 1 << 4,
    };


    enum MoveType{
        Null,
        Normal,//can be a quiet move or a capture
        QueenSideCastle,
        KingSideCastle,
        DoublePawnPush,
        EnPassant,
        Promote,
    };

    const uint64_t WRookRPosition=1;
    const uint64_t  WRookLPosition=1LL<<7;
    const uint64_t  WKingPosition=1LL<<3;
    const uint64_t BRookLPosition=1LL<<63;
    const uint64_t  BRookRPosition=1LL<<56;
    const uint64_t  BKingPosition=1LL<<59;

    char getPieceChar(Piece type);
    inline Color  getOpposite(Color color){
        return static_cast<Color>(1-color);
    }
    inline Piece getPiece(PieceType type,Color color){
        int index= static_cast<int>(type)*2+color;
        return static_cast<Piece>(index);
    }
}

#endif
