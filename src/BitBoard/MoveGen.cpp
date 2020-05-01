#include "MoveGen.h"

namespace BitEngine {

    MoveGen::MoveGen(Board &internal_board) : board(internal_board) {

    }


    void MoveGen::addQuiet(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves) {

        moves.push_back(Move(MoveType::Normal, origin, dest, to_move, PieceType::None, PieceType::None));
    }


    void
    MoveGen::addDoublePawnPushMove(uint64_t origin, uint64_t dest, PieceType pawn_type, std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::DoublePawnPush, origin, dest, pawn_type, PieceType::None,
                             PieceType::None));
    }

    void MoveGen::addEnPassant(uint64_t origin, uint64_t dest, BitEngine::Color color, std::vector<Move> &moves) {
        if (color == White) {
            moves.push_back(Move(MoveType::EnPassant, origin, dest, WPawn, BPawn, PieceType::None));
        } else {
            moves.push_back(Move(MoveType::EnPassant, origin, dest, BPawn, WPawn, PieceType::None));
        }
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

    uint64_t MoveGen::getPawnAttacks(uint64_t position, uint64_t white_pieces, uint64_t black_pieces, Color color) {
        uint64_t left_attack, right_attack, attacks;
        if (color == White) {
            right_attack = (position & Tables::ClearFile[7]) << 7;
            left_attack = (position & Tables::ClearFile[0]) << 9;
            return (left_attack | right_attack) & (~white_pieces);
        } else {
            right_attack = (position & Tables::ClearFile[0]) >> 7;
            left_attack = (position & Tables::ClearFile[7]) >> 9;
            return (left_attack | right_attack) & (~black_pieces);
        }
    }


    void MoveGen::addWhitePawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves) {
        uint64_t white_pawns = board.bitboards[PieceType::WPawn];
        uint64_t all = white_pieces | black_pieces;
        uint64_t square, single_push, double_push;
        while (white_pawns) {
            square = popLsb(white_pawns);

            single_push = (square << 8) & (~all);
            double_push = ((single_push & Tables::MaskRank[2]) << 8) & (~all);

            if (single_push) {
                if ((single_push & Tables::MaskRank[7]) == 0)
                    addQuiet(square, single_push, WPawn, moves);
                else addPromotions(square, single_push, White, PieceType::None, moves);
            }

            if (double_push)
                addDoublePawnPushMove(square, double_push, WPawn, moves);

            uint64_t attacks = getPawnAttacks(square, white_pieces, black_pieces, White);

            while (attacks) {
                uint64_t attack = popLsb(attacks);
                if (attack & black_pieces) {
                    if ((attack & Tables::MaskRank[7]) == 0)
                        addCapture(square, attack, WPawn, moves);
                    else addPromotions(square, attack, White, board.getPieceAt(attack), moves);
                } else {
                    uint64_t dest = board.gamestate.getLastMove().getDestination();
                    MoveType type = board.gamestate.getLastMove().getType();

                    if (type == MoveType::DoublePawnPush && (dest == square << 1 || dest == square >> 1) &&
                        attack == dest << 8)
                        addEnPassant(square, attack, White, moves);
                }
            }
        }
    }

    void MoveGen::addBlackPawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves) {
        uint64_t black_pawns = board.bitboards[PieceType::BPawn];
        uint64_t all = white_pieces | black_pieces;
        uint64_t square, single_push, double_push;
        while (black_pawns) {
            square = popLsb(black_pawns);
            single_push = (square >> 8) & (~all);
            double_push = ((single_push & Tables::MaskRank[5]) >> 8) & (~all);
            if (single_push) {
                if ((single_push & Tables::MaskRank[0]) == 0)
                    addQuiet(square, single_push, BPawn, moves);
                else addPromotions(square, single_push, Black, PieceType::None, moves);
            }

            if (double_push)
                addDoublePawnPushMove(square, double_push, BPawn, moves);

            uint64_t attacks = getPawnAttacks(square, white_pieces, black_pieces, Black);
            while (attacks) {
                uint64_t attack = popLsb(attacks);
                if (attack & white_pieces) {
                    if ((attack & Tables::MaskRank[0]) == 0)
                        addCapture(square, attack, BPawn, moves);
                    else addPromotions(square, attack, Black, board.getPieceAt(attack), moves);
                }
                else {
                    uint64_t dest = board.gamestate.getLastMove().getDestination();
                    MoveType type = board.gamestate.getLastMove().getType();

                    if (type == MoveType::DoublePawnPush && (dest == square << 1 || dest == square >> 1)
                        && attack == dest >> 8)
                        addEnPassant(square, attack, Black, moves);
                }
            }
        }
    }

    std::vector<Move> MoveGen::getAllMoves() {
        std::vector<Move> to_return;
        Color turn = board.getTurn();

        uint64_t black = board.getBlackPieces();
        uint64_t white = board.getWhitePieces();

        Color color = board.getTurn();

        addAllKingMoves(white, black, color, to_return);
        addAllPawnMoves(white, black, color, to_return);

        return to_return;
    }

    void
    MoveGen::addAllPawnMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves) {
        if (color == White)
            addWhitePawnsMoves(white_pieces, black_pieces, moves);
        else
            addBlackPawnsMoves(white_pieces, black_pieces, moves);
    }

    void MoveGen::addAllKingMoves(uint64_t white_pieces, uint64_t black_pieces, Color color,
                                  std::vector<Move> &moves) {
        uint64_t king_square, same_side, opposite_side;
        PieceType king_type;
        if (color == White) {
            king_type = WKing;
            king_square = board.bitboards[WKing];
            same_side = white_pieces;
            opposite_side = black_pieces;
        } else {
            king_type = BKing;
            king_square = board.bitboards[BKing];
            same_side = black_pieces;
            opposite_side = white_pieces;
        }
        uint64_t clip1 = king_square & Tables::ClearFile[0], clip2 = king_square & Tables::ClearFile[7];
        uint64_t spot1 = clip1 << 9, spot2 = king_square << 8, spot3 = clip2 << 7, spot4 = clip1 << 1,
                spot5 = clip2 >> 1, spot6 = clip1 >> 7, spot7 = king_square >> 8, spot8 = clip2 >> 9;
        uint64_t king_valid = (spot1 | spot2 | spot3 | spot4 | spot5 | spot6 | spot7 | spot8) & (~same_side);

        while (king_valid) {
            uint64_t square = popLsb(king_valid);
            if (square & opposite_side) {
                addCapture(king_square, square, king_type, moves);
            } else {
                addQuiet(king_square, square, king_type, moves);
            }
        }
    }
}