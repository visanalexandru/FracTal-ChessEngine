#ifndef BITENGINE_GAMESTATE_H
#define BITENGINE_GAMESTATE_H

#include <cstdint>
#include "Move.h"

namespace BitEngine {
    class GameState {

    private:
        uint16_t state_flags;
        Move lastmove;
    public:

        bool getState(State state) const;

        void setState(State state);

        void unsetState(State state);

        void toggleState(State state);

        Move getLastMove() const;

        void setLastMove(Move move);

        GameState();

    };
}

#endif
