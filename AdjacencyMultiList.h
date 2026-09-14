// AdjacencyMultiList.h
#ifndef ADJACENCY_MULTI_LIST_H
#define ADJACENCY_MULTI_LIST_H

#include <vector>
#include <memory>
#include <iostream>

struct EdgeNode {
    int v1, v2;
    int weight;  // 添加权重
    EdgeNode* ilink;
    EdgeNode* jlink;
    bool visited;
    
    EdgeNode(int a, int b, int w = 1) : v1(a), v2(b), weight(w), 
                                       ilink(nullptr), jlink(nullptr), 
                                       visited(false) {}
};

struct VertexNode {
    int data;
    EdgeNode* firstEdge;
    
    VertexNode(int d) : data(d), firstEdge(nullptr) {}
};

class AdjacencyMultiList {
private:
    std::vector<VertexNode> vertices;
    std::vector<std::unique_ptr<EdgeNode>> edges;
    int vertexCount;
    bool directed;
    
public:
    AdjacencyMultiList(int n, bool dir = false);
    
    void addEdge(int v1, int v2, int weight = 1);
    void resetVisited();
    
    // 获取邻接边（带权重）
    std::vector<std::pair<int, int>> getAdjacentVerticesWithWeight(int v) const;
    std::vector<int> getAdjacentVertices(int v) const;
    EdgeNode* getFirstEdge(int v) const;
    
    int getVertexCount() const { return vertexCount; }
    const std::vector<VertexNode>& getVertices() const { return vertices; }
    const std::vector<std::unique_ptr<EdgeNode>>& getAllEdges() const { return edges; }
    
    void markEdgeVisited(EdgeNode* edge, int from);
    void print() const;
};

#endif // ADJACENCY_MULTI_LIST_H