#include "Eval.h"

namespace Engine {
    Eval::Eval(Board &board) : internal_board(board), movegen(board){

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
    int Eval::getPhase() const {
        int knights_count=popCount(internal_board.getBitboard(WKnight))+popCount(internal_board.getBitboard(BKnight));
        int bishops_count=popCount(internal_board.getBitboard(WBishop))+popCount(internal_board.getBitboard(BBishop));
        int rooks_count=popCount(internal_board.getBitboard(WRook))+popCount(internal_board.getBitboard(BRook));
        int queens_count=popCount(internal_board.getBitboard(WQueen))+popCount(internal_board.getBitboard(BQueen));
        int phase=24-bishops_count-knights_count-2*rooks_count-4*queens_count;
        phase = (phase * 256 + 12) / 24;
        return phase ;
    }
    int Eval::getBonusKingScore(Color color) const{
        int king_opening,king_ending;
        int phase=getPhase();
        if(color==White){
            int pos=bitScanForward(internal_board.getBitboard(WKing));
            king_opening=Tables::KingBonus[0][63-pos];
            king_ending=Tables::KingBonus[1][63-pos];
        }
        else{
            int pos=bitScanForward(internal_board.getBitboard(BKing));
            king_opening=Tables::KingBonus[0][pos];
            king_ending=Tables::KingBonus[1][pos];
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
        int score=getScore();
        if(color==Black)
            return -score;
        return  score;
    }

    bool Eval::compare(const Move&a,const Move&b) {
        return a.getScore() >b.getScore();
    }
    void Eval::setRating(std::vector<Move> &moves) {
        Color color=internal_board.getTurn();
        Transposition here=TranspositionTable::getInstance().getTransposition(internal_board.getGameState().zobrist_key);
        Move best_move=here.getBestMove();
        int score;
        for(Move&move:moves){
            if(here.getType()!=NodeType::Null && move==best_move){
                move.setScore(100000);
            }
            else{
                score=0;
                score+=getPieceValue(move.getTaken());
                score+=getPieceValue(move.getPromotion());
                move.setScore(score);
            }
        }
    }

    Move Eval::getBestMove(float allotted) {
        last_time=std::chrono::system_clock::now();
        allotted_time=allotted;
        premature_stop=false;
        int alpha = -infinity, beta = infinity;
        Color  color=internal_board.getTurn();
        Move bestmove;

        std::vector<Move> legal=movegen.getAllMoves();
        if(legal.size()){
            if(legal.size()==1)
                bestmove=legal[0];
            else{
                bestmove=legal[0];//random legal move
                int depth=1;
                while(true){
                    Move aux=megamaxRoot(depth,color);
                    if(!premature_stop){
                        std::cout<<"searched depth "<<depth<<std::endl;
                        bestmove=aux;
                    }
                    else break;
                    depth++;
                }
            }
        }
        return bestmove;
    }
    bool Eval::hasTimeLeft() const {
        std::chrono::duration<double> diff = std::chrono::system_clock::now()-last_time;
        return (diff.count()) < allotted_time;
    }

    Move Eval::megamaxRoot(int depth,Color color) {
        int alpha = -infinity, beta = infinity, best = -infinity;
        Move to_return;
        std::vector<Move> moves = movegen.getAllMoves();
        setRating(moves);
        std::sort(moves.begin(), moves.end(), compare);
        for (const Move &move:moves) {
            if(!hasTimeLeft()){
                premature_stop=true;
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
        if(!premature_stop)//don't add this node in the transposition table because it was not entirely evaluated
        TranspositionTable::getInstance().addEntry(Transposition(NodeType::Exact,internal_board.getGameState().zobrist_key,depth,best,to_return));
        return to_return;
    }

    //see https://en.wikipedia.org/wiki/Negamax with transposition tables
    int Eval::megamax(int depth, int alpha, int beta, Color color) {

        uint64_t hash=internal_board.getGameState().zobrist_key;
        int alphaOrig=alpha;
        Transposition node=TranspositionTable::getInstance().getTransposition(hash);
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
            return getHeuristicScore(color);
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
            if(!hasTimeLeft()){
                premature_stop=true;
                break;
            }
            internal_board.makeMove(move);
            int down = -megamax(depth - 1, -beta, -alpha, getOpposite(color));
            internal_board.undoLastMove();
            if(down>best){
                best=down;
                best_move=move;
            }
            alpha = std::max(alpha, best);
            if (alpha >= beta)
                break;
        }

        if(!premature_stop) {
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