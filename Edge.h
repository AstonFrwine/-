// Edge.h
#ifndef EDGE_H
#define EDGE_H

#include <tuple>

struct WeightedEdge {
    int from;
    int to;
    int weight;
    
    WeightedEdge(int f = 0, int t = 0, int w = 1) : from(f), to(t), weight(w) {}
    
    // 转换为tuple以便与现有代码兼容
    operator std::tuple<int, int, int>() const {
        return std::make_tuple(from, to, weight);
    }
    
    // 从tuple构造
    WeightedEdge(const std::tuple<int, int, int>& t) 
        : from(std::get<0>(t)), to(std::get<1>(t)), weight(std::get<2>(t)) {}
    
    WeightedEdge(const WeightedEdge& other) 
        : from(other.from), to(other.to), weight(other.weight) {}

    bool operator==(const WeightedEdge& other) const {
        return from == other.from && to == other.to;
    }
};

#endif // EDGE_H