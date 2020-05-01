#ifndef BITENGINE_BOARD_H
#define BITENGINE_BOARD_H


#include <cstdint>
#include <iostream>
#include <stack>
#include <sstream>
#include "Tables.h"
#include "Types.h"
#include "GameState.h"

namespace BitEngine {
    class Board {
    private:
        void makeNormalMove(const Move&move);
        void makePromotion(const Move &move);
        void makeEnPassant(const Move&move);

        const uint64_t WRookLPosition=0;
        const uint64_t  WRookRPosition=1LL<<7;
        const uint64_t  WKingPosition=1LL<<3;
        const uint64_t BRookLPosition=1LL<<63;
        const uint64_t  BRookRPosition=1LL<<56;
        const uint64_t  BKingPosition=1LL<<59;
        std::stack<GameState> history;

    public:
        static void print(uint64_t bitboard);
        Board();
        uint64_t bitboards[12];
        GameState gamestate;
        uint64_t getWhitePieces() const;
        uint64_t getBlackPieces() const;
        uint64_t getAll() const;
        Color getTurn() const;
        std::string prt() const;
        PieceType getPieceAt(uint64_t position) const;
        void setPieceAt(uint64_t position,PieceType piece);
        void removePieceAt(uint64_t position,PieceType piece);
        void makeMove(const Move&move);
        void loadFen(const std::string&fen);
    };
}

#endif
