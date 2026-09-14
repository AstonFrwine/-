// AdjacencyList.h
#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include "Edge.h"
#include <vector>
#include <memory>
#include <utility>

struct AdjListNode {
    int dest;
    int weight;
    std::shared_ptr<AdjListNode> next;
    
    AdjListNode(int d, int w = 1) : dest(d), weight(w), next(nullptr) {}
};

struct AdjList {
    std::shared_ptr<AdjListNode> head;
    int vertex;
    
    AdjList(int v) : vertex(v), head(nullptr) {}
};

class AdjacencyList {
private:
    std::vector<AdjList> adjLists;
    int vertexCount;
    bool directed;
    
public:
    AdjacencyList(int n, bool dir = false);
    
    void addEdge(int src, int dest, int weight = 1);
    void removeEdge(int src, int dest);
    std::vector<std::pair<int, int>> getAdjacentVerticesWithWeight(int v) const;
    std::vector<int> getAdjacentVertices(int v) const;
    std::shared_ptr<AdjListNode> getAdjListHead(int v) const;
    std::vector<WeightedEdge> getAllEdgesWithWeight() const; // (src, dest, weight)
    std::vector<std::pair<int, int>> getAllEdges() const;
    void print() const;
    int getVertexCount() const { return vertexCount; }
    
private:
    bool edgeExists(int src, int dest) const;
};

#endif // ADJACENCY_LIST_H