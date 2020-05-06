#ifndef CHESSENGINE_EVAL_H
#define CHESSENGINE_EVAL_H

#include "MoveGen.h"

namespace Engine {
    class Eval {
    private:
        struct node{
            int value;
            int depth;
            Move move;
        };

        Board&internal_board;
        MoveGen movegen;
        const int pawn_value=10;
        const int knight_value=30;
        const int bishop_value=30;
        const int rook_value=50;
        const int queen_value=100;
        const int canCastle=3;
        const int infinity=999999;
        const int checkmate=-9999;
        const int stalemate=0;
        int getScore() const;
        node megamax(int depth,int alpha,int beta,Color color);
    public:
        Eval(Board&board);
        Move getBestMove();
    };
};


#endif
