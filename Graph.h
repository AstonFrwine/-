// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include "AdjacencyMultiList.h"
#include "AdjacencyList.h"
#include "Edge.h"

class Graph {
private:
    int vertexCount;
    bool directed;
    std::unique_ptr<AdjacencyMultiList> multiList;
    std::unique_ptr<AdjacencyList> adjList;
    
    // 遍历结果存储
    std::vector<int> dfsSequence;
    std::vector<WeightedEdge> dfsTreeEdges; // (from, to, weight)
    std::vector<int> bfsSequence;
    std::vector<WeightedEdge> bfsTreeEdges; // (from, to, weight)
    
public:
    Graph(int n, bool dir = false);
    ~Graph(){}

    // 辅助函数
    void DFSUtil(int v, std::vector<bool>& visited, std::vector<WeightedEdge>& edges);
    
    // 图构建
    bool loadFromFile(const std::string& filename);
    void addEdge(int v1, int v2, int weight = 1);
    void addWeightedEdge(int v1, int v2, int weight = 1);
    
    // 遍历算法
    void DFS(int start);
    void DFS_NonRecursive(int start);
    void BFS(int start);
    
    // 生成树
    void buildDFSTree(int start);
    void buildBFSTree(int start);
    
    // 最短路径
    std::vector<int> shortestPathBFS(int start, int target);
    std::vector<int> shortestPathDijkstra(int start, int target);
    std::pair<std::vector<int>, int> shortestPathDijkstraWithDistance(int start, int target);
    
    // 输出结果
    void printDFSSequence() const;
    void printBFSSequence() const;
    void printDFSTreeEdges() const;
    void printBFSTreeEdges() const;
    void printDFSTree() const;
    void printBFSTree() const;
    void printPath(const std::vector<int>& path) const;
    void printGraphInfo() const;
    
    // 图信息
    int getVertexCount() const { return vertexCount; }
    const std::vector<WeightedEdge>& getDFSTreeEdges() const { return dfsTreeEdges; }
    const std::vector<WeightedEdge>& getBFSTreeEdges() const { return bfsTreeEdges; }
    std::vector<WeightedEdge> getAllEdgesWithWeight() const;
    
    // 可视化
    void exportToDot(const std::string& filename, bool showTree = false, bool isDFSTree = true, bool showWeights = true) const;
    void renderGraph(const std::string& dotFile, const std::string& outputFile) const;
};

#endif // GRAPH_H