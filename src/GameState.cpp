#include "GameState.h"


namespace Engine {

    GameState::GameState() : state_flags(0) {

    }

    bool GameState::getState(State state) const {
        return state_flags & state;
    }

    void GameState::setState(State state) {
        state_flags |= state;
    }

    void GameState::unsetState(State state) {
        state_flags &= (~state);
    }

    void GameState::toggleState(State state) {
        state_flags ^= state;
    }

    void GameState::setLastMove(Move move) {
        lastmove = move;
    }

    Move GameState::getLastMove() const {
        return lastmove;
    }

}
