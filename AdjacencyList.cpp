// AdjacencyList.cpp
#include "AdjacencyList.h"
#include "Edge.h"
#include <iostream>
#include <algorithm>
#include <tuple>

AdjacencyList::AdjacencyList(int n, bool dir) 
    : vertexCount(n), directed(dir) {
    for (int i = 0; i <= n; i++) {
        adjLists.push_back(AdjList(i));
    }
}

void AdjacencyList::addEdge(int src, int dest, int weight) {
    if (src < 1 || src > vertexCount || dest < 1 || dest > vertexCount) {
        std::cerr << "错误：顶点编号超出范围 (" << src << ", " << dest << ")" << std::endl;
        return;
    }
    
    if (src == dest) {
        std::cerr << "警告：忽略自环边 (" << src << ", " << dest << ")" << std::endl;
        return;
    }
    
    // 检查边是否已存在
    if (edgeExists(src, dest)) {
        std::cout << "边 (" << src << ", " << dest << ") 已存在" << std::endl;
        return;
    }
    
    // 创建新节点并添加到src的邻接表头部
    std::shared_ptr<AdjListNode> newNode = std::make_shared<AdjListNode>(dest, weight);
    newNode->next = adjLists[src].head;
    adjLists[src].head = newNode;
    
    // 如果是无向图，还需要添加反向边
    if (!directed && src != dest) {
        std::shared_ptr<AdjListNode> newNode2 = std::make_shared<AdjListNode>(src, weight);
        newNode2->next = adjLists[dest].head;
        adjLists[dest].head = newNode2;
    }
    
    std::cout << "添加带权边: " << src << " → " << dest << " (权重: " << weight << ")" << std::endl;
}

bool AdjacencyList::edgeExists(int src, int dest) const {
    std::shared_ptr<AdjListNode> p = adjLists[src].head;
    while (p != nullptr) {
        if (p->dest == dest) {
            return true;
        }
        p = p->next;
    }
    return false;
}

std::vector<std::pair<int, int>> AdjacencyList::getAdjacentVerticesWithWeight(int v) const {
    std::vector<std::pair<int, int>> adjVertices; // (顶点, 权重)
    
    if (v < 1 || v > vertexCount) {
        return adjVertices;
    }
    
    std::shared_ptr<AdjListNode> p = adjLists[v].head;
    while (p != nullptr) {
        adjVertices.push_back({p->dest, p->weight});
        p = p->next;
    }
    
    // 排序以确保顺序一致
    std::sort(adjVertices.begin(), adjVertices.end(), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
    
    return adjVertices;
}

std::vector<int> AdjacencyList::getAdjacentVertices(int v) const {
    std::vector<int> adjVertices;
    auto adjWithWeight = getAdjacentVerticesWithWeight(v);
    for (const auto& [vertex, weight] : adjWithWeight) {
        adjVertices.push_back(vertex);
    }
    return adjVertices;
}

std::vector<WeightedEdge> AdjacencyList::getAllEdgesWithWeight() const {
    std::vector<WeightedEdge> allEdges; // (src, dest, weight)
    std::vector<std::vector<bool>> visited(vertexCount + 1, 
                                          std::vector<bool>(vertexCount + 1, false));
    
    for (int i = 1; i <= vertexCount; i++) {
        std::shared_ptr<AdjListNode> p = adjLists[i].head;
        while (p != nullptr) {
            int dest = p->dest;
            if (!visited[i][dest] && !visited[dest][i]) {
                allEdges.push_back({i, dest, p->weight});
                visited[i][dest] = true;
                if (!directed) {
                    visited[dest][i] = true;
                }
            }
            p = p->next;
        }
    }
    
    return allEdges;
}

std::vector<std::pair<int, int>> AdjacencyList::getAllEdges() const {
    auto edgesWithWeight = getAllEdgesWithWeight();
    std::vector<std::pair<int, int>> allEdges;
    for (const auto& [src, dest, weight] : edgesWithWeight) {
        allEdges.push_back({src, dest});
    }
    return allEdges;
}

void AdjacencyList::print() const {
    std::cout << "邻接表（带权重）：" << std::endl;
    for (int i = 1; i <= vertexCount; i++) {
        std::cout << "顶点 " << i << ": ";
        std::shared_ptr<AdjListNode> p = adjLists[i].head;
        while (p != nullptr) {
            std::cout << p->dest << "(" << p->weight << ")";
            p = p->next;
            if (p) std::cout << " → ";
        }
        std::cout << std::endl;
    }
}