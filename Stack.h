// Stack.h
#ifndef STACK_H
#define STACK_H

#include "Edge.h"
#include <vector>
#include <stdexcept>
#include <memory>

// ==============================
// 基础模板栈类
// ==============================
template<typename T>
class Stack {
private:
    std::vector<T> data;
    
public:
    Stack() = default;
    
    // 入栈
    void push(const T& value) {
        data.push_back(value);
    }
    
    void push(T&& value) {
        data.push_back(std::move(value));
    }
    
    // 出栈
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack underflow");
        }
        T value = std::move(data.back());
        data.pop_back();
        return value;
    }
    
    // 查看栈顶元素
    const T& top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
    
    T& top() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
    
    // 栈是否为空
    bool isEmpty() const {
        return data.empty();
    }
    
    // 栈大小
    size_t size() const {
        return data.size();
    }
    
    // 清空栈
    void clear() {
        data.clear();
    }
    
    // 检查栈中是否包含元素
    bool contains(const T& value) const {
        for (const auto& item : data) {
            if (item == value) {
                return true;
            }
        }
        return false;
    }
};

// ==============================
// DFS相关栈元素结构
// ==============================
struct DFSStackElement {
    int vertex;          // 当前顶点
    int parent;          // 父节点
    int edgeWeight;      // 边权重（新增）
    
    DFSStackElement(int v, int p = -1, int w = 1) 
        : vertex(v), parent(p), edgeWeight(w) {}
    
    bool operator==(const DFSStackElement& other) const {
        return vertex == other.vertex;
    }
};

// ==============================
// 用于Dijkstra算法的优先队列元素
// ==============================
struct DijkstraPQElement {
    int vertex;
    int distance;
    
    DijkstraPQElement(int v, int d) : vertex(v), distance(d) {}
    
    // 用于优先队列的比较（距离越小优先级越高）
    bool operator>(const DijkstraPQElement& other) const {
        return distance > other.distance;
    }
    
    bool operator<(const DijkstraPQElement& other) const {
        return distance < other.distance;
    }
};

// ==============================
// 用于最短路径回溯的结构
// ==============================
struct PathNode {
    int vertex;
    std::shared_ptr<PathNode> prev;
    int distance;
    
    PathNode(int v, std::shared_ptr<PathNode> p = nullptr, int d = 0)
        : vertex(v), prev(p), distance(d) {}
};

// ==============================
// 用于非递归DFS的增强栈
// ==============================
class DFSStack {
private:
    Stack<DFSStackElement> stack;
    std::vector<bool> visited;
    int startVertex;
    
public:
    DFSStack(int vertexCount, int start) 
        : visited(vertexCount + 1, false), startVertex(start) {
        push(start, -1, 0);  // 起点没有父节点和权重
    }
    
    // 入栈
    void push(int vertex, int parent, int weight) {
        stack.push(DFSStackElement(vertex, parent, weight));
    }
    
    // 出栈
    DFSStackElement pop() {
        return stack.pop();
    }
    
    // 栈是否为空
    bool isEmpty() const {
        return stack.isEmpty();
    }
    
    // 标记顶点为已访问
    void markVisited(int vertex) {
        if (vertex >= 0 && vertex < visited.size()) {
            visited[vertex] = true;
        }
    }
    
    // 检查顶点是否已访问
    bool isVisited(int vertex) const {
        if (vertex >= 0 && vertex < visited.size()) {
            return visited[vertex];
        }
        return false;
    }
    
    // 获取已访问顶点列表
    std::vector<int> getVisitedVertices() const {
        std::vector<int> result;
        for (size_t i = 1; i < visited.size(); i++) {
            if (visited[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
    
    // 清空栈
    void clear() {
        stack.clear();
        std::fill(visited.begin(), visited.end(), false);
    }
};

// ==============================
// 用于BFS的队列
// ==============================
template<typename T>
class Queue {
private:
    std::vector<T> data;
    size_t frontIndex;
    
public:
    Queue() : frontIndex(0) {}
    
    // 入队
    void enqueue(const T& value) {
        data.push_back(value);
    }
    
    void enqueue(T&& value) {
        data.push_back(std::move(value));
    }
    
    // 出队
    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue underflow");
        }
        T value = std::move(data[frontIndex]);
        frontIndex++;
        
        // 定期清理已出队的元素
        if (frontIndex > data.size() / 2) {
            data.erase(data.begin(), data.begin() + frontIndex);
            frontIndex = 0;
        }
        
        return value;
    }
    
    // 查看队首元素
    const T& front() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return data[frontIndex];
    }
    
    T& front() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return data[frontIndex];
    }
    
    // 队列是否为空
    bool isEmpty() const {
        return frontIndex >= data.size();
    }
    
    // 队列大小
    size_t size() const {
        return data.size() - frontIndex;
    }
    
    // 清空队列
    void clear() {
        data.clear();
        frontIndex = 0;
    }
};

#endif // STACK_H