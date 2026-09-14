// Graph.cpp（部分关键实现）
#include "Graph.h"
#include "GraphVisualizer.h"
#include "Stack.h"
#include "Tree.h"
#include "Edge.h"
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <unordered_set>
#include <sstream>
#include <unordered_map>

Graph::Graph(int n, bool dir) : vertexCount(n), directed(dir) {
    multiList = std::make_unique<AdjacencyMultiList>(n, dir);
    adjList = std::make_unique<AdjacencyList>(n, dir);
}

void Graph::DFSUtil(int v, std::vector<bool>& visited, std::vector<WeightedEdge>& edges) {
    visited[v] = true;
    dfsSequence.push_back(v);
    
    auto adjVertices = multiList->getAdjacentVerticesWithWeight(v);
    for (const auto& [w, weight] : adjVertices) {
        if (!visited[w]) {
            edges.push_back(WeightedEdge(v, w, weight));
            DFSUtil(w, visited, edges);
        }
    }
}

// 非递归DFS实现
void Graph::DFS_NonRecursive(int start) {
    if (start < 1 || start > vertexCount) {
        std::cerr << "错误：起点顶点编号无效" << std::endl;
        return;
    }
    
    DFSStack dfsStack(vertexCount, start);
    dfsSequence.clear();
    dfsTreeEdges.clear();
    
    std::cout << "非递归DFS遍历序列：" << std::endl;
    
    while (!dfsStack.isEmpty()) {
        DFSStackElement current = dfsStack.pop();
        int v = current.vertex;
        int parent = current.parent;
        int weight = current.edgeWeight;
        
        if (!dfsStack.isVisited(v)) {
            dfsStack.markVisited(v);
            dfsSequence.push_back(v);
            
            // 输出访问顺序
            std::cout << v << " ";
            
            // 记录生成树边（带权重）
            if (parent != -1) {
                dfsTreeEdges.push_back({parent, v, weight});
            }
            
            // 获取所有邻接点（带权重）
            auto adjVertices = multiList->getAdjacentVerticesWithWeight(v);
            
            // 逆序入栈以保证与递归DFS顺序一致
            for (auto it = adjVertices.rbegin(); it != adjVertices.rend(); ++it) {
                int w = it->first;
                int edgeWeight = it->second;
                
                if (!dfsStack.isVisited(w)) {
                    dfsStack.push(w, v, edgeWeight);
                }
            }
        }
    }
    std::cout << std::endl;
    
    // 输出生成树边集（带权重）
    std::cout << "非递归DFS生成树边集（带权重）：" << std::endl;
    for (const auto& edge : dfsTreeEdges) {
        int from = edge.from;
        int to = edge.to;
        int weight = edge.weight;
        std::cout << "(" << from << "->" << to << ", 权重=" << weight << ") ";
    }
    std::cout << std::endl;
}

// 最短路径（BFS版本，用于无权图）
std::vector<int> Graph::shortestPathBFS(int start, int target) {
    if (start < 1 || start > vertexCount || target < 1 || target > vertexCount) {
        return {};
    }
    
    std::vector<bool> visited(vertexCount + 1, false);
    std::vector<int> parent(vertexCount + 1, -1);
    std::queue<int> q;
    
    visited[start] = true;
    q.push(start);
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        if (v == target) break;
        
        auto adjVertices = multiList->getAdjacentVertices(v);
        for (int w : adjVertices) {
            if (!visited[w]) {
                visited[w] = true;
                parent[w] = v;
                q.push(w);
            }
        }
    }
    
    // 回溯路径
    std::vector<int> path;
    if (parent[target] != -1 || start == target) {
        for (int v = target; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());
    }
    
    return path;
}

// 输出树形结构
void Graph::printDFSTree() const {
    std::cout << "\n深度优先生成树结构：" << std::endl;
    std::cout << "根节点: " << (dfsSequence.empty() ? -1 : dfsSequence[0]) << std::endl;
    
    // 构建邻接表表示树
    std::vector<std::vector<int>> treeAdj(vertexCount + 1);
    for (const auto& edge : dfsTreeEdges) {
        treeAdj[edge.from].push_back(edge.to);
    }
    
    // 递归打印树
    std::function<void(int, int)> printNode = [&](int node, int depth) {
        for (int i = 0; i < depth; i++) {
            std::cout << (i == depth - 1 ? "|-- " : "    ");
        }
        std::cout << node << std::endl;
        
        for (int child : treeAdj[node]) {
            printNode(child, depth + 1);
        }
    };
    
    if (!dfsSequence.empty()) {
        printNode(dfsSequence[0], 0);
    }
}

