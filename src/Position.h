#ifndef POSITION_H
#define POSITION_H

namespace Engine {
    class Position {
    public:
        int x;
        int y;

        Position(int a = 0, int b = 0);

        bool isInside();

        Position operator+(const Position &other) const;

        Position operator-(const Position &other) const;

        void operator+=(const Position &other);

        void operator-=(const Position &other);
    };
}

#endif
