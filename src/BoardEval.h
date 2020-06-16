#ifndef CHESSENGINE_BOARDEVAL_H
#define CHESSENGINE_BOARDEVAL_H

#include "Board.h"

namespace Engine {
    class BoardEval {
    private:
        Board &internal_board;

        const int doubled_pawn_penalty=-50;

        int getMaterialScore(Color color) const;

        int getBonusPieceScore(PieceType piece, Color color) const;

        int getBonusKingScore(Color color, int phase) const;

        int getBonusScore(Color color, int phase) const;

        int getPhase() const;

        int getDoubledPawnCount(Color color) const;

        int getPawnStructureScore(Color color) const;


    public:
        BoardEval(Board &board);

        int getScore() const;
    };
};

#endif
