#include "Eval.h"

namespace Engine {
    Eval::Eval(Board &board) : internal_board(board), movegen(board) {

    }

    int Eval::getScore() const {
        int white_pawn_score = popCount(internal_board.getBitboard(WPawn)) * pawn_value;
        int black_pawn_score = -popCount(internal_board.getBitboard(BPawn)) * pawn_value;

        int white_knight_score = popCount(internal_board.getBitboard(WKnight)) * knight_value;
        int black_knight_score = -popCount(internal_board.getBitboard(BKnight)) * knight_value;

        int white_bishop_score = popCount(internal_board.getBitboard(WBishop)) * bishop_value;
        int black_bishop_score = -popCount(internal_board.getBitboard(BBishop)) * bishop_value;

        int white_rook_score = popCount(internal_board.getBitboard(WRook)) * rook_value;
        int black_rook_score = -popCount(internal_board.getBitboard(BRook)) * rook_value;

        int white_queen_score = popCount(internal_board.getBitboard(WQueen)) * queen_value;
        int black_queen_score = -popCount(internal_board.getBitboard(BQueen)) * queen_value;

        return white_pawn_score + black_pawn_score + white_knight_score + black_knight_score + white_bishop_score +
               black_bishop_score + white_rook_score + black_rook_score + white_queen_score + black_queen_score;
    }
    int Eval::score(Engine::Move a) {
        int to_return=0;
        if(a.getTaken()!=Piece::None)
            to_return++;
        if(a.getType()==MoveType::Promote)
            to_return+=10;
        return to_return;
    }
    bool Eval::compare(Move a,Move b) {
        return score(a)>score(b);
    }

    Move Eval::getBestMove() {
        std::cout<<"current score"<<getScore()<<std::endl;
        int alpha=-infinity,beta=infinity;
        node best=megamax(6,-infinity,infinity,internal_board.getTurn());
        return best.move;
    }

    Eval::node Eval::megamax(int depth,int alpha,int beta,Color color){
        if (depth == 0) {
            int score = getScore();
            if (color == White)
                return {score,0};
            return {-score,0};
        }
        std::vector<Move> moves = movegen.getAllMoves();
        node bestnode{-infinity,depth};

        if(moves.size()==0){
            if(movegen.isInCheck(color))
                bestnode.value=checkmate;
            else bestnode.value=stalemate;
        }
        std::sort(moves.begin(),moves.end(),compare);

        for (const Move &move:moves) {
            internal_board.makeMove(move);
            node next=megamax(depth - 1,-beta,-alpha,getOpposite(color));
            int down = -next.value;
            internal_board.undoLastMove();

            if (down > bestnode.value || (down==bestnode.value && bestnode.depth<next.depth)) {
                bestnode.move = move;
                bestnode.value = down;
                bestnode.depth=next.depth;
            }

            alpha=std::max(alpha,bestnode.value);
            if(alpha>=beta)
                break;
        }
        return bestnode;
    }
}