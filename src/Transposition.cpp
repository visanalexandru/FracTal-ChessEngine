#include "Transposition.h"

namespace Engine {

    Transposition::Transposition() : type(NodeType::Null), zobrist_hash(0), depth(0), value(0) {

    }

    Transposition::Transposition(Engine::NodeType node_type, uint64_t node_hash, int node_depth, int node_value,
                                 Move node_best_move) :
            type(node_type), zobrist_hash(node_hash), depth(node_depth), value(node_value), best_move(node_best_move) {


    }

    uint64_t Transposition::getZobristHash() const {
        return zobrist_hash;
    }

    int Transposition::getDepth() const {
        return depth;
    }

    NodeType Transposition::getType() const {
        return type;
    }

    int Transposition::getValue() const {
        return value;
    }

    const Move &Transposition::getBestMove() const {
        return best_move;
    }

}