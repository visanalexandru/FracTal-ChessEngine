#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <cstdint>
#include "Move.h"
namespace Engine{
	class GameState{

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
