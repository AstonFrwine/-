// GraphVisualizer.cpp
#include "GraphVisualizer.h"
#include "FontConfig.h"
#include "Edge.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

// 带权图导出

// 普通图导出（调用带权版本，权重默认为1）
void GraphVisualizer::exportGraphToDot(const std::string& filename, int vertexCount, const std::vector<WeightedEdge>& edges,
                                             GraphType type, bool showWeights,
                                             const std::vector<int>& highlightPath, int startVertex) {
    exportWeightedGraphToDot(filename, vertexCount, edges, type, false, highlightPath, startVertex);
}

std::string GraphVisualizer::getEdgeAttributes(GraphType type, int from, int to,
                                             int weight, bool showWeight,
                                             const std::vector<int>& highlightPath) {
    std::stringstream ss;
    
    // 检查边是否在最短路径中
    bool isInPath = false;
    if (!highlightPath.empty()) {
        for (size_t i = 0; i < highlightPath.size() - 1; i++) {
            if ((highlightPath[i] == from && highlightPath[i + 1] == to) ||
                (highlightPath[i] == to && highlightPath[i + 1] == from)) {
                isInPath = true;
                break;
            }
        }
    }
    
    // 设置边属性
    ss << "[";
    
    // 颜色和线宽
    if (isInPath) {
        ss << "color=red, penwidth=3";
    } else if (type == DFS_TREE || type == BFS_TREE) {
        ss << "color=blue, penwidth=2";
    } else {
        ss << "color=gray50, penwidth=1.5";
    }
    
    // 标签（权重）
    if (showWeight && weight != 1) {
        if (isInPath) {
            ss << ", label=\"" << weight << "\", fontcolor=red, fontsize=12";
        } else {
            ss << ", label=\"" << weight << "\", fontcolor=black, fontsize=10";
        }
    } else if (type == DFS_TREE || type == BFS_TREE) {
        ss << ", label=\"\"";
    }
    
    // 其他属性
    if (type == DFS_TREE || type == BFS_TREE) {
        ss << ", arrowhead=normal";
    }
    
    ss << "]";
    
    return ss.str();
}

std::string GraphVisualizer::getGraphAttributes(GraphType type) {
    std::stringstream ss;
    
    ss << "  rankdir=TB;\n";
    ss << "  node [" << FontConfig::getNodeStyle() << "];\n";
    ss << "  edge [" << FontConfig::getEdgeStyle() << "];\n";
    
    std::string title;
    if (FontConfig::isChineseSupported()) {
        // 中文标题
        switch (type) {
            case ORIGINAL: title = "原始图结构（带权重）"; break;
            case DFS_TREE: title = "深度优先生成树"; break;
            case BFS_TREE: title = "广度优先生成树"; break;
            case SHORTEST_PATH: title = "最短路径"; break;
        }
    } else {
        // 如果不行，打印英文标题
        switch (type) {
            case ORIGINAL: title = "Original Graph (with weights)"; break;
            case DFS_TREE: title = "DFS Spanning Tree"; break;
            case BFS_TREE: title = "BFS Spanning Tree"; break;
            case SHORTEST_PATH: title = "Shortest Path"; break;
        }
    }
    
    if (FontConfig::isChineseSupported()) {
        ss << "  label=" << FontConfig::getFontTag(title, 20) << ";\n";
    } else {
        ss << "  label=\"" << title << "\";\n";
    }
    
    ss << "  labelloc=t;\n";
    
    return ss.str();
}

std::string GraphVisualizer::getVertexAttributes(int vertex, GraphType type, int startVertex) {
    std::stringstream ss;
    
    ss << "[";
    
    // 特殊标记起点
    if (vertex == startVertex) {
        ss << "fillcolor=orange, fontcolor=black, penwidth=2, style=\"filled,bold\"";
    } else if (type == DFS_TREE || type == BFS_TREE) {
        // 生成树中的非根节点
        ss << "fillcolor=lightblue, fontcolor=black";
    } else {
        ss << "fillcolor=lightblue, fontcolor=black";
    }
    
    ss << "]";
    
    return ss.str();
}

bool GraphVisualizer::renderDotToImage(const std::string& dotFile,
                                     const std::string& outputFile,
                                     const std::string& format) {
    // 构建命令行
    std::string command;
    
#ifdef _WIN32
    command = "dot -T" + format + " \"" + dotFile + "\" -o \"" + outputFile + "\"";
#else
    command = "dot -T" + format + " '" + dotFile + "' -o '" + outputFile + "'";
#endif
    
    std::cout << "执行命令: " << command << std::endl;
    
    int result = system(command.c_str());
    
    if (result == 0) {
        std::cout << "✓ Graphviz渲染成功！" << std::endl;
        return true;
    } else {
        std::cerr << "✗ Graphviz渲染失败" << std::endl;
        std::cerr << "请检查：\n";
        std::cerr << "1. Graphviz是否已安装\n";
        std::cerr << "2. 'dot'命令是否在PATH中\n";
        std::cerr << "3. 可以手动执行命令: " << command << std::endl;
        return false;
    }
}

void GraphVisualizer::exportWeightedGraphToDot(const std::string& filename, int vertexCount, const std::vector<WeightedEdge>& edges,
                                             GraphType type, bool showWeights,
                                             const std::vector<int>& highlightPath, int startVertex) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }
    
    std::stringstream ss;
    
    // 根据图类型设置方向
    if (type == ORIGINAL) {
        ss << "graph G {\n";
    } else {
        ss << "digraph G {\n";  // 有向图用于树
    }
    
    // 图属性
    ss << getGraphAttributes(type);
    // 设置顶点属性
    for (int i = 1; i <= vertexCount; i++) {
        ss << "  " << i << " " << getVertexAttributes(i, type, startVertex) << ";\n";
    }
    // 添加边（带权重）
    std::unordered_set<std::string> addedEdges;
    for (const auto& edge : edges) {
        int from = edge.from;
        int to = edge.to;
        int weight = edge.weight;
        std::string edgeKey = std::to_string(from) + "-" + std::to_string(to);
        std::string reverseKey = std::to_string(to) + "-" + std::to_string(from);
    
        // 如果是无向图且边已添加，则跳过
        if (type == ORIGINAL && addedEdges.count(reverseKey)) {
            continue;
        }
        
        ss << "  " << from;
        if (type == ORIGINAL) {
            ss << " -- ";
        } else {
            ss << " -> ";
        }
        ss << to << " " << getEdgeAttributes(type, from, to, weight, 
                                           showWeights, highlightPath) << ";\n";
        
        addedEdges.insert(edgeKey);
    }
    
    ss << "}\n";
    outFile << ss.str();
    outFile.close();
    
    std::cout << "带权图DOT文件已生成: " << filename << std::endl;
}
