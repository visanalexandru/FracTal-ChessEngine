
#ifndef BITENGINE_MOVE_H
#define BITENGINE_MOVE_H

#include <cstdint>
#include <string>
#include <cmath>
#include "Types.h"
namespace BitEngine{


    class Move {
    private:
        uint64_t origin;
        uint64_t destination;
        MoveType move_type;
        PieceType taken;
        PieceType promote_to;
        PieceType moved;
    public:
        Move(MoveType type, uint64_t start, uint64_t end, PieceType piece_moved, PieceType piece_taken, PieceType promotion);

        Move();

        uint64_t getOrigin() const;

        uint64_t getDestination() const;

        MoveType getType() const;

        PieceType getTaken() const;

        PieceType getPromotion() const;

        PieceType getMoved() const;

        std::string toString() const;
    };



}


#endif