// 使用Graphviz渲染图像
void Graph::renderGraph(const std::string& dotFile, const std::string& outputFile) const {
    if (GraphVisualizer::renderDotToImage(dotFile, outputFile)) {
        std::cout << "图像已生成: " << outputFile << std::endl;
    } else {
        std::cerr << "图像生成失败，请确保已安装Graphviz" << std::endl;
        std::cerr << "可以从 https://graphviz.org/download/ 下载安装" << std::endl;
    }
}

// Graph.cpp (补充部分)

void Graph::buildDFSTree(int start) {
    if (dfsTreeEdges.empty()) {
        DFS(start);
    }
    
    Tree dfsTree(start, vertexCount);
    dfsTree.buildFromEdges(dfsTreeEdges);
    std::cout << "\n深度优先生成树：" << std::endl;
    dfsTree.printTree();
}

void Graph::buildBFSTree(int start) {
    if (bfsTreeEdges.empty()) {
        BFS(start);
    }
    
    Tree bfsTree(start, vertexCount);
    bfsTree.buildFromEdges(bfsTreeEdges);
    std::cout << "\n广度优先生成树：" << std::endl;
    bfsTree.printTree();
}

void Graph::printBFSTree() const {
    std::cout << "\n广度优先生成树结构：" << std::endl;
    std::cout << "根节点: " << (bfsSequence.empty() ? -1 : bfsSequence[0]) << std::endl;
    
    // 构建邻接表表示树
    std::vector<std::vector<int>> treeAdj(vertexCount + 1);
    for (const auto& edge : bfsTreeEdges) {
        treeAdj[edge.from].push_back(edge.to);
    }
    
    // 使用队列进行层次遍历
    if (bfsSequence.empty()) {
        std::cout << "树为空" << std::endl;
        return;
    }
    
    std::queue<std::pair<int, int>> q; // (节点, 深度)
    q.push({bfsSequence[0], 0});
    
    while (!q.empty()) {
        auto [node, depth] = q.front();
        q.pop();
        
        // 打印缩进
        for (int i = 0; i < depth; i++) {
            std::cout << "    ";
        }
        
        // 打印节点
        std::cout << "|-- " << node << std::endl;
        
        // 将子节点加入队列
        for (int child : treeAdj[node]) {
            q.push({child, depth + 1});
        }
    }
}

void Graph::printDFSSequence() const {
    std::cout << "DFS遍历序列: ";
    for (int v : dfsSequence) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void Graph::printBFSTreeEdges() const {
    std::cout << "BFS生成树边集: ";
    for (const auto& edge : bfsTreeEdges) {
        std::cout << "(" << edge.from << "->" << edge.to << ") ";
    }
    std::cout << std::endl;
}

void Graph::printPath(const std::vector<int>& path) const {
    if (path.empty()) {
        std::cout << "空路径" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

// Graph.cpp（关键修改部分）

// 从文件加载图（兼容无权重格式）
bool Graph::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }
    
    int n, m;
    inFile >> n >> m;
    
    vertexCount = n;
    multiList = std::make_unique<AdjacencyMultiList>(n, directed);
    adjList = std::make_unique<AdjacencyList>(n, directed);
    
    std::cout << "正在加载图数据，顶点数: " << n << ", 边数: " << m << std::endl;
    
    int edgeCount = 0;
    std::string line;
    
    // 跳过第一行
    std::getline(inFile, line);
    
    while (edgeCount < m && std::getline(inFile, line)) {
        std::istringstream iss(line);
        int v1, v2;
        
        if (iss >> v1 >> v2) {
            int weight = 1;
            // 尝试读取第三个值作为权重
            if (iss >> weight) {
                // 成功读取权重
            }
            addEdge(v1, v2, weight);
            edgeCount++;
        }
    }
    
    inFile.close();
    
    std::cout << "成功加载 " << edgeCount << " 条边" << std::endl;
    return true;
}

void Graph::addEdge(int v1, int v2, int weight) {
    if (multiList) {
        multiList->addEdge(v1, v2, weight);
    }
    if (adjList) {
        adjList->addEdge(v1, v2, weight);
    }
}

void Graph::addWeightedEdge(int v1, int v2, int weight) {
    addEdge(v1, v2, weight);
}

void Graph::DFS(int start) {
    if (start < 1 || start > vertexCount) {
        std::cerr << "错误：起点顶点编号无效" << std::endl;
        return;
    }
    
    std::vector<bool> visited(vertexCount + 1, false);
    dfsSequence.clear();
    dfsTreeEdges.clear();
    
    std::cout << "递归DFS遍历序列：" << std::endl;
    DFSUtil(start, visited, dfsTreeEdges);
    
    for (int v : dfsSequence) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    
    std::cout << "DFS生成树边集（带权重）：" << std::endl;
    for (const auto& edge : dfsTreeEdges) {
        std::cout << "(" << edge.from << "->" << edge.to 
                  << ", 权重=" << edge.weight << ") ";
    }
    std::cout << std::endl;
}

// BFS（带权重记录）
void Graph::BFS(int start) {
    if (start < 1 || start > vertexCount) {
        std::cerr << "错误：起点顶点编号无效" << std::endl;
        return;
    }
    
    std::vector<bool> visited(vertexCount + 1, false);
    std::queue<int> q;
    bfsSequence.clear();
    bfsTreeEdges.clear();
    
    visited[start] = true;
    q.push(start);
    
    std::cout << "BFS遍历序列：" << std::endl;
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        bfsSequence.push_back(v);
        
        std::cout << v << " ";
        
        auto adjVertices = multiList->getAdjacentVerticesWithWeight(v);
        for (const auto& [w, weight] : adjVertices) {
            if (!visited[w]) {
                visited[w] = true;
                q.push(w);
                bfsTreeEdges.push_back({v, w, weight});
            }
        }
    }
    std::cout << std::endl;
    
    std::cout << "BFS生成树边集（带权重）：" << std::endl;
    for (const auto& edge : bfsTreeEdges) {
        std::cout << "(" << edge.from << "->" << edge.to 
                  << ", 权重=" << edge.weight << ") ";
    }
    std::cout << std::endl;
}

