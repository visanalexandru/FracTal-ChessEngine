
#ifndef BITENGINE_MOVEGEN_H
#define BITENGINE_MOVEGEN_H

#include "Board.h"
#include "Utils.h"
#include <vector>

namespace BitEngine {
    class MoveGen {
    private:
        Board &board;

        void addCapture(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves);

        void addQuiet(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves);

        void addPromotions(uint64_t origin, uint64_t dest, Color color, PieceType taken, std::vector<Move> &moves);

        void addEnPassant(uint64_t origin, uint64_t dest, Color color, std::vector<Move> &moves);

        void addDoublePawnPushMove(uint64_t origin, uint64_t dest, PieceType pawn_type, std::vector<Move> &moves);


        //functions that get the attack bitboards
        //pawn attacks need the attacking color
        uint64_t getPawnAttacks(uint64_t position, uint64_t same_side,Color color);

        uint64_t getKingAttacks(uint64_t position, uint64_t same_side);

        uint64_t getKnightAttacks(uint64_t position,uint64_t same_side);


        //Adds all the pawn moves for white
        void addWhitePawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves);

        //Adds all the pawn moves for black
        void addBlackPawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves);

        //Adds all pawn moves
        void addAllPawnMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        //adds all attacks for not-pawn piece
        void addAllAttacks(uint64_t origin, uint64_t attacks, uint64_t opposite_side, PieceType piece_type,
                           std::vector<Move> &moves);


        void addAllKingMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        void addAllKnightMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);


    public:
        MoveGen(Board &internal_board);

        std::vector<Move> getAllMoves();

    };
}


#endif
