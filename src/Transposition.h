

#ifndef CHESSENGINE_TRANSPOSITION_H
#define CHESSENGINE_TRANSPOSITION_H

#include <cstdint>
#include "Move.h"

namespace Engine {
    enum class NodeType {
        Null,
        Exact,
        LowerBound,
        UpperBound
    };

    class Transposition {
    private:
        uint64_t zobrist_hash;
        int depth;
        NodeType type;
        int value;
        Move best_move;
    public:
        uint64_t getZobristHash() const;

        int getDepth() const;

        NodeType getType() const;

        int getValue() const;

        const Move &getBestMove() const;

        Transposition();

        Transposition(NodeType node_type, uint64_t node_hash, int node_depth, int node_value, Move node_best_move);
    };

}


#endif
