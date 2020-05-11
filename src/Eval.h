#ifndef CHESSENGINE_EVAL_H
#define CHESSENGINE_EVAL_H

#include "MoveGen.h"
#include "TranspositionTable.h"
#include <algorithm>
#include <fstream>
namespace Engine {
    class Eval {
    private:
       static bool compare(const Move&a,const Move&b);

        Board&internal_board;
        MoveGen movegen;
        const int pawn_value=100;
        const int knight_value=320;
        const int bishop_value=330;
        const int rook_value=500;
        const int queen_value=900;
        const int infinity=99999999;
        const int checkmate=-9999999;
        const int stalemate=0;
        TranspositionTable Ttable;
        void setRating(std::vector<Move>&moves);
        int getHeuristicScore(Color color) const;
        int getBonusPieceScore(PieceType piece,Color color) const;
        int getScore() const;
        int getMaterialScore(Color color) const;
        int getBonusScore(Color color) const;
        int megamax(int depth,int alpha,int beta,Color color);
        Move megamaxRoot(int depth,Color color);
    public:
        Eval(Board&board);
        Move getBestMove();
    };
};


#endif
