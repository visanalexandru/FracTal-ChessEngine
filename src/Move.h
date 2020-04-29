#ifndef MOVE_H
#define MOVE_H

#include "Position.h"
#include "Types.h"
#include<string>
#include<cstdint>

namespace Engine {


    class Move {
    private:
        Position origin;
        Position destination;
        MoveType move_type;
        uint8_t taken;
        uint8_t promote_to;
        uint8_t moved;
    public:
        Move(MoveType type, Position start, Position end, uint8_t piece_moved, uint8_t piece_taken, uint8_t promotion);

        Move();

        Position getOrigin() const;

        Position getDestination() const;

        MoveType getType() const;

        uint8_t getTaken() const;

        uint8_t getPromotion() const;

        uint8_t getMoved() const;

        std::string toString() const;
    };
};

#endif
