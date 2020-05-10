#include "Eval.h"

namespace Engine {
    Eval::Eval(Board &board) : internal_board(board), movegen(board),Ttable(10000000){

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
        Ttable.addEntry(Transposition(NodeType::Exact,internal_board.getGameState().zobrist_key,depth,best));
        return to_return;
    }

    //see https://en.wikipedia.org/wiki/Negamax with transposition tables
    int Eval::megamax(int depth, int alpha, int beta, Color color) {

        uint64_t hash=internal_board.getGameState().zobrist_key;
        int alphaOrig=alpha;
        Transposition node=Ttable.getTransposition(hash);
        if(node.getType()!=NodeType::Null &&node.getDepth()>=depth){
            if(node.getType()==NodeType::Exact)
                return node.getValue();
            else if(node.getType()==NodeType::LowerBound)
                alpha=std::max(alpha,node.getValue());
            else if(node.getType()==NodeType::UpperBound)
                beta=std::min(beta,node.getValue());
            if(alpha>=beta)
                return node.getValue();
        }

        if (depth == 0) {
            int score = getScore();
            if (color == Black)
                score = -score;
            return score;
        }
        std::vector<Move> moves = movegen.getAllMoves();
        if (moves.size() == 0) {
            if (movegen.isInCheck(color))
                return checkmate - depth;//try to delay the checkmate
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


        //save the position in the transposition table
        NodeType node_type;
        if(best<=alphaOrig)//did not affect the score
            node_type=NodeType::UpperBound;
        else if(best>=beta)
            node_type=NodeType::LowerBound;
        else node_type=NodeType::Exact;
        Ttable.addEntry(Transposition(node_type,hash,depth,best));
        return best;
    }
}