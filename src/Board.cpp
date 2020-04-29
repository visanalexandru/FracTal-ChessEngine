#include "Board.h"

namespace Engine {
    Board::Board() {
        initBoard();
    }

    void Board::initBoard() {


        current_game_state.setState(canCastleQueenSideBlack);
        current_game_state.setState(canCastleKingSideBlack);
        current_game_state.setState(canCastleQueenSideWhite);
        current_game_state.setState(canCastleKingSideWhite);

        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 8; k++)
                pieces[i][k] = None;
        }

        for (int i = 0; i < 8; i++) {
            pieces[1][i] = WhitePawn;
            pieces[6][i] = BlackPawn;
        }

        pieces[0][0] = WhiteRook;
        pieces[0][1] = WhiteKnight;
        pieces[0][2] = WhiteBishop;
        pieces[0][3] = WhiteQueen;
        pieces[0][4] = WhiteKing;
        pieces[0][5] = WhiteBishop;
        pieces[0][6] = WhiteKnight;
        pieces[0][7] = WhiteRook;

        pieces[7][0] = BlackRook;
        pieces[7][1] = BlackKnight;
        pieces[7][2] = BlackBishop;
        pieces[7][3] = BlackQueen;
        pieces[7][4] = BlackKing;
        pieces[7][5] = BlackBishop;
        pieces[7][6] = BlackKnight;
        pieces[7][7] = BlackRook;

    }

    void Board::makeNormalMove(const Move &move) {

        Position org = move.getOrigin();
        setPieceAt(move.getDestination(), getPieceAt(org));
        setPieceAt(org, None);


        if (getTurn() == Color::White) {
            if (org.y == 0 && org.x == 4) {
                current_game_state.unsetState(canCastleKingSideWhite);
                current_game_state.unsetState(canCastleQueenSideWhite);
            } else if (org.y == 0 && org.x == 0) {
                current_game_state.unsetState(canCastleQueenSideWhite);
            } else if (org.y == 0 && org.x == 7) {
                current_game_state.unsetState(canCastleKingSideWhite);
            }
        } else {

            if (org.y == 7 && org.x == 4) {
                current_game_state.unsetState(canCastleKingSideBlack);
                current_game_state.unsetState(canCastleQueenSideBlack);
            } else if (org.y == 7 && org.x == 0) {
                current_game_state.unsetState(canCastleQueenSideBlack);
            } else if (org.y == 7 && org.x == 7) {
                current_game_state.unsetState(canCastleKingSideBlack);
            }

        }
    }

    void Board::undoNormalMove(const Move &move) {
        setPieceAt(move.getOrigin(), move.getMoved());
        setPieceAt(move.getDestination(), move.getTaken());
    }

    void Board::makePromotion(const Move &move) {
        setPieceAt(move.getDestination(), move.getPromotion());
        setPieceAt(move.getOrigin(), None);
    }

    void Board::makeEnPassant(const Move &move) {
        Position a = move.getOrigin();
        Position b = move.getDestination();
        setPieceAt(Position(b.x, a.y), None);
        setPieceAt(b, getPieceAt(a));
        setPieceAt(a, None);
    }

    void Board::undoEnPassant(const Move &move) {
        Position a = move.getOrigin();
        Position b = move.getDestination();
        setPieceAt(move.getOrigin(), move.getMoved());
        setPieceAt(b, None);
        setPieceAt(Position(b.x, a.y), move.getTaken());
    }

    void Board::makeKingSideCastle() {
        if (getTurn() == Color::White) {
            pieces[0][6] = WhiteKing;
            pieces[0][4] = None;
            pieces[0][5] = WhiteRook;
            pieces[0][7] = None;

            current_game_state.unsetState(canCastleKingSideWhite);
            current_game_state.unsetState(canCastleQueenSideWhite);
        } else {
            pieces[7][6] = BlackKing;
            pieces[7][4] = None;
            pieces[7][5] = BlackRook;
            pieces[7][7] = None;
            current_game_state.unsetState(canCastleKingSideBlack);
            current_game_state.unsetState(canCastleQueenSideBlack);

        }
    }

    void Board::undoKingSideCastle(const Move &move) {

        if (move.getMoved() == WhiteKing) {
            setPieceAt(Position(4, 0), WhiteKing);
            setPieceAt(Position(6, 0), None);
            setPieceAt(Position(5, 0), None);
            setPieceAt(Position(7, 0), WhiteRook);
        } else {
            setPieceAt(Position(4, 7), BlackKing);
            setPieceAt(Position(6, 7), None);
            setPieceAt(Position(5, 7), None);
            setPieceAt(Position(7, 7), BlackRook);
        }

    }

    void Board::makeQueenSideCastle() {
        if (getTurn() == Color::White) {
            pieces[0][2] = WhiteKing;
            pieces[0][4] = None;
            pieces[0][3] = WhiteRook;
            pieces[0][0] = None;
            current_game_state.unsetState(canCastleKingSideWhite);
            current_game_state.unsetState(canCastleQueenSideWhite);

        } else {
            pieces[7][2] = BlackKing;
            pieces[7][4] = None;
            pieces[7][3] = BlackRook;
            pieces[7][0] = None;
            current_game_state.unsetState(canCastleKingSideBlack);
            current_game_state.unsetState(canCastleQueenSideBlack);
        }
    }

    void Board::undoQueenSideCastle(const Move &move) {
        if (move.getMoved() == WhiteKing) {
            setPieceAt(Position(4, 0), WhiteKing);
            setPieceAt(Position(3, 0), None);
            setPieceAt(Position(2, 0), None);
            setPieceAt(Position(1, 0), None);
            setPieceAt(Position(0, 0), WhiteRook);
        } else {
            setPieceAt(Position(4, 7), BlackKing);
            setPieceAt(Position(3, 7), None);
            setPieceAt(Position(2, 7), None);
            setPieceAt(Position(1, 7), None);
            setPieceAt(Position(0, 7), BlackRook);
        }
    }

    void Board::makeMove(const Move &move) {
        history.push(current_game_state);
        MoveType type = move.getType();
        if (type == MoveType::Normal || type == MoveType::DoublePawnPush)
            makeNormalMove(move);
        else if (type == MoveType::EnPassant)
            makeEnPassant(move);
        else if (type == MoveType::KingSideCastle)
            makeKingSideCastle();
        else if (type == MoveType::QueenSideCastle)
            makeQueenSideCastle();
        else if (type == MoveType::Promote)
            makePromotion(move);

        current_game_state.toggleState(turnColor);
        current_game_state.setLastMove(move);
    }

    void Board::undoLastMove() {
        Move last_move = current_game_state.getLastMove();
        MoveType type = last_move.getType();
        if (type == MoveType::Normal || type == MoveType::DoublePawnPush || type == MoveType::Promote) {
            undoNormalMove(last_move);
        } else if (type == MoveType::EnPassant) {
            undoEnPassant(last_move);
        } else if (type == MoveType::KingSideCastle) {
            undoKingSideCastle(last_move);
        } else if (type == MoveType::QueenSideCastle) {
            undoQueenSideCastle(last_move);
        }

        current_game_state = history.top();
        history.pop();
    }


    std::string Board::print() const {
        std::string to_return;
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 33; k++)
                to_return += '#';
            to_return += '\n';
            to_return += "# ";

            for (int k = 0; k < 8; k++) {
                to_return += getChar(pieces[7 - i][k]);
                to_return += " # ";
            }
            to_return += '\n';
        }
        for (int k = 0; k < 33; k++)
            to_return += '#';

        to_return += '\n';
        return to_return;
    }


    Color Board::getTurn() const {
        return (Engine::Color) current_game_state.getState(turnColor);
    }

    GameState Board::getGamestate() const {
        return current_game_state;
    }

    void Board::setPieceAt(const Position &pos, uint8_t piece) {
        pieces[pos.y][pos.x] = piece;
    }

    uint8_t Board::getPieceAt(const Position &pos) const {
        return pieces[pos.y][pos.x];
    }
}