// Dijkstra最短路径（返回路径和总距离）
std::pair<std::vector<int>, int> Graph::shortestPathDijkstraWithDistance(int start, int target) {
    if (start < 1 || start > vertexCount || target < 1 || target > vertexCount) {
        return {{}, -1};
    }
    // 初始化距离和前驱数组
    std::vector<int> dist(vertexCount + 1, std::numeric_limits<int>::max());
    std::vector<int> prev(vertexCount + 1, -1);
    std::vector<bool> visited(vertexCount + 1, false);
    dist[start] = 0;

    // 使用优先队列实现最小堆
    auto cmp = [&dist](int a, int b) { return dist[a] > dist[b]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);
    pq.push(start);
    
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        if (u == target) break;
        
        // 遍历u的所有邻接点
        auto adjVertices = adjList->getAdjacentVerticesWithWeight(u);
        for (const auto& [v, weight] : adjVertices) {
            if (!visited[v]) {
                int alt = dist[u] + weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    pq.push(v);
                }
            }
        }
    }
    // 回溯路径
    std::vector<int> path;
    int totalDistance = -1;
    if (dist[target] != std::numeric_limits<int>::max()) {
        totalDistance = dist[target];
        for (int v = target; v != -1; v = prev[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());
    }
    return {path, totalDistance};
}

std::vector<int> Graph::shortestPathDijkstra(int start, int target) {
    auto result = shortestPathDijkstraWithDistance(start, target);
    return result.first;
}

// 获取所有带权边
std::vector<WeightedEdge> Graph::getAllEdgesWithWeight() const {
    return adjList->getAllEdgesWithWeight();
}

// 导出为Graphviz格式（带权重）
void Graph::exportToDot(const std::string& filename, bool showTree, bool isDFSTree, bool showWeights) const {
    // 获取边数据
    std::vector<WeightedEdge> edges;
    
    if (showTree) {
        edges = isDFSTree ? dfsTreeEdges : bfsTreeEdges;
    } else {
        edges = getAllEdgesWithWeight();
    }
    
    // 转换为GraphVisualizer需要的格式
    GraphVisualizer::GraphType type = showTree ? 
        (isDFSTree ? GraphVisualizer::DFS_TREE : GraphVisualizer::BFS_TREE) : 
        GraphVisualizer::ORIGINAL;
    
    GraphVisualizer::exportWeightedGraphToDot(filename, vertexCount, 
                                             edges, type, showWeights);
    
    std::cout << "Graphviz DOT文件已生成: " << filename << std::endl;
}

// 打印图信息
void Graph::printGraphInfo() const {
    std::cout << "\n图结构信息：" << std::endl;
    std::cout << "顶点数: " << vertexCount << std::endl;
    std::cout << "存储结构: 邻接多重表 + 邻接表" << std::endl;
    std::cout << "图类型: " << (directed ? "有向图" : "无向图") << std::endl;
    
    auto allEdges = getAllEdgesWithWeight();
    std::cout << "边数: " << allEdges.size() << std::endl;
    std::cout << "权重范围: ";
    
    if (!allEdges.empty()) {
        int minWeight = allEdges[0].weight;
        int maxWeight = allEdges[0].weight;
        
        for (const auto& [from, to, weight] : allEdges) {
            if (weight < minWeight) minWeight = weight;
            if (weight > maxWeight) maxWeight = weight;
        }
        
        std::cout << minWeight << " 到 " << maxWeight << std::endl;
    } else {
        std::cout << "无" << std::endl;
    }
}
