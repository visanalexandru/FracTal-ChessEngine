#include "Types.h"


namespace Engine {
    namespace {
        const int piece_value[7]{1, 3, 3, 5, 10, 11, 0};
        const int color[13]{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2};
        const char piece_char[7]{'p', 'n', 'b', 'r', 'q', 'k', ' '};
    }


    uint8_t makePiece(Piece type, bool color) {
        uint8_t tp = static_cast<uint8_t>(type);
        if (color)
            tp |= 0x8;
        return tp;
    }

    Piece getPieceType(uint8_t pc) {
        uint8_t type = pc & 0x7;
        return static_cast<Piece>(type);
    }

    Color getPieceColor(uint8_t pc) {
        uint8_t color = pc & 0x8;
        if (color)
            return Black;
        return White;
    }

    int getPieceValue(uint8_t pc) {
        uint8_t value = piece_value[pc & 0x7];
        if (pc & 0x8)
            return -value;
        return value;
    }

    char getChar(uint8_t pc) {
        char lw = piece_char[pc & 0x7];
        if (pc & 0x8)
            return lw;
        return toupper(lw);
    }

}