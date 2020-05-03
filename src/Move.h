
#ifndef BITENGINE_MOVE_H
#define BITENGINE_MOVE_H

#include <cstdint>
#include <string>
#include <cmath>
#include "Utils.h"
#include "Types.h"
namespace BitEngine{


    class Move {
    private:
        uint64_t origin;
        uint64_t destination;
        MoveType move_type;
        Piece taken;
        Piece promote_to;
        Piece moved;
    public:
        Move(MoveType type, uint64_t start, uint64_t end, Piece piece_moved, Piece piece_taken, Piece promotion);

        Move();

        uint64_t getOrigin() const;

        uint64_t getDestination() const;

        MoveType getType() const;

        Piece getTaken() const;

        Piece getPromotion() const;

        Piece getMoved() const;

        std::string toString() const;
    };



}


#endif
