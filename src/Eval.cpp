#include <cstring>
#include "Eval.h"

namespace Engine {
    Eval::Eval(Board &board) : internal_board(board), movegen(board), history_heuristic() {

    }

    int Eval::getMaterialScore(Color color) const {
        int pawn_score =
                popCount(internal_board.getBitboard(getPiece(PieceType::Pawn, color))) * getPieceValue(PieceType::Pawn);
        int knight_score = popCount(internal_board.getBitboard(getPiece(PieceType::Knight, color))) *
                           getPieceValue(PieceType::Knight);
        int bishop_score = popCount(internal_board.getBitboard(getPiece(PieceType::Bishop, color))) *
                           getPieceValue(PieceType::Bishop);
        int rook_score =
                popCount(internal_board.getBitboard(getPiece(PieceType::Rook, color))) * getPieceValue(PieceType::Rook);
        int queen_score = popCount(internal_board.getBitboard(getPiece(PieceType::Queen, color))) *
                          getPieceValue(PieceType::Queen);
        return pawn_score + knight_score + bishop_score + rook_score + queen_score;
    }

    void Eval::clearHistory() {
        memset(history_heuristic,0,sizeof(history_heuristic));
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

    int Eval::getPhase() const {
        int knights_count =
                popCount(internal_board.getBitboard(WKnight)) + popCount(internal_board.getBitboard(BKnight));
        int bishops_count =
                popCount(internal_board.getBitboard(WBishop)) + popCount(internal_board.getBitboard(BBishop));
        int rooks_count = popCount(internal_board.getBitboard(WRook)) + popCount(internal_board.getBitboard(BRook));
        int queens_count = popCount(internal_board.getBitboard(WQueen)) + popCount(internal_board.getBitboard(BQueen));
        int phase = 24 - bishops_count - knights_count - 2 * rooks_count - 4 * queens_count;
        phase = (phase * 256 + 12) / 24;
        return phase;
    }

    int Eval::getBonusKingScore(Color color) const {
        int king_opening, king_ending;
        int phase = getPhase();
        if (color == White) {
            int pos = bitScanForward(internal_board.getBitboard(WKing));
            king_opening = Tables::KingBonus[0][63 - pos];
            king_ending = Tables::KingBonus[1][63 - pos];
        } else {
            int pos = bitScanForward(internal_board.getBitboard(BKing));
            king_opening = Tables::KingBonus[0][pos];
            king_ending = Tables::KingBonus[1][pos];
        }
        int eval = ((king_opening * (256 - phase)) + (king_ending * phase)) / 256;
        return eval;
    }

    int Eval::getBonusScore(Color color) const {
        return getBonusPieceScore(PieceType::Pawn, color) + getBonusPieceScore(PieceType::Knight, color) +
               getBonusPieceScore(PieceType::Bishop, color) + getBonusPieceScore(PieceType::Rook, color) +
               getBonusPieceScore(PieceType::Queen, color) + getBonusKingScore(color);
    }

    int Eval::getScore() const {
        return getMaterialScore(White) - getMaterialScore(Black)
               + getBonusScore(White) - getBonusScore(Black);
    }

    int Eval::getHeuristicScore(Color color) const {
        int score = getScore();
        if (color == Black)
            return -score;
        return score;
    }

    bool Eval::compare(const Move &a, const Move &b) {
        return a.getScore() > b.getScore();
    }

    void Eval::setRating(std::vector<Move> &moves) {
        Color color = internal_board.getTurn();
        Transposition here = TranspositionTable::getInstance().getTransposition(
                internal_board.getGameState().zobrist_key);
        Move best_move = here.getBestMove();
        int score;
        for (Move &move:moves) {
            if (here.getType() != NodeType::Null && move == best_move) {
                move.setScore(infinity);
            } else {
                score = 0;
                if (move.getType() == MoveType::Promote) {
                    PieceType promotion = getPieceType(move.getPromotion());
                    score += getPieceValue(promotion) * 100+2000;
                }
                else{
                    if (move.isCapture()) {
                        score += getPieceValue(getPieceType(move.getTaken())) * 100+1000;
                        score -= getPieceValue(getPieceType(move.getMoved()));
                    }
                    else {
                       score += history_heuristic[color][bitScanForward(move.getOrigin())][bitScanForward(move.getDestination())];
                    }
                }
                move.setScore(score);
            }
        }
    }

    Move Eval::getBestMove(float allotted) {
        last_time = std::chrono::system_clock::now();
        allotted_time = allotted;
        premature_stop = false;
        int alpha = -infinity, beta = infinity;
        Color color = internal_board.getTurn();
        Move bestmove;
        nodes = 0;
        int score = 0;
        clearHistory();
        std::vector<Move> legal = movegen.getAllMoves();
        if (legal.size()) {
            if (legal.size() == 1)
                bestmove = legal[0];
            else {
                bestmove = legal[0];//random legal move
                int depth = 1;
                while (true) {
                    Move aux = megamaxRoot(depth, color, score);
                    if (!premature_stop) {
                        printInfo(depth, aux, score);
                        bestmove = aux;
                    } else break;
                    depth++;
                }
            }
        }
        return bestmove;
    }

    void Eval::printInfo(int current_depth, const Move &current_best_move, int score) const {
        std::cout << "info depth " << current_depth << " ";
        std::cout << "nodes " << nodes << " ";
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - last_time;
        double nps=(double)(nodes)/diff.count();
        std::cout << "score cp " << score << " ";
        std::cout << "nps " <<(int)nps << " ";
        std::cout << "pv ";

        Engine::Move best = current_best_move;
        int depth = 0;
        while (depth < current_depth && best.getType() != MoveType::Null) {
            std::cout << best.toString() << " ";
            internal_board.makeMove(best);
            depth++;

            Transposition next = TranspositionTable::getInstance().getTransposition(
                    internal_board.getGameState().zobrist_key);
            if (next.getType() != NodeType::Null) {
                best = next.getBestMove();
            } else break;
        }
        //reset moves
        while (depth) {
            internal_board.undoLastMove();
            depth--;
        }
        std::cout << std::endl;
    }

    bool Eval::hasTimeLeft() const {
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - last_time;
        return (diff.count()) < allotted_time;
    }

    Move Eval::megamaxRoot(int depth, Color color, int &score) {
        int alpha = -infinity, beta = infinity, best = -infinity;
        Move to_return;
        root = internal_board.getNumMoves();
        std::vector<Move> moves = movegen.getAllMoves();
        setRating(moves);
        std::sort(moves.begin(), moves.end(), compare);
        for (const Move &move:moves) {
            if (!hasTimeLeft()) {
                premature_stop = true;
                break;
            }
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
        if (!premature_stop) {
            score = best;
            TranspositionTable::getInstance().addEntry(
                    Transposition(NodeType::Exact, internal_board.getGameState().zobrist_key, depth, best, to_return));
        }//else don't add this node in the transposition table because it was not entirely evaluated

        return to_return;
    }

    bool Eval::isThreefoldRepetition() const {
        const std::deque<GameState> &history = internal_board.getHistory();
        int repetitions = 0;
        uint64_t hash = internal_board.getGameState().zobrist_key;

        for (int k = history.size() - 2; k >= 0; k -= 2) {
            if (history[k].zobrist_key == hash) {
                if (k >= root)//found repetition in current search space
                    return true;
                //else we need 2 repetitions that happen before the root
                repetitions++;
                if (repetitions == 2)
                    return true;
            }
        }
        return false;
    }

    int Eval::quiescenceSearch(int alpha, int beta, Color color) {
        int stand_pat = getHeuristicScore(color);
        nodes++;
        if (stand_pat >= beta)
            return beta;
        alpha = std::max(alpha, stand_pat);

        std::vector<Move> moves = movegen.getAllCaptures();
        setRating(moves);
        std::sort(moves.begin(), moves.end(), compare);

        for (const Move &move:moves) {
            internal_board.makeMove(move);
            int score = -quiescenceSearch(-beta, -alpha, getOpposite(color));
            internal_board.undoLastMove();

            if (score >= beta)
                return beta;
            alpha = std::max(alpha, score);
        }
        return alpha;
    }

    //see https://en.wikipedia.org/wiki/Negamax with transposition tables
    int Eval::megamax(int depth, int alpha, int beta, Color color) {
        uint64_t hash = internal_board.getGameState().zobrist_key;
        int alphaOrig = alpha;
        Transposition node = TranspositionTable::getInstance().getTransposition(hash);
        if (node.getType() != NodeType::Null && node.getDepth() >= depth) {
            if (node.getType() == NodeType::Exact)
                return node.getValue();
            else if (node.getType() == NodeType::LowerBound)
                alpha = std::max(alpha, node.getValue());
            else if (node.getType() == NodeType::UpperBound)
                beta = std::min(beta, node.getValue());
            if (alpha >= beta)
                return node.getValue();
        }

        if (depth == 0) {
            return quiescenceSearch(-infinity, infinity, color);
        }
        std::vector<Move> moves = movegen.getAllMoves();
        if (moves.size() == 0) {
            if (movegen.isInCheck(color))
                return checkmate - depth;//try to delay the checkmate
            return stalemate;
        }
        int best = -infinity;
        Move best_move;
        setRating(moves);
        std::sort(moves.begin(), moves.end(), compare);
        for (const Move &move:moves) {
            if (!hasTimeLeft()) {
                premature_stop = true;
                break;
            }
            internal_board.makeMove(move);
            int down = -megamax(depth - 1, -beta, -alpha, getOpposite(color));
            internal_board.undoLastMove();
            if (down > best) {
                best = down;
                best_move = move;
            }
            alpha = std::max(alpha, best);
            if (alpha >= beta) {
                history_heuristic[color][bitScanForward(move.getOrigin())][bitScanForward(move.getDestination())] +=
                        depth * depth;
                break;
            }
        }

        if (!premature_stop) {
            //save the position in the transposition table
            NodeType node_type;
            if (best <= alphaOrig)//did not affect the score
                node_type = NodeType::UpperBound;
            else if (best >= beta)
                node_type = NodeType::LowerBound;
            else node_type = NodeType::Exact;
            TranspositionTable::getInstance().addEntry(Transposition(node_type, hash, depth, best, best_move));
        }
        return best;
    }
}