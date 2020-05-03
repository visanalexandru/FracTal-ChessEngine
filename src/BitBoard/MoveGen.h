
#ifndef BITENGINE_MOVEGEN_H
#define BITENGINE_MOVEGEN_H

#include "Board.h"
#include "Utils.h"
#include <vector>

namespace BitEngine {
    class MoveGen {
    private:
        Board &board;

        //the capture move queries the bitboards to see what piece is at dest
        void addCapture(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves);

        //doesn't query the bitboard
        void addQuiet(uint64_t origin, uint64_t dest, PieceType to_move, std::vector<Move> &moves);

        void addPromotions(uint64_t origin, uint64_t dest, Color color, PieceType taken, std::vector<Move> &moves);

        void addEnPassant(uint64_t origin, uint64_t dest, Color color, std::vector<Move> &moves);

        void addDoublePawnPushMove(uint64_t origin, uint64_t dest, PieceType pawn_type, std::vector<Move> &moves);


        //functions that get the attack bitboards from a given position
        //pawn attacks need the attacking color
        //passing a position=0 is undefined !!!
        uint64_t getPawnAttacks(uint64_t position, uint64_t same_side, Color color);

        uint64_t getKingAttacks(uint64_t position, uint64_t same_side);

        uint64_t getKnightAttacks(uint64_t position, uint64_t same_side);

        //a and b require forward bit scan,c and d require reverse bit scan
        uint64_t getDiagonalAttacks(uint64_t position, uint64_t same_side, uint64_t all, Tables::Direction a,
                                    Tables::Direction b, Tables::Direction c, Tables::Direction d);

        //rook and bishop attacks require all the pieces for blocking
        uint64_t getRookAttacks(uint64_t position, uint64_t same_side, uint64_t all);


        uint64_t getBishopAttacks(uint64_t position, uint64_t same_side, uint64_t all);

        uint64_t getQueenAttacks(uint64_t position, uint64_t same_side, uint64_t all);

        uint64_t getAllRookAttacks(uint64_t positions, uint64_t same_side, uint64_t all);

        uint64_t getAllBishopAttacks(uint64_t positions, uint64_t same_side, uint64_t all);

        uint64_t getAllQueenAttacks(uint64_t positions,uint64_t same_side,uint64_t all);

        uint64_t getAllAttacks(uint64_t white_pieces, uint64_t black_pieces, Color color);

        bool squareUnderAttack(uint64_t square,uint64_t white_pieces,uint64_t black_pieces,Color turn);


        //Adds all the pawn moves for white
        void addWhitePawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves);

        //Adds all the pawn moves for black
        void addBlackPawnsMoves(uint64_t white_pieces, uint64_t black_pieces, std::vector<Move> &moves);

        //Adds all pawn moves
        void addAllPawnMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        //adds all attacks for non-pawn piece
        void addAllAttacks(uint64_t origin, uint64_t attacks, uint64_t opposite_side, PieceType piece_type,
                           std::vector<Move> &moves);


        void addAllKingMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        void addAllKnightMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        void addAllRookMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        void addAllBishopMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);

        void addAllQueenMoves(uint64_t white_pieces, uint64_t black_pieces, Color color, std::vector<Move> &moves);


    public:
        MoveGen(Board &internal_board);

        std::vector<Move> getAllMoves();

    };
}


#endif
