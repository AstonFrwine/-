// GraphVisualizer.h
#ifndef GRAPH_VISUALIZER_H
#define GRAPH_VISUALIZER_H

#include "Edge.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>

class GraphVisualizer {
public:
    enum GraphType {
        ORIGINAL,
        DFS_TREE,
        BFS_TREE,
        SHORTEST_PATH
    };
    
    // 普通图导出
    static void exportGraphToDot(const std::string& filename, int vertexCount, const std::vector<WeightedEdge>& edges,
                                GraphType type = ORIGINAL, bool showWeights = false,
                                const std::vector<int>& highlightPath = {}, int startVertex = -1);
    
    // 带权图导出
    static void exportWeightedGraphToDot(const std::string& filename, int vertexCount, const std::vector<WeightedEdge>& edges,
                                        GraphType type = ORIGINAL, bool showWeights = true,
                                        const std::vector<int>& highlightPath = {}, int startVertex = -1);
    // 渲染DOT文件为图像
    static bool renderDotToImage(const std::string& dotFile, 
                                const std::string& outputFile = "graph.png",
                                const std::string& format = "png");
    
private:
    static std::string getGraphAttributes(GraphType type);
    static std::string getEdgeAttributes(GraphType type, int from, int to, 
                                        int weight = 1, bool showWeight = true,
                                        const std::vector<int>& highlightPath = {});
    static std::string getVertexAttributes(int vertex, GraphType type, 
                                          int startVertex = -1);
};

#endif // GRAPH_VISUALIZER_H