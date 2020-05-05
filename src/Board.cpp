
#include "Board.h"

namespace BitEngine {

    Board::Board() : bitboards() {
        loadFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    uint64_t Board::getPieces(Color color) const {
        uint64_t to_return = 0;
        for (int i = 0; i < 6; i++)
            to_return |= bitboards[2 * i + color];
        return to_return;
    }


    uint64_t Board::getAll() const {
        return getPieces(White) | getPieces(Black);
    }

    std::string Board::toString() const {
        std::string to_return;
        for (int i = 7; i >= 0; i--) {
            for (int k = 0; k < 33; k++)
                to_return += '#';
            to_return += '\n';
            to_return += "# ";

            for (int k = 7; k >= 0; k--) {
                Piece piece = getPieceAt(1LL << (i * 8 + k));
                to_return += getPieceChar(piece);
                to_return += " # ";
            }
            to_return += '\n';
        }
        for (int k = 0; k < 33; k++)
            to_return += '#';

        to_return += '\n';
        return to_return;
    }

    uint64_t Board::getBitboard(Piece type) {
        return bitboards[type];
    }


    Piece Board::getPieceAt(uint64_t position) const {
        for (int i = 0; i < 12; i++) {
            if (bitboards[i] & position) {
                return static_cast<Piece >(i);
            }
        }
        return Piece::None;
    }

    void Board::removePieceAt(uint64_t position, Piece piece) {
        bitboards[piece] &= ~position;
    }

    void Board::setPieceAt(uint64_t position, Piece piece) {
        bitboards[piece] |= position;
    }

    Color Board::getTurn() const {
        return static_cast<Color>(gamestate.getState(turnColor));
    }

    void Board::makeNormalMove(const BitEngine::Move &move) {
        uint64_t org = move.getOrigin();


        if (move.getTaken() != None)
            removePieceAt(move.getDestination(), move.getTaken());
        setPieceAt(move.getDestination(), move.getMoved());
        removePieceAt(move.getOrigin(), move.getMoved());
    }

    void Board::makePromotion(const BitEngine::Move &move) {
        uint64_t org = move.getOrigin();
        if (move.getTaken() != None)
            removePieceAt(move.getDestination(), move.getTaken());
        setPieceAt(move.getDestination(), move.getPromotion());
        removePieceAt(move.getOrigin(), move.getMoved());
    }

    void Board::makeEnPassant(const BitEngine::Move &move) {
        uint64_t org = move.getOrigin();
        uint64_t dest = move.getDestination();

        setPieceAt(move.getDestination(), move.getMoved());
        removePieceAt(move.getOrigin(), move.getMoved());

        if (dest == org << 9 || dest == org >> 7) {
            removePieceAt(org << 1, move.getTaken());
        } else {
            removePieceAt(org >> 1, move.getTaken());
        }
    }

    void Board::makeKingSideCastle() {
        if (getTurn() == White) {
            removePieceAt(WKingPosition, WKing);
            removePieceAt(WRookRPosition, WRook);

            setPieceAt(WKingPosition >> 2, Piece::WKing);
            setPieceAt(WKingPosition >> 1, Piece::WRook);

        } else {
            removePieceAt(BKingPosition, BKing);
            removePieceAt(BRookRPosition, BRook);

            setPieceAt(BKingPosition >> 2, Piece::BKing);
            setPieceAt(BKingPosition >> 1, Piece::BRook);

        }
    }

    void Board::makeQueenSideCastle() {

        if (getTurn() == White) {
            removePieceAt(WKingPosition, WKing);
            removePieceAt(WRookLPosition, WRook);

            setPieceAt(WKingPosition << 2, Piece::WKing);
            setPieceAt(WKingPosition << 1, Piece::WRook);

        } else {
            removePieceAt(BKingPosition, BKing);
            removePieceAt(BRookLPosition, BRook);

            setPieceAt(BKingPosition << 2, Piece::BKing);
            setPieceAt(BKingPosition << 1, Piece::BRook);
        }
    }

    void Board::updateCastlingRights(const Move &move) {
        uint64_t origin = move.getOrigin();
        uint64_t destination = move.getDestination();

        if (origin == WKingPosition) {
            gamestate.unsetState(canCastleKingSideWhite);
            gamestate.unsetState(canCastleQueenSideWhite);
        }
        if (origin == BKingPosition) {
            gamestate.unsetState(canCastleKingSideBlack);
            gamestate.unsetState(canCastleQueenSideBlack);
        }
        if (origin == WRookLPosition || destination == WRookLPosition)
            gamestate.unsetState(canCastleQueenSideWhite);
        if (origin == WRookRPosition || destination == WRookRPosition)
            gamestate.unsetState(canCastleKingSideWhite);
        if (origin == BRookLPosition || destination == BRookLPosition)
            gamestate.unsetState(canCastleQueenSideBlack);
        if (origin == BRookRPosition || destination == BRookRPosition)
            gamestate.unsetState(canCastleKingSideBlack);
    }


    void Board::makeMove(const Move &move) {
        history.push(gamestate);
        MoveType type = move.getType();
        if (type == MoveType::Normal || type == MoveType::DoublePawnPush)
            makeNormalMove(move);
        else if (type == MoveType::Promote)
            makePromotion(move);
        else if (type == MoveType::EnPassant)
            makeEnPassant(move);
        else if (type == MoveType::KingSideCastle)
            makeKingSideCastle();
        else if (type == MoveType::QueenSideCastle)
            makeQueenSideCastle();
        updateCastlingRights(move);
        gamestate.toggleState(turnColor);
        gamestate.setLastMove(move);

    }

    void Board::undoNormalMove(const Move &move) {

        uint64_t dest = move.getDestination();
        uint64_t org = move.getOrigin();
        removePieceAt(dest, move.getMoved());
        if (move.getTaken() != Piece::None) {
            setPieceAt(dest, move.getTaken());
        }
        setPieceAt(move.getOrigin(), move.getMoved());
    }

    void Board::undoPromotion(const BitEngine::Move &move) {
        removePieceAt(move.getDestination(), move.getPromotion());
        if (move.getTaken() != Piece::None)
            setPieceAt(move.getDestination(), move.getTaken());
        setPieceAt(move.getOrigin(), move.getMoved());
    }

    void Board::undoEnPassant(const BitEngine::Move &move) {
        uint64_t dest = move.getDestination();
        uint64_t org = move.getOrigin();
        removePieceAt(dest, move.getMoved());

        if (dest == org << 9 || dest == org >> 7) {
            setPieceAt(org << 1, move.getTaken());
        } else {
            setPieceAt(org >> 1, move.getTaken());
        }

        setPieceAt(org, move.getMoved());
    }

    void Board::undoKingSideCastle() {
        if (getOpposite(getTurn()) == White) {
            removePieceAt(WRookRPosition << 1, WKing);
            removePieceAt(WRookRPosition << 2, WRook);

            setPieceAt(WKingPosition, WKing);
            setPieceAt(WRookRPosition, WRook);
        } else {
            removePieceAt(BRookRPosition << 1, BKing);
            removePieceAt(BRookRPosition << 2, BRook);
            setPieceAt(BKingPosition, BKing);
            setPieceAt(BRookRPosition, BRook);
        }
    }

    void Board::undoQueenSideCastle() {
        if (getOpposite(getTurn()) == White) {
            removePieceAt(WRookLPosition >> 2, WKing);
            removePieceAt(WRookLPosition >> 3, WRook);

            setPieceAt(WKingPosition, WKing);
            setPieceAt(WRookLPosition, WRook);
        } else {
            removePieceAt(BRookLPosition >> 2, BKing);
            removePieceAt(BRookLPosition >> 3, BRook);
            setPieceAt(BKingPosition, BKing);
            setPieceAt(BRookLPosition, BRook);
        }

    }

    void Board::undoLastMove() {
        Move last_move = gamestate.getLastMove();
        MoveType type = last_move.getType();
        if (type == MoveType::Normal || type == MoveType::DoublePawnPush) {
            undoNormalMove(last_move);
        } else if (type == MoveType::EnPassant) {
            undoEnPassant(last_move);
        } else if (type == MoveType::Promote) {
            undoPromotion(last_move);
        } else if (type == MoveType::KingSideCastle) {
            undoKingSideCastle();
        } else if (type == MoveType::QueenSideCastle) {
            undoQueenSideCastle();
        }

        gamestate = history.top();
        history.pop();
    }

    void Board::resetBoard() {
        for (int i = 0; i < 12; i++)
            bitboards[i] = 0;
        gamestate.reset();
    }


    void Board::loadFen(const std::string &fen) {
        resetBoard();
        std::stringstream sstream;
        sstream << fen;

        std::string aux;
        sstream >> aux;

        int cursor = 63;
        for (char a:aux) {
            if (isdigit(a)) {
                cursor -= (int) (a - '0');
            } else if (a != '/') {
                uint64_t position = 1LL << cursor;

                switch (a) {
                    case 'p':
                        setPieceAt(position, BPawn);
                        break;
                    case 'q':
                        setPieceAt(position, BQueen);
                        break;
                    case 'n':
                        setPieceAt(position, BKnight);
                        break;
                    case 'k':
                        setPieceAt(position, BKing);
                        break;
                    case 'b':
                        setPieceAt(position, BBishop);
                        break;
                    case 'r':
                        setPieceAt(position, BRook);
                        break;


                    case 'P':
                        setPieceAt(position, WPawn);
                        break;
                    case 'Q':
                        setPieceAt(position, WQueen);
                        break;
                    case 'N':
                        setPieceAt(position, WKnight);
                        break;
                    case 'K':
                        setPieceAt(position, WKing);
                        break;
                    case 'B':
                        setPieceAt(position, WBishop);
                        break;
                    case 'R':
                        setPieceAt(position, WRook);
                        break;
                }
                cursor--;
            }
        }

        GameState new_game_state;
        sstream >> aux;
        if (aux == "b")
            new_game_state.toggleState(turnColor);

        sstream >> aux;
        for (char a:aux) {
            if (a == 'K')
                new_game_state.setState(canCastleKingSideWhite);
            else if (a == 'k')
                new_game_state.setState(canCastleKingSideBlack);
            else if (a == 'Q')
                new_game_state.setState(canCastleQueenSideWhite);
            else if (a == 'q')
                new_game_state.setState(canCastleQueenSideBlack);
        }
        gamestate = new_game_state;
    }
}