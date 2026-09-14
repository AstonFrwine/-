// Tree.h
#ifndef TREE_H
#define TREE_H

#include "Edge.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>

class TreeNode {
public:
    int data;
    std::vector<std::shared_ptr<TreeNode>> children;
    
    TreeNode(int d) : data(d) {}
    
    void addChild(std::shared_ptr<TreeNode> child) {
        children.push_back(child);
    }
};

class Tree {
private:
    std::shared_ptr<TreeNode> root;
    int vertexCount;
    
public:
    Tree(int rootData, int n);
    
    void buildFromEdges(const std::vector<WeightedEdge>& edges);
    void printTree() const;
    void printTreeToFile(const std::string& filename) const;
    
    std::shared_ptr<TreeNode> getRoot() const { return root; }
    
private:
    void printNode(const std::shared_ptr<TreeNode>& node, 
                  int depth, 
                  std::vector<bool>& lastFlag,
                  std::stringstream& ss) const;
    std::shared_ptr<TreeNode> findNode(int data) const;
    std::shared_ptr<TreeNode> findNodeHelper(const std::shared_ptr<TreeNode>& node, int data) const;
};

#endif // TREE_H