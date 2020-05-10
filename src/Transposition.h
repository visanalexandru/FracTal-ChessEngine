

#ifndef CHESSENGINE_TRANSPOSITION_H
#define CHESSENGINE_TRANSPOSITION_H

#include <cstdint>

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
    public:
        uint64_t getZobristHash() const;
        int getDepth() const;
        NodeType getType() const;
        int getValue() const;
        Transposition();
        Transposition(NodeType node_type,uint64_t node_hash,int node_depth,int node_value);
    };

}


#endif
