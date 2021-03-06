#include "MoveGen.h"

namespace Engine {

    MoveGen::MoveGen(Board &internal_board) : board(internal_board) {

    }


    void MoveGen::addQuiet(uint64_t origin, uint64_t dest, Piece to_move, std::vector<Move> &moves) {

        moves.push_back(Move(MoveType::Normal, origin, dest, to_move, Piece::None, Piece::None));
    }


    void
    MoveGen::addDoublePawnPushMove(uint64_t origin, uint64_t dest, Color color, std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::DoublePawnPush, origin, dest, getPiece(PieceType::Pawn, color), Piece::None,
                             Piece::None));
    }

    void MoveGen::addEnPassant(uint64_t origin, uint64_t dest, Color color, std::vector<Move> &moves) {
        Piece pawn = getPiece(PieceType::Pawn, color), taken_pawn = getPiece(PieceType::Pawn, getOpposite(color));
        moves.push_back(Move(MoveType::EnPassant, origin, dest, pawn, taken_pawn, Piece::None));
    }

    void
    MoveGen::addPromotions(uint64_t origin, uint64_t dest, Color color, Piece taken, std::vector<Move> &moves) {
        Piece pawn = getPiece(PieceType::Pawn, color);
        moves.push_back(Move(MoveType::Promote, origin, dest, pawn, taken, getPiece(PieceType::Queen, color)));
        moves.push_back(Move(MoveType::Promote, origin, dest, pawn, taken, getPiece(PieceType::Rook, color)));
        moves.push_back(Move(MoveType::Promote, origin, dest, pawn, taken, getPiece(PieceType::Bishop, color)));
        moves.push_back(Move(MoveType::Promote, origin, dest, pawn, taken, getPiece(PieceType::Knight, color)));
    }

    void MoveGen::addCapture(uint64_t origin, uint64_t dest, Piece to_move, std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::Normal, origin, dest, to_move, board.getPieceAt(dest), Piece::None));
    }

    void MoveGen::addKingSideCastle(uint64_t origin, uint64_t dest, Color color, std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::KingSideCastle, origin, dest, getPiece(PieceType::King, color), None, None));
    }

    void MoveGen::addQueenSideCastle(uint64_t origin, uint64_t dest, Engine::Color color,
                                     std::vector<Move> &moves) {
        moves.push_back(Move(MoveType::QueenSideCastle, origin, dest, getPiece(PieceType::King, color), None, None));
    }

    uint64_t MoveGen::getPawnAttacks(uint64_t position, uint64_t same_side, Color color) {
        uint64_t left_attack, right_attack;
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
                                         Engine::Tables::Direction a, Engine::Tables::Direction b,
                                         Engine::Tables::Direction c, Engine::Tables::Direction d) {
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

    uint64_t MoveGen::getQueenAttacks(uint64_t position, uint64_t same_side, uint64_t all) {
        return getRookAttacks(position, same_side, all) | getBishopAttacks(position, same_side, all);
    }

    uint64_t MoveGen::getAllRookAttacks(uint64_t positions, uint64_t same_side, uint64_t all) {
        uint64_t result = 0, square;
        while (positions) {
            square = popLsb(positions);
            result |= getRookAttacks(square, same_side, all);
        }
        return result;
    }

    uint64_t MoveGen::getAllBishopAttacks(uint64_t positions, uint64_t same_side, uint64_t all) {
        uint64_t result = 0, square;
        while (positions) {
            square = popLsb(positions);
            result |= getBishopAttacks(square, same_side, all);
        }
        return result;
    }

    uint64_t MoveGen::getAllQueenAttacks(uint64_t positions, uint64_t same_side, uint64_t all) {
        uint64_t result = 0, square;
        while (positions) {
            square = popLsb(positions);
            result |= getQueenAttacks(square, same_side, all);
        }
        return result;
    }

    bool
    MoveGen::squareUnderAttack(uint64_t square, Color color) {//if the opposite side of the color can attack the square
        Color opposite = getOpposite(color);
        uint64_t same_side = board.getPieces(color);
        uint64_t opposite_side = board.getPieces(opposite);
        uint64_t all = same_side | opposite_side;

        uint64_t opposite_bishops = board.getBitboard(getPiece(PieceType::Bishop, opposite));
        uint64_t opposite_rooks = board.getBitboard(getPiece(PieceType::Rook, opposite));
        uint64_t opposite_queens = board.getBitboard(getPiece(PieceType::Queen, opposite));
        uint64_t opposite_knights = board.getBitboard(getPiece(PieceType::Knight, opposite));
        uint64_t opposite_pawns = board.getBitboard(getPiece(PieceType::Pawn, opposite));
        uint64_t opposite_king = board.getBitboard(getPiece(PieceType::King, opposite));

        uint64_t bishops_and_queens = opposite_bishops | opposite_queens;
        uint64_t rooks_and_queens = opposite_rooks | opposite_queens;

        if (bishops_and_queens && (getBishopAttacks(square, same_side, all) & bishops_and_queens))
            return true;

        if (rooks_and_queens && (getRookAttacks(square, same_side, all) & rooks_and_queens))
            return true;

        if (opposite_knights && (getKnightAttacks(square, same_side) & opposite_knights))
            return true;
        if (opposite_pawns && (getPawnAttacks(square, same_side, color) & opposite_pawns))
            return true;
        if (getKingAttacks(square, same_side) & opposite_king)
            return true;
        return false;
    }


    uint64_t MoveGen::getAllAttacks(Color color) {
        uint64_t same_side = board.getPieces(color);
        uint64_t all = board.getAll();

        Piece queen = getPiece(PieceType::Queen, color);
        Piece bishop = getPiece(PieceType::Bishop, color);
        Piece rook = getPiece(PieceType::Rook, color);

        uint64_t queens_and_bishops = board.getBitboard(queen) | board.getBitboard(bishop);
        uint64_t queens_and_rooks = board.getBitboard(queen) | board.getBitboard(rook);

        return getPawnAttacks(board.getBitboard(getPiece(PieceType::Pawn, color)), same_side, color) |
               getKnightAttacks(board.getBitboard(getPiece(PieceType::Knight, color)), same_side) |
               getKingAttacks(board.getBitboard(getPiece(PieceType::King, color)), same_side) |
               getAllBishopAttacks(queens_and_bishops, same_side, all) |
               getAllRookAttacks(queens_and_rooks, same_side, all);
    }

    void MoveGen::addWhitePawnCaptures(uint64_t square, uint64_t same_side, uint64_t opposite_side,
                                       std::vector<Move> &moves) {
        uint64_t attacks = getPawnAttacks(square, same_side, White);
        while (attacks) {
            uint64_t attack = popLsb(attacks);
            if (attack & opposite_side) {
                if ((attack & Tables::MaskRank[7]) == 0)
                    addCapture(square, attack, WPawn, moves);
                else addPromotions(square, attack, White, board.getPieceAt(attack), moves);
            } else {
                uint64_t dest = board.gamestate.lastmove.getDestination();
                MoveType type = board.gamestate.lastmove.getType();

                if (type == MoveType::DoublePawnPush && (dest == square << 1 || dest == square >> 1) &&
                    attack == dest << 8)
                    addEnPassant(square, attack, White, moves);
            }
        }
    }

    void MoveGen::addWhitePawnsMoves(uint64_t same_side, uint64_t opposite_side, std::vector<Move> &moves) {
        uint64_t white_pawns = board.getBitboard(Piece::WPawn);
        uint64_t all = same_side | opposite_side;
        uint64_t square, single_push, double_push;
        while (white_pawns) {
            square = popLsb(white_pawns);

            single_push = (square << 8) & (~all);
            double_push = ((single_push & Tables::MaskRank[2]) << 8) & (~all);

            if (single_push) {
                if ((single_push & Tables::MaskRank[7]) == 0)
                    addQuiet(square, single_push, WPawn, moves);
                else addPromotions(square, single_push, White, Piece::None, moves);
            }

            if (double_push)
                addDoublePawnPushMove(square, double_push, White, moves);

            addWhitePawnCaptures(square, same_side, opposite_side, moves);
        }
    }

    void MoveGen::addBlackPawnCaptures(uint64_t square, uint64_t same_side, uint64_t opposite_side,
                                       std::vector<Move> &moves) {
        uint64_t attacks = getPawnAttacks(square, same_side, Black);
        while (attacks) {
            uint64_t attack = popLsb(attacks);
            if (attack & opposite_side) {
                if ((attack & Tables::MaskRank[0]) == 0)
                    addCapture(square, attack, BPawn, moves);
                else addPromotions(square, attack, Black, board.getPieceAt(attack), moves);
            } else {
                uint64_t dest = board.gamestate.lastmove.getDestination();
                MoveType type = board.gamestate.lastmove.getType();

                if (type == MoveType::DoublePawnPush && (dest == square << 1 || dest == square >> 1)
                    && attack == dest >> 8)
                    addEnPassant(square, attack, Black, moves);
            }
        }
    }

    void MoveGen::addBlackPawnsMoves(uint64_t same_side, uint64_t opposite_side, std::vector<Move> &moves) {
        uint64_t black_pawns = board.getBitboard(Piece::BPawn);
        uint64_t all = same_side | opposite_side;
        uint64_t square, single_push, double_push;
        while (black_pawns) {
            square = popLsb(black_pawns);
            single_push = (square >> 8) & (~all);
            double_push = ((single_push & Tables::MaskRank[5]) >> 8) & (~all);
            if (single_push) {
                if ((single_push & Tables::MaskRank[0]) == 0)
                    addQuiet(square, single_push, BPawn, moves);
                else addPromotions(square, single_push, Black, Piece::None, moves);
            }

            if (double_push)
                addDoublePawnPushMove(square, double_push, Black, moves);

            addBlackPawnCaptures(square, same_side, opposite_side, moves);
        }
    }

    std::vector<Move> MoveGen::getAllCaptures() {

        std::vector<Move> to_return;
        to_return.reserve(300);
        Color color = board.getTurn();
        Color opposite_color = getOpposite(color);
        uint64_t same_side = board.getPieces(color);
        uint64_t opposite_side = board.getPieces(opposite_color);
        Piece king = getPiece(PieceType::King, color);

        addAllKingCaptures(same_side, opposite_side, color, to_return);
        addAllPawnCaptures(same_side, opposite_side, color, to_return);
        addAllKnightCaptures(same_side, opposite_side, color, to_return);
        addAllRookCaptures(same_side, opposite_side, color, to_return);
        addAllBishopCaptures(same_side, opposite_side, color, to_return);
        addAllQueenCaptures(same_side, opposite_side, color, to_return);
        for (int i = 0; i < to_return.size(); i++) {
            board.makeMove(to_return[i]);
            uint64_t kingpos = board.getBitboard(king);
            if (isInCheck(color)) {
                to_return.erase(to_return.begin() + i);
                i--;
            }
            board.undoLastMove();
        }
        return to_return;
    }

    std::vector<Move> MoveGen::getAllMoves() {
        std::vector<Move> to_return;
        to_return.reserve(300);
        Color color = board.getTurn();
        Color opposite_color = getOpposite(color);
        uint64_t same_side = board.getPieces(color);
        uint64_t opposite_side = board.getPieces(opposite_color);
        Piece king = getPiece(PieceType::King, color);

        addAllKingMoves(same_side, opposite_side, color, to_return);
        addAllPawnMoves(same_side, opposite_side, color, to_return);
        addAllKnightMoves(same_side, opposite_side, color, to_return);
        addAllRookMoves(same_side, opposite_side, color, to_return);
        addAllBishopMoves(same_side, opposite_side, color, to_return);
        addAllQueenMoves(same_side, opposite_side, color, to_return);


        for (int i = 0; i < to_return.size(); i++) {
            board.makeMove(to_return[i]);
            uint64_t kingpos = board.getBitboard(king);
            if (isInCheck(color)) {
                to_return.erase(to_return.begin() + i);
                i--;
            }
            board.undoLastMove();
        }
        addAllCastling(same_side | opposite_side, color, to_return);
        return to_return;
    }

    void
    MoveGen::addAllPawnMoves(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        if (color == White)
            addWhitePawnsMoves(same_side, opposite_side, moves);
        else
            addBlackPawnsMoves(same_side, opposite_side, moves);
    }

    void MoveGen::addAllCaptures(uint64_t origin, uint64_t attacks, uint64_t opposite_side, Engine::Piece piece_type,
                                 std::vector<Move> &moves) {
        while (attacks) {
            uint64_t square = popLsb(attacks);
            if (square & opposite_side) {
                addCapture(origin, square, piece_type, moves);
            }
        }
    }

    void MoveGen::addAllPawnCaptures(uint64_t same_side, uint64_t opposite_side, Color color,
                                     std::vector<Move> &moves) {
        Piece pawn = getPiece(PieceType::Pawn, color);
        uint64_t pawns = board.getBitboard(pawn);

        while (pawns) {
            uint64_t square = popLsb(pawns);
            if (color == White)
                addWhitePawnCaptures(square, same_side, opposite_side, moves);
            else addBlackPawnCaptures(square, same_side, opposite_side, moves);
        }
    }

    void MoveGen::addAllKnightCaptures(uint64_t same_side, uint64_t opposite_side, Color color,
                                       std::vector<Move> &moves) {
        Piece knight_type = getPiece(PieceType::Knight, color);
        uint64_t knight_squares = board.getBitboard(knight_type);
        while (knight_squares) {
            uint64_t knight_square = popLsb(knight_squares);
            uint64_t knight_attacks = getKnightAttacks(knight_square, same_side);
            addAllCaptures(knight_square, knight_attacks, opposite_side, knight_type, moves);
        }
    }

    void
    MoveGen::addAllBishopCaptures(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece bishop_type = getPiece(PieceType::Bishop, color);
        uint64_t bishop_squares = board.getBitboard(bishop_type);
        while (bishop_squares) {
            uint64_t bishop_square = popLsb(bishop_squares);
            uint64_t bishop_attacks = getBishopAttacks(bishop_square, same_side, same_side | opposite_side);
            addAllCaptures(bishop_square, bishop_attacks, opposite_side, bishop_type, moves);
        }
    }

    void
    MoveGen::addAllRookCaptures(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece rook_type = getPiece(PieceType::Rook, color);
        uint64_t rook_squares = board.getBitboard(rook_type);
        while (rook_squares) {
            uint64_t rook_square = popLsb(rook_squares);
            uint64_t rook_attacks = getRookAttacks(rook_square, same_side, same_side | opposite_side);
            addAllCaptures(rook_square, rook_attacks, opposite_side, rook_type, moves);
        }
    }

    void
    MoveGen::addAllQueenCaptures(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece queen_type = getPiece(PieceType::Queen, color);
        uint64_t queen_squares = board.bitboards[queen_type];
        while (queen_squares) {
            uint64_t queen_square = popLsb(queen_squares);
            uint64_t queen_attacks = getQueenAttacks(queen_square, same_side, same_side | opposite_side);
            addAllCaptures(queen_square, queen_attacks, opposite_side, queen_type, moves);
        }
    }

    void
    MoveGen::addAllKingCaptures(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece king_type = getPiece(PieceType::King, color);
        uint64_t king_square = board.getBitboard(king_type);
        uint64_t king_attacks = getKingAttacks(king_square, same_side);
        addAllCaptures(king_square, king_attacks, opposite_side, king_type, moves);
    }

    void MoveGen::addAllAttacks(uint64_t origin, uint64_t attacks, uint64_t opposite_side,
                                Piece piece_type, std::vector<Move> &moves) {
        while (attacks) {
            uint64_t square = popLsb(attacks);
            if (square & opposite_side) {
                addCapture(origin, square, piece_type, moves);
            } else {
                addQuiet(origin, square, piece_type, moves);
            }
        }
    }

    void MoveGen::addAllKingMoves(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece king_type = getPiece(PieceType::King, color);
        uint64_t king_square = board.getBitboard(king_type);
        uint64_t king_attacks = getKingAttacks(king_square, same_side);
        addAllAttacks(king_square, king_attacks, opposite_side, king_type, moves);
    }

    void MoveGen::addAllKnightMoves(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece knight_type = getPiece(PieceType::Knight, color);
        uint64_t knight_squares = board.getBitboard(knight_type);
        while (knight_squares) {
            uint64_t knight_square = popLsb(knight_squares);
            uint64_t knight_attacks = getKnightAttacks(knight_square, same_side);
            addAllAttacks(knight_square, knight_attacks, opposite_side, knight_type, moves);
        }
    }

    void MoveGen::addAllRookMoves(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece rook_type = getPiece(PieceType::Rook, color);
        uint64_t rook_squares = board.getBitboard(rook_type);
        while (rook_squares) {
            uint64_t rook_square = popLsb(rook_squares);
            uint64_t rook_attacks = getRookAttacks(rook_square, same_side, same_side | opposite_side);
            addAllAttacks(rook_square, rook_attacks, opposite_side, rook_type, moves);
        }
    }

    void MoveGen::addAllBishopMoves(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece bishop_type = getPiece(PieceType::Bishop, color);
        uint64_t bishop_squares = board.getBitboard(bishop_type);
        while (bishop_squares) {
            uint64_t bishop_square = popLsb(bishop_squares);
            uint64_t bishop_attacks = getBishopAttacks(bishop_square, same_side, same_side | opposite_side);
            addAllAttacks(bishop_square, bishop_attacks, opposite_side, bishop_type, moves);
        }
    }

    void MoveGen::addAllQueenMoves(uint64_t same_side, uint64_t opposite_side, Color color, std::vector<Move> &moves) {
        Piece queen_type = getPiece(PieceType::Queen, color);
        uint64_t queen_squares = board.bitboards[queen_type];
        while (queen_squares) {
            uint64_t queen_square = popLsb(queen_squares);
            uint64_t queen_attacks = getQueenAttacks(queen_square, same_side, same_side | opposite_side);
            addAllAttacks(queen_square, queen_attacks, opposite_side, queen_type, moves);
        }
    }

    void MoveGen::addAllCastling(uint64_t all, Color color, std::vector<Move> &moves) {
        uint64_t attacks = getAllAttacks(getOpposite(color));
        uint64_t not_in_check = ~attacks;
        uint64_t not_occupied = ~all;
        uint64_t valid = ~(attacks | all);


        if (color == White) {
            uint64_t white_king = WKingPosition;
            uint64_t a = white_king >> 1, b = white_king >> 2, c = white_king << 1, d = white_king << 2, e =
                    white_king << 3;
            if (white_king & not_in_check) {
                if (board.gamestate.getState(canCastleKingSideWhite) && (a & valid) && (b & valid)) {
                    addKingSideCastle(white_king, b, color, moves);
                }
                if (board.gamestate.getState(canCastleQueenSideWhite) && (c & valid) && (d & valid) &&
                    (e & not_occupied)) {
                    addQueenSideCastle(white_king, d, color, moves);
                }
            }
        } else {
            uint64_t black_king = BKingPosition;
            uint64_t a = black_king >> 1, b = black_king >> 2, c = black_king << 1, d = black_king << 2, e =
                    black_king << 3;
            if (black_king & not_in_check) {
                if (board.gamestate.getState(canCastleKingSideBlack) && (a & valid) && (b & valid)) {
                    addKingSideCastle(black_king, b, color, moves);
                }
                if (board.gamestate.getState(canCastleQueenSideBlack) && (c & valid) && (d & valid) &&
                    (e & not_occupied)) {
                    addQueenSideCastle(black_king, d, color, moves);
                }
            }
        }
    }

    bool MoveGen::isInCheck(Engine::Color color) {
        uint64_t king_position = board.getBitboard(getPiece(PieceType::King, color));
        return squareUnderAttack(king_position, color);
    }

}