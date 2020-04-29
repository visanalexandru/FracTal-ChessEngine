#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "Board.h"

namespace Engine {
    class MoveGen {
    private:
        Board &board;

        //These do not check if the move is legal,just provide
        //a way to easily create moves assuming the move is legal
        Move createNormal(Position a, Position b) const;

        Move createPromotion(Position a, Position b, uint8_t promote_to) const;

        Move createDoublePawnPush(Position a, Position b) const;

        Move createEnPassant(Position a, Position b) const;

        Move createQueenSideCastle() const;

        Move createKingSideCastle() const;

        void generatePawnMoves(Position a, std::vector<Move> &moves) const;

        void generateKingMoves(Position a, std::vector<Move> &moves) const;

        void generateKnightMoves(Position a, std::vector<Move> &moves) const;

        void generateBishopMoves(Position a, std::vector<Move> &moves) const;

        void generateRookMoves(Position a, std::vector<Move> &moves) const;

        void generateQueenMoves(Position a, std::vector<Move> &moves) const;


    public:
        MoveGen(Board &internal_board);

        std::vector<Move> getAllMoves() const;

    };
}


#endif
