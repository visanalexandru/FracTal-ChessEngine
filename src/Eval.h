#ifndef CHESSENGINE_EVAL_H
#define CHESSENGINE_EVAL_H

#include "MoveGen.h"
#include "TranspositionTable.h"
#include <algorithm>
#include <fstream>
#include "Logger.h"
#include<chrono>
namespace Engine {
    class Eval {
    private:
       static bool compare(const Move&a,const Move&b);

        Board&internal_board;
        MoveGen movegen;
        const int infinity=99999999;
        const int checkmate=-9999999;
        const int stalemate=0;
        const int threefold_repetition=0;
        int root;//the root position of the search
        std::chrono::time_point<std::chrono::system_clock> last_time;
        float allotted_time;
        bool premature_stop;
        bool hasTimeLeft() const;
        void setRating(std::vector<Move>&moves);
        int getHeuristicScore(Color color) const;
        int getBonusPieceScore(PieceType piece,Color color) const;
        int getScore() const;
        int getPhase() const;
        int getBonusKingScore(Color color) const;
        int getMaterialScore(Color color) const;
        int getBonusScore(Color color) const;
        int megamax(int depth,int alpha,int beta,Color color);
        Move megamaxRoot(int depth,Color color);
        bool isThreefoldRepetition() const;
    public:
        Eval(Board&board);
        Move getBestMove(float allotted);
    };
};


#endif
