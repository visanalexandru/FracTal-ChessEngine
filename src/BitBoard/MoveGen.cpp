#include "MoveGen.h"

namespace BitEngine {

    MoveGen::MoveGen(Board &internal_board) : board(internal_board) {

    }


    void MoveGen::addQuiet(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves) {

        moves.push_back(Move(MoveType::Normal, origin, dest, to_move, PieceType::None, PieceType::None));
    }


    void MoveGen::addDoublePawnPushMove(uint64_t origin, uint64_t dest, PieceType pawn_type, std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::DoublePawnPush, origin, dest, pawn_type, PieceType::None,
                             PieceType::None));
    }

    void
    MoveGen::addPromotions(uint64_t origin, uint64_t dest, Color color, PieceType taken, std::vector<Move> &moves) {
        if (color == White) {
            moves.push_back(Move(MoveType::Promote, origin, dest, WPawn, taken, WQueen));
            moves.push_back(Move(MoveType::Promote, origin, dest, WPawn, taken, WRook));
            moves.push_back(Move(MoveType::Promote, origin, dest, WPawn, taken, WBishop));
            moves.push_back(Move(MoveType::Promote, origin, dest, WPawn, taken, WKnight));
        } else {
            moves.push_back(Move(MoveType::Promote, origin, dest, BPawn, taken, BQueen));
            moves.push_back(Move(MoveType::Promote, origin, dest, BPawn, taken, BRook));
            moves.push_back(Move(MoveType::Promote, origin, dest, BPawn, taken, BBishop));
            moves.push_back(Move(MoveType::Promote, origin, dest, BPawn, taken, BKnight));
        }
    }

    void MoveGen::addCapture(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::Normal, origin, dest, to_move, board.getPieceAt(dest), PieceType::None));
    }


    void MoveGen::addWhitePawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves) {
        uint64_t white_pawns = board.bitboards[PieceType::WPawn];
        uint64_t all = white_pieces | black_pieces;
        uint64_t square, single_push, double_push, right_attack, left_attack;
        while (white_pawns) {
            square = popLsb(white_pawns);

            single_push = (square << 8) & (~all);
            double_push = ((single_push & Tables::MaskRank[2]) << 8) & (~all);
            right_attack = (square & Tables::ClearFile[7]) << 7;
            left_attack = (square & Tables::ClearFile[0]) << 9;

            if (single_push) {
                if ((single_push & Tables::MaskRank[7]) == 0)
                    addQuiet(square, single_push, WPawn, moves);
                else addPromotions(square, single_push, White, PieceType::None, moves);
            }

            if (double_push)
                addDoublePawnPushMove(square, double_push, WPawn, moves);

            uint64_t attacks = left_attack | right_attack;
            while (attacks) {
                uint64_t attack = popLsb(attacks);
                if (attack & black_pieces) {
                    if ((attack & Tables::MaskRank[7]) == 0)
                        addCapture(square, attack, WPawn, moves);
                    else addPromotions(square, attack, White, board.getPieceAt(attack), moves);
                }
            }
        }
    }

    void MoveGen::addBlackPawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves) {
        uint64_t black_pawns = board.bitboards[PieceType::BPawn];
        uint64_t all = white_pieces | black_pieces;
        uint64_t square, single_push, double_push, right_attack, left_attack;
        while (black_pawns) {
            square = popLsb(black_pawns);
            single_push = (square >> 8) & (~all);
            double_push = ((single_push & Tables::MaskRank[5]) >> 8) & (~all);
            right_attack = (square & Tables::ClearFile[0]) >> 7;
            left_attack = (square & Tables::ClearFile[7]) >> 9;

            if (single_push) {
                if ((single_push & Tables::MaskRank[0]) == 0)
                    addQuiet(square, single_push, BPawn, moves);
                else addPromotions(square, single_push, Black, PieceType::None, moves);
            }

            if (double_push)
                addDoublePawnPushMove(square, double_push, BPawn, moves);

            uint64_t attacks = left_attack | right_attack;
            while (attacks) {
                uint64_t attack = popLsb(attacks);
                if (attack & white_pieces) {
                    if ((attack & Tables::MaskRank[0]) == 0)
                        addCapture(square, attack, BPawn, moves);
                    else addPromotions(square, attack, Black, board.getPieceAt(attack), moves);
                }
            }
        }
    }

    std::vector<Move> MoveGen::getAllMoves() {
        std::vector<Move> to_return;
        Color turn = board.getTurn();

        uint64_t black = board.getBlackPieces();
        uint64_t white = board.getWhitePieces();

        Color  color=board.getTurn();

        addAllPawnMoves(white, black, color,to_return);

        return to_return;
    }

    void MoveGen::addAllPawnMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves) {
        if (color == White)
            addWhitePawnsMoves(white_pieces, black_pieces, moves);
        else
            addBlackPawnsMoves(white_pieces, black_pieces, moves);
    }


}