// AdjacencyMultiList.cpp
#include "AdjacencyMultiList.h"
#include <algorithm>
#include <iostream>

AdjacencyMultiList::AdjacencyMultiList(int n, bool dir) 
    : vertexCount(n), directed(dir) {
    for (int i = 0; i <= n; i++) {
        vertices.push_back(VertexNode(i));
    }
}

void AdjacencyMultiList::addEdge(int v1, int v2, int weight) {
    if (v1 < 1 || v1 > vertexCount || v2 < 1 || v2 > vertexCount) {
        std::cerr << "错误：顶点编号 " << v1 << " 或 " << v2 << " 超出范围" << std::endl;
        return;
    }
    
    if (v1 == v2) {
        std::cerr << "警告：忽略自环边 (" << v1 << ", " << v2 << ")" << std::endl;
        return;
    }
    
    // 检查边是否已存在
    EdgeNode* p = vertices[v1].firstEdge;
    while (p != nullptr) {
        if ((p->v1 == v1 && p->v2 == v2) || (p->v1 == v2 && p->v2 == v1)) {
            std::cout << "边 (" << v1 << ", " << v2 << ") 已存在" << std::endl;
            return;
        }
        if (p->v1 == v1) {
            p = p->ilink;
        } else {
            p = p->jlink;
        }
    }
    
    // 创建新边节点（带权重）
    std::unique_ptr<EdgeNode> newEdge = std::make_unique<EdgeNode>(v1, v2, weight);
    EdgeNode* edgePtr = newEdge.get();
    
    // 添加到v1的边链表中
    if (vertices[v1].firstEdge == nullptr) {
        vertices[v1].firstEdge = edgePtr;
    } else {
        edgePtr->ilink = vertices[v1].firstEdge;
        vertices[v1].firstEdge = edgePtr;
    }
    
    // 如果是无向图，添加到v2的边链表中
    if (!directed) {
        if (vertices[v2].firstEdge == nullptr) {
            vertices[v2].firstEdge = edgePtr;
        } else {
            EdgeNode* p2 = vertices[v2].firstEdge;
            EdgeNode* prev = nullptr;
            
            // 找到插入位置
            while (p2 != nullptr) {
                if ((p2->v1 == v2 && p2->ilink == nullptr) || 
                    (p2->v2 == v2 && p2->jlink == nullptr)) {
                    break;
                }
                prev = p2;
                if (p2->v1 == v2) {
                    p2 = p2->ilink;
                } else {
                    p2 = p2->jlink;
                }
            }
            
            if (v2 == edgePtr->v1) {
                edgePtr->ilink = p2;
            } else {
                edgePtr->jlink = p2;
            }
            
            if (prev) {
                if (prev->v1 == v2) {
                    prev->ilink = edgePtr;
                } else {
                    prev->jlink = edgePtr;
                }
            } else {
                vertices[v2].firstEdge = edgePtr;
            }
        }
    }
    
    edges.push_back(std::move(newEdge));
    std::cout << "添加带权边: (" << v1 << ", " << v2 << ") 权重=" << weight << std::endl;
}

std::vector<std::pair<int, int>> AdjacencyMultiList::getAdjacentVerticesWithWeight(int v) const {
    std::vector<std::pair<int, int>> adjVertices; // (顶点, 权重)
    
    if (v < 1 || v > vertexCount) {
        return adjVertices;
    }
    
    EdgeNode* p = vertices[v].firstEdge;
    while (p != nullptr) {
        if (p->v1 == v) {
            adjVertices.push_back({p->v2, p->weight});
            p = p->ilink;
        } else if (p->v2 == v) {
            adjVertices.push_back({p->v1, p->weight});
            p = p->jlink;
        } else {
            break;
        }
    }
    
    // 按顶点编号排序
    std::sort(adjVertices.begin(), adjVertices.end(), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
    
    return adjVertices;
}

std::vector<int> AdjacencyMultiList::getAdjacentVertices(int v) const {
    std::vector<int> adjVertices;
    auto adjWithWeight = getAdjacentVerticesWithWeight(v);
    for (const auto& [vertex, weight] : adjWithWeight) {
        adjVertices.push_back(vertex);
    }
    return adjVertices;
}

void AdjacencyMultiList::print() const {
    std::cout << "邻接多重表（带权重）：" << std::endl;
    for (int i = 1; i <= vertexCount; i++) {
        std::cout << "顶点 " << i << ": ";
        EdgeNode* p = vertices[i].firstEdge;
        while (p != nullptr) {
            if (p->v1 == i) {
                std::cout << p->v2 << "(" << p->weight << ")";
                p = p->ilink;
            } else {
                std::cout << p->v1 << "(" << p->weight << ")";
                p = p->jlink;
            }
            if (p) std::cout << " → ";
        }
        std::cout << std::endl;
    }
}