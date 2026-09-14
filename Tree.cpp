// Tree.cpp
#include "Tree.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <algorithm>

Tree::Tree(int rootData, int n) : vertexCount(n) {
    root = std::make_shared<TreeNode>(rootData);
}

std::shared_ptr<TreeNode> Tree::findNodeHelper(const std::shared_ptr<TreeNode>& node, int data) const {
    if (node->data == data) {
        return node;
    }
    
    for (const auto& child : node->children) {
        std::shared_ptr<TreeNode> found = findNodeHelper(child, data);
        if (found != nullptr) {
            return found;
        }
    }
    
    return nullptr;
}

std::shared_ptr<TreeNode> Tree::findNode(int data) const {
    return findNodeHelper(root, data);
}

void Tree::buildFromEdges(const std::vector<WeightedEdge>& edges) {
    // 首先确保根节点存在
    if (root == nullptr) {
        std::cerr << "错误：树没有根节点" << std::endl;
        return;
    }
    
    // 使用BFS构建树，确保层次结构正确
    std::queue<int> nodeQueue;
    std::vector<bool> visited(vertexCount + 1, false);
    
    visited[root->data] = true;
    nodeQueue.push(root->data);
    
    while (!nodeQueue.empty()) {
        int currentData = nodeQueue.front();
        nodeQueue.pop();
        
        std::shared_ptr<TreeNode> currentNode = findNode(currentData);
        if (currentNode == nullptr) {
            continue;
        }
        
        // 找到所有以当前节点为起点的边
        for (const auto& edge : edges) {
            if (edge.from == currentData && !visited[edge.to]) {
                // 添加子节点
                std::shared_ptr<TreeNode> childNode = std::make_shared<TreeNode>(edge.to);
                currentNode->addChild(childNode);
                
                visited[edge.to] = true;
                nodeQueue.push(edge.to);
            }
        }
    }
}

void Tree::printNode(const std::shared_ptr<TreeNode>& node, 
                    int depth, 
                    std::vector<bool>& lastFlag,
                    std::stringstream& ss) const {
    // 打印缩进
    for (int i = 0; i < depth; i++) {
        if (i == depth - 1) {
            ss << (lastFlag[i] ? "└── " : "├── ");
        } else {
            ss << (lastFlag[i] ? "    " : "│   ");
        }
    }
    
    // 打印节点数据
    ss << node->data << std::endl;
    
    // 递归打印子节点
    int childCount = node->children.size();
    for (int i = 0; i < childCount; i++) {
        if (depth >= static_cast<int>(lastFlag.size())) {
            lastFlag.push_back(false);
        }
        lastFlag[depth] = (i == childCount - 1);
        printNode(node->children[i], depth + 1, lastFlag, ss);
    }
}

void Tree::printTree() const {
    if (root == nullptr) {
        std::cout << "树为空" << std::endl;
        return;
    }
    
    std::stringstream ss;
    std::vector<bool> lastFlag;
    
    ss << "树形结构：" << std::endl;
    lastFlag.push_back(true); // 根节点总是最后一个
    printNode(root, 0, lastFlag, ss);
    
    std::cout << ss.str();
}

void Tree::printTreeToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }
    
    std::stringstream ss;
    std::vector<bool> lastFlag;
    
    ss << "树形结构：" << std::endl;
    lastFlag.push_back(true);
    printNode(root, 0, lastFlag, ss);
    
    outFile << ss.str();
    outFile.close();
    
    std::cout << "树形结构已保存到: " << filename << std::endl;
}