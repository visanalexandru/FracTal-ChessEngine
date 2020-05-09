#ifndef CHESSENGINE_GAMESTATE_H
#define CHESSENGINE_GAMESTATE_H

#include <cstdint>
#include "Move.h"

namespace Engine {
    class GameState {

    private:
        uint16_t state_flags;
    public:
        uint64_t zobrist_key;
        Move lastmove;
        bool getState(State state) const;

        void setState(State state);

        void unsetState(State state);

        void toggleState(State state);

        void reset();

        GameState();

    };
}

#endif
