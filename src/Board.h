#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H


#include <cstdint>
#include <iostream>
#include <deque>
#include <sstream>
#include "Tables.h"
#include "Types.h"
#include "GameState.h"
#include "Zobrist.h"

namespace Engine {
    class Board {
    private:
        void makeNormalMove(const Move&move);
        void makePromotion(const Move &move);
        void makeEnPassant(const Move&move);
        void makeDoublePawnPush(const Move&move);
        void makeKingSideCastle();
        void makeQueenSideCastle();
        void updateCastlingRights(const Move&move);


        //We don't need to update the zobrist key for in  the undo functions as a zobrist history is held in the game state stack
        void undoNormalMove(const Move&move);
        void undoEnPassant(const Move&move);
        void undoPromotion(const Move&move);
        void undoKingSideCastle();
        void undoQueenSideCastle();
        std::deque<GameState> history;
        uint64_t bitboards[12];
        GameState gamestate;
        void resetBoard();
        int numMoves;
    public:
        Board();
        const GameState&getGameState();
        uint64_t getBitboard(Piece type);
        uint64_t  getPieces(Color color) const;
        uint64_t getAll() const;
        int getNumMoves() const;
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
