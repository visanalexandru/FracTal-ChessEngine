#include "Eval.h"

namespace Engine {
    Eval::Eval(Board &board) : internal_board(board), movegen(board) {

    }

    int Eval::getMaterialScore(Color color) const {
        int pawn_score = popCount(internal_board.getBitboard(getPiece(PieceType::Pawn, color))) * pawn_value;
        int knight_score = popCount(internal_board.getBitboard(getPiece(PieceType::Knight, color))) * knight_value;
        int bishop_score = popCount(internal_board.getBitboard(getPiece(PieceType::Bishop, color))) * bishop_value;
        int rook_score = popCount(internal_board.getBitboard(getPiece(PieceType::Rook, color))) * rook_value;
        int queen_score = popCount(internal_board.getBitboard(getPiece(PieceType::Queen, color))) * queen_value;
        return pawn_score + knight_score + bishop_score + rook_score + queen_score;
    }

    int Eval::getBonusPieceScore(PieceType piece, Color color) const {
        int score = 0, bonus, pos;
        int piece_index = static_cast<int>(piece);
        uint64_t positions = internal_board.getBitboard(getPiece(piece, color));

        while (positions) {
            pos = bitScanForward(popLsb(positions));
            if (color == White)
                bonus = Tables::Bonus[piece_index][63 - pos];
            else bonus = Tables::Bonus[piece_index][pos];

            score += bonus;
        }
        return score;

    }

    int Eval::getBonusScore(Color color) const {
        return getBonusPieceScore(PieceType::Pawn, color) + getBonusPieceScore(PieceType::Knight, color) +
               getBonusPieceScore(PieceType::Bishop, color) + getBonusPieceScore(PieceType::Rook, color) +
               getBonusPieceScore(PieceType::Queen, color) + getBonusPieceScore(PieceType::King, color);
    }

    int Eval::getScore() const {
        return getMaterialScore(White) - getMaterialScore(Black)
               + getBonusScore(White) - getBonusScore(Black);
    }

    int Eval::score(Engine::Move a) {
        int to_return = 0;
        if (a.getTaken() != Piece::None)
            to_return++;
        if (a.getType() == MoveType::Promote)
            to_return += 10;
        return to_return;
    }

    bool Eval::compare(Move a, Move b) {
        return score(a) > score(b);
    }

    Move Eval::getBestMove() {
        int alpha = -infinity, beta = infinity;
        return megamaxRoot(6,internal_board.getTurn());
    }

    Move Eval::megamaxRoot(int depth,Color color) {
        int alpha = -infinity, beta = infinity, best = -infinity;
        Move to_return;
        std::vector<Move> moves = movegen.getAllMoves();
        std::sort(moves.begin(), moves.end(), compare);
        for (const Move &move:moves) {
            internal_board.makeMove(move);
            int down = -megamax(depth - 1, -beta, -alpha, getOpposite(color));
            internal_board.undoLastMove();
            if (down > best) {
                best = down;
                to_return = move;
            }
            alpha = std::max(alpha, best);
            if (alpha >= beta)
                break;
        }
        return to_return;
    }

    int Eval::megamax(int depth, int alpha, int beta, Color color) {
        if (depth == 0) {
            int score = getScore();
            if (color == Black)
                score = -score;
            return score;
        }
        std::vector<Move> moves = movegen.getAllMoves();
        if (moves.size() == 0) {
            if (movegen.isInCheck(color))
                return checkmate + depth;
            return stalemate;
        }
        int best = -infinity;
        std::sort(moves.begin(), moves.end(), compare);
        for (const Move &move:moves) {
            internal_board.makeMove(move);
            int down = -megamax(depth - 1, -beta, -alpha, getOpposite(color));
            internal_board.undoLastMove();
            best = std::max(best, down);
            alpha = std::max(alpha, best);
            if (alpha >= beta)
                break;
        }
        return best;
    }
}