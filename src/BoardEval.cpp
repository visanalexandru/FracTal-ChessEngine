#include "BoardEval.h"


namespace Engine {

    BoardEval::BoardEval(Engine::Board &board) : internal_board(board) {

    }

    int BoardEval::getPhase() const {
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


    int BoardEval::getMaterialScore(Color color) const {
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

    int BoardEval::getBonusPieceScore(PieceType piece, Color color,int phase) const {
        int opening= 0,ending=0, pos;
        int piece_index = static_cast<int>(piece);
        uint64_t positions = internal_board.getBitboard(getPiece(piece, color));
        while (positions) {
            pos = bitScanForward(popLsb(positions));
            if (color == White){
                opening+= Tables::Bonus[0][piece_index][63 - pos];
                ending+= Tables::Bonus[1][piece_index][63 - pos];
            }
            else {
                opening+= Tables::Bonus[0][piece_index][pos];
                ending+= Tables::Bonus[1][piece_index][pos];
            }
        }
        return interpolate(opening,ending,phase);
    }

    int BoardEval::interpolate(int opening, int ending, int phase) const {
        return  ((opening* (256 - phase)) + (ending * phase)) / 256;
    }

    int BoardEval::getDoubledPawnCount(Color color) const {
        Piece pawn = getPiece(PieceType::Pawn, color);
        uint64_t pawn_bitboard = internal_board.getBitboard(pawn);
        int result = 0;

        for (int i = 0; i < 8; i++) {
            uint64_t pawns_on_file = pawn_bitboard & Tables::MaskFile[i];
            if (pawns_on_file) {
                result += popCount(pawns_on_file) - 1;
            }

        }
        return result;
    }
    int BoardEval::getPassedPawnCount(Color color) const {
        Piece pawn = getPiece(PieceType::Pawn, color);
        Piece opposite_pawn=getPiece(PieceType::Pawn,getOpposite(color));
        int result=0;
        uint64_t pawn_bitboard = internal_board.getBitboard(pawn);
        uint64_t opposite_pawn_bitboard=internal_board.getBitboard(opposite_pawn);
        uint64_t front_fill,pawn_pos;
        while(pawn_bitboard){
            pawn_pos=popLsb(pawn_bitboard);
            int position=bitScanForward(pawn_pos);
            front_fill=Tables::PawnFrontFill[color][position];
            if((front_fill&opposite_pawn_bitboard)==0)
                result++;
        }
        return result;
    }

    int BoardEval::getPawnStructureScore(Color color,int phase) const {
        int opening=0,ending=0;
        int num_doubled_pawns=getDoubledPawnCount(color);
        int num_passed_pawns=getPassedPawnCount(color);

        opening+=num_doubled_pawns*doubled_pawn_penalty[0];
        ending+=num_doubled_pawns*doubled_pawn_penalty[1];

        opening+=num_passed_pawns*passed_pawn_bonus[0];
        ending+=num_passed_pawns*passed_pawn_bonus[1];

        return interpolate(opening,ending,phase);
    }

    int BoardEval::getBonusScore(Color color, int phase) const {
        return getBonusPieceScore(PieceType::Pawn, color,phase) + getBonusPieceScore(PieceType::Knight, color,phase) +
               getBonusPieceScore(PieceType::Bishop, color,phase) + getBonusPieceScore(PieceType::Rook, color,phase) +
               getBonusPieceScore(PieceType::Queen, color,phase) + getBonusPieceScore(PieceType::King,color,phase);
    }

    int BoardEval::getScore() const {
        int phase = getPhase();
        return getMaterialScore(White) - getMaterialScore(Black)
               + getBonusScore(White, phase) - getBonusScore(Black, phase)
               + getPawnStructureScore(White,phase)-getPawnStructureScore(Black,phase);
    }

}