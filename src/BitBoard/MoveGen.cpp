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

    uint64_t MoveGen::getPawnAttacks(uint64_t position, uint64_t same_side, Color color) {
        uint64_t left_attack, right_attack, attacks;
        if (color == White) {
            right_attack = (position & Tables::ClearFile[7]) << 7;
            left_attack = (position & Tables::ClearFile[0]) << 9;
        } else {
            right_attack = (position & Tables::ClearFile[0]) >> 7;
            left_attack = (position & Tables::ClearFile[7]) >> 9;
        }
        return (left_attack | right_attack) & (~same_side);
    }

    uint64_t MoveGen::getKingAttacks(uint64_t position, uint64_t same_side) {

        uint64_t clip1 = position & Tables::ClearFile[0], clip2 = position & Tables::ClearFile[7];
        uint64_t spot1 = clip1 << 9, spot2 = position << 8, spot3 = clip2 << 7, spot4 = clip1 << 1,
                spot5 = clip2 >> 1, spot6 = clip1 >> 7, spot7 = position >> 8, spot8 = clip2 >> 9;

        uint64_t king_valid = (spot1 | spot2 | spot3 | spot4 | spot5 | spot6 | spot7 | spot8);
        return king_valid & (~same_side);
    }

    uint64_t MoveGen::getKnightAttacks(uint64_t position, uint64_t same_side) {
        uint64_t clip1 = position & Tables::ClearFile[0], clip2 = position & Tables::ClearFile[7],
                clip3 = position & Tables::ClearFile[0] & Tables::ClearFile[1],
                clip4 = position & Tables::ClearFile[6] & Tables::ClearFile[7];

        uint64_t spot1 = clip3 << 10, spot2 = clip1 << 17, spot3 = clip2 << 15, spot4 = clip4 << 6,
                spot5 = clip4 >> 10, spot6 = clip2 >> 17, spot7 = clip1 >> 15, spot8 = clip3 >> 6;
        uint64_t knight_valid = (spot1 | spot2 | spot3 | spot4 | spot5 | spot6 | spot7 | spot8);

        return knight_valid & (~same_side);
    }

    uint64_t MoveGen::getDiagonalAttacks(uint64_t position, uint64_t same_side, uint64_t all,
                                         BitEngine::Tables::Direction a, BitEngine::Tables::Direction b,
                                         BitEngine::Tables::Direction c, BitEngine::Tables::Direction d) {
        int index = bitScanForward(position), square;

        uint64_t up = Tables::AttackTables[index][a];
        uint64_t left = Tables::AttackTables[index][b];

        uint64_t down = Tables::AttackTables[index][c];
        uint64_t right = Tables::AttackTables[index][d];

        uint64_t blockerUp = up & all;
        uint64_t blockerLeft = left & all;
        uint64_t blockerRight = right & all;
        uint64_t blockerDown = down & all;

        if (blockerUp) {
            square = bitScanForward(blockerUp);
            up ^= Tables::AttackTables[square][a];
        }

        if (blockerLeft) {
            square = bitScanForward(blockerLeft);
            left ^= Tables::AttackTables[square][b];
        }
        if (blockerRight) {
            square = bitScanReverse(blockerRight);
            right ^= Tables::AttackTables[square][d];
        }

        if (blockerDown) {
            square = bitScanReverse(blockerDown);
            down ^= Tables::AttackTables[square][c];
        }
        uint64_t attacks = (up ^ left ^ right ^ down) & (~same_side);
        return attacks;

    }

    uint64_t MoveGen::getRookAttacks(uint64_t position, uint64_t same_side, uint64_t all) {
        return getDiagonalAttacks(position, same_side, all, Tables::North, Tables::West, Tables::South, Tables::East);
    }

    uint64_t MoveGen::getBishopAttacks(uint64_t position, uint64_t same_side, uint64_t all) {
        return getDiagonalAttacks(position, same_side, all, Tables::NorthWest, Tables::NorthEast, Tables::SouthEast,
                                  Tables::SouthWest);
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

            uint64_t attacks = getPawnAttacks(square, white_pieces, White);

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

            uint64_t attacks = getPawnAttacks(square, black_pieces, Black);
            while (attacks) {
                uint64_t attack = popLsb(attacks);
                if (attack & white_pieces) {
                    if ((attack & Tables::MaskRank[0]) == 0)
                        addCapture(square, attack, BPawn, moves);
                    else addPromotions(square, attack, Black, board.getPieceAt(attack), moves);
                } else {
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
        addAllKnightMoves(white, black, color, to_return);
        addAllRookMoves(white, black, color, to_return);
        addAllBishopMoves(white, black, color, to_return);

        return to_return;
    }

    void
    MoveGen::addAllPawnMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves) {
        if (color == White)
            addWhitePawnsMoves(white_pieces, black_pieces, moves);
        else
            addBlackPawnsMoves(white_pieces, black_pieces, moves);
    }

    void MoveGen::addAllAttacks(uint64_t origin, uint64_t attacks, uint64_t opposite_side,
                                PieceType piece_type, std::vector<Move> &moves) {
        while (attacks) {
            uint64_t square = popLsb(attacks);
            if (square & opposite_side) {
                addCapture(origin, square, piece_type, moves);
            } else {
                addQuiet(origin, square, piece_type, moves);
            }
        }
    }

    void MoveGen::addAllKingMoves(uint64_t white_pieces, uint64_t black_pieces, Color color,
                                  std::vector<Move> &moves) {
        uint64_t king_square, opposite_side, same_side;
        PieceType king_type;

        if (color == White) {
            king_type = WKing;
            opposite_side = black_pieces;
            same_side = white_pieces;
        } else {
            opposite_side = white_pieces;
            same_side = black_pieces;
            king_type = BKing;
        }
        king_square = board.bitboards[king_type];
        uint64_t king_attacks = getKingAttacks(king_square, same_side);
        addAllAttacks(king_square, king_attacks, opposite_side, king_type, moves);
    }

    void MoveGen::addAllKnightMoves(uint64_t white_pieces, uint64_t black_pieces, BitEngine::Color color,
                                    std::vector<Move> &moves) {
        uint64_t knight_squares, opposite_side, same_side;
        PieceType knight_type;

        if (color == White) {
            knight_type = WKnight;
            opposite_side = black_pieces;
            same_side = white_pieces;
        } else {
            opposite_side = white_pieces;
            knight_type = BKnight;
            same_side = black_pieces;
        }
        knight_squares = board.bitboards[knight_type];
        while (knight_squares) {
            uint64_t knight_square = popLsb(knight_squares);
            uint64_t knight_attacks = getKnightAttacks(knight_square, same_side);
            addAllAttacks(knight_square, knight_attacks, opposite_side, knight_type, moves);
        }

    }

    void MoveGen::addAllRookMoves(uint64_t white_pieces, uint64_t black_pieces, BitEngine::Color color,
                                  std::vector<Move> &moves) {

        uint64_t rook_squares, opposite_side, same_side;
        PieceType rook_type;

        if (color == White) {
            rook_type = WRook;
            opposite_side = black_pieces;
            same_side = white_pieces;
        } else {
            opposite_side = white_pieces;
            rook_type = BRook;
            same_side = black_pieces;
        }

        rook_squares = board.bitboards[rook_type];
        while (rook_squares) {
            uint64_t rook_square = popLsb(rook_squares);
            uint64_t rook_attacks = getRookAttacks(rook_square, same_side, white_pieces | black_pieces);
            addAllAttacks(rook_square, rook_attacks, opposite_side, rook_type, moves);
        }
    }

    void MoveGen::addAllBishopMoves(uint64_t white_pieces, uint64_t black_pieces, BitEngine::Color color,
                                    std::vector<Move> &moves) {

        uint64_t bishop_squares, opposite_side, same_side;
        PieceType bishop_type;

        if (color == White) {
            bishop_type= WBishop;
            opposite_side = black_pieces;
            same_side = white_pieces;
        } else {
            opposite_side = white_pieces;
            bishop_type = BBishop;
            same_side = black_pieces;
        }

        bishop_squares = board.bitboards[bishop_type];
        while (bishop_squares) {
            uint64_t bishop_square = popLsb(bishop_squares);
            uint64_t bishop_attacks = getBishopAttacks(bishop_square, same_side, white_pieces | black_pieces);
            board.print(bishop_attacks);
            addAllAttacks(bishop_square, bishop_attacks, opposite_side, bishop_type, moves);
        }
    }
}