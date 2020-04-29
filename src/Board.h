#ifndef BOARD_H
#define BOARD_H

#include<cstdint>
#include<iostream>
#include<stack>
#include "GameState.h"
#include "Move.h"
#include "Types.h"
#include<vector>

namespace Engine {

    class Board {
    private:
        uint8_t pieces[8][8];
        std::stack<GameState> history;
        GameState current_game_state;

        void setPieceAt(const Position &po, uint8_t piece);

        void makeNormalMove(const Move &move);

        void makeQueenSideCastle();

        void makeKingSideCastle();

        void makeEnPassant(const Move &move);

        void makePromotion(const Move &move);

        void undoNormalMove(const Move &move);//use this for promotions also
        void undoQueenSideCastle(const Move &move);

        void undoKingSideCastle(const Move &move);

        void undoEnPassant(const Move &move);

        void initBoard();

    public:

        //These do not check if the move is legal,just provide
        //a way to easily create moves assuming the move is legal
        Board();

        std::string print() const;

        uint8_t getPieceAt(const Position &position) const;

        GameState getGamestate() const;

        void makeMove(const Move &move);

        void undoLastMove();

        Color getTurn() const;
    };
}


#endif
