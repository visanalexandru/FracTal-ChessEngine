#include "Move.h"

namespace Engine {

    Move::Move(MoveType type, uint64_t start, uint64_t end, Piece piece_moved, Piece piece_taken, Piece promotion) :
            move_type(type),
            origin(start),
            destination(end),
            taken(piece_taken),
            promote_to(promotion),
            moved(piece_moved),
            score(0) {


    }

    int Move::getScore() const {
        return score;
    }

    void Move::setScore(int new_score) {
        score = new_score;
    }

    Move::Move() : move_type(MoveType::Null),
                   taken(Piece::None) {

    }


    uint64_t Move::getOrigin() const {
        return origin;
    }

    uint64_t Move::getDestination() const {
        return destination;
    }

    MoveType Move::getType() const {
        return move_type;
    }

    Piece Move::getMoved() const {
        return moved;
    }

    Piece Move::getTaken() const {
        return taken;
    }

    Piece Move::getPromotion() const {
        return promote_to;
    }

    std::string Move::toString() const {
        std::string result;
        int orgindex = bitScanForward(origin), destindex = bitScanForward(destination);

        int orgy = orgindex / 8;
        int orgx = (7 - orgindex % 8);

        int desty = destindex / 8;
        int destx = (7 - destindex % 8);

        result += (char) (orgx + 'a');
        result += (char) (orgy + '1');

        result += (char) (destx + 'a');
        result += (char) (desty + '1');

        if (move_type == MoveType::Promote) {
            result += tolower(getPieceChar(promote_to));
        }


        return result;
    }

    bool Move::operator==(const Move &rhs) const {
        return origin == rhs.origin &&
               destination == rhs.destination &&
               move_type == rhs.move_type &&
               taken == rhs.taken &&
               promote_to == rhs.promote_to &&
               moved == rhs.moved;
    }
}
