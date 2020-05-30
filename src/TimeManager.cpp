#include "TimeManager.h"


namespace Engine {

    namespace TimeManager {
        float getTimePerMove(int remaining, int increment) {
            float remaining_moves = 20.f;
            float epsilon = 0.1f;//for delay
            float remaining_seconds = (float) remaining / 1000.f - epsilon;

            float ms_remaining =
                    ((float) remaining + (remaining_moves - 1) * (float) increment) / remaining_moves - epsilon;

            float seconds_remaining = ms_remaining / 1000;

            return std::min(seconds_remaining, remaining_seconds);
        }
    }

}