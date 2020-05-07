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
        node best = megamax(6, -infinity, infinity, internal_board.getTurn());
        return best.move;
    }

    Eval::node Eval::megamax(int depth, int alpha, int beta, Color color) {
        if (depth == 0) {
            int score = getScore();
            if (color == White)
                return {score, 0};
            return {-score, 0};
        }
        std::vector<Move> moves = movegen.getAllMoves();
        node bestnode{-infinity, depth};

        if (moves.size() == 0) {
            if (movegen.isInCheck(color))
                bestnode.value = checkmate;
            else bestnode.value = stalemate;
        }
        std::sort(moves.begin(), moves.end(), compare);
        for (const Move &move:moves) {
            internal_board.makeMove(move);
            node next = megamax(depth - 1, -beta, -alpha, getOpposite(color));
            int down = -next.value;
            internal_board.undoLastMove();

            if (down > bestnode.value || (down == bestnode.value && bestnode.depth < next.depth)) {
                bestnode.move = move;
                bestnode.value = down;
                bestnode.depth = next.depth;
            }

            alpha = std::max(alpha, bestnode.value);
            if (alpha >= beta)
                break;
        }
        return bestnode;
    }
}