#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H


#include <cstdint>
#include <iostream>
#include <stack>
#include <sstream>
#include "Tables.h"
#include "Types.h"
#include "GameState.h"

namespace Engine {
    class Board {
    private:
        void makeNormalMove(const Move&move);
        void makePromotion(const Move &move);
        void makeEnPassant(const Move&move);
        void makeKingSideCastle();
        void makeQueenSideCastle();
        void updateCastlingRights(const Move&move);

        void undoNormalMove(const Move&move);
        void undoEnPassant(const Move&move);
        void undoPromotion(const Move&move);
        void undoKingSideCastle();
        void undoQueenSideCastle();
        std::stack<GameState> history;
        uint64_t bitboards[12];
        GameState gamestate;

        void resetBoard();
    public:
        Board();
        uint64_t getBitboard(Piece type);
        uint64_t  getPieces(Color color) const;
        uint64_t getAll() const;
        Color getTurn() const;
        std::string toString() const;
        Piece getPieceAt(uint64_t position) const;
        void setPieceAt(uint64_t position, Piece piece);
        void removePieceAt(uint64_t position, Piece piece);
        void makeMove(const Move&move);
        void undoLastMove();
        void loadFen(const std::string&fen);

        friend class MoveGen;
    };
}

#endif
