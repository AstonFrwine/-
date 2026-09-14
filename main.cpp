// main.cpp
// g++ -std=c++17 -o GraphProject4 main.cpp Graph.cpp AdjacencyMultiList.cpp AdjacencyList.cpp GraphVisualizer.cpp Tree.cpp -I.
// ./GraphProject4
#include "Graph.h"
#include <iostream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printMenu() {
    std::cout << "=========================================" << std::endl;
    std::cout << "       无向图算法与可视化系统" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "1. 加载无向图数据文件（无写明权重的视作1）" << std::endl;
    std::cout << "2. 手动添加带权边" << std::endl;
    std::cout << "3. 显示图结构信息" << std::endl;
    std::cout << "4. 深度优先遍历（递归）" << std::endl;
    std::cout << "5. 深度优先遍历（非递归）" << std::endl;
    std::cout << "6. 广度优先遍历" << std::endl;
    std::cout << "7. 构建深度优先生成树" << std::endl;
    std::cout << "8. 构建广度优先生成树" << std::endl;
    std::cout << "9. 树形结构输出" << std::endl;
    std::cout << "10. 计算最短路径（BFS，无权）" << std::endl;
    std::cout << "11. 计算最短路径（Dijkstra，带权）" << std::endl;
    std::cout << "12. 可视化原始图（带权重）" << std::endl;
    std::cout << "13. 可视化生成树" << std::endl;
    std::cout << "14. 可视化最短路径" << std::endl;
    std::cout << "15. 导出所有结果" << std::endl;
    std::cout << "0. 退出程序" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "请输入选择（0-15）: ";
}

void printGraphInfo(Graph* graph) {
    if (!graph) return;
    
    std::cout << "\n图结构信息：" << std::endl;
    std::cout << "顶点数: " << graph->getVertexCount() << std::endl;
    std::cout << "存储结构: 邻接多重表 + 邻接表" << std::endl;
    std::cout << "图类型: 无向图" << std::endl;
}

void addEdgesManually(Graph* graph) {
    if (!graph) {
        std::cout << "请先创建图！" << std::endl;
        return;
    }
    
    std::cout << "\n手动添加边 (输入0 0结束):" << std::endl;
    int v1, v2;
    int count = 0;
    
    while (true) {
        std::cout << "输入边 (格式: 顶点1 顶点2): ";
        std::cin >> v1 >> v2;
        
        if (v1 == 0 && v2 == 0) {
            break;
        }
        
        if (v1 < 1 || v2 < 1 || v1 > graph->getVertexCount() || v2 > graph->getVertexCount()) {
            std::cout << "顶点编号应在 1-" << graph->getVertexCount() << " 范围内" << std::endl;
            continue;
        }
        
        graph->addEdge(v1, v2);
        count++;
    }
    
    std::cout << "成功添加 " << count << " 条边" << std::endl;
}

void exportAllResults(Graph* graph) {
    if (!graph) {
        std::cout << "请先加载图数据！" << std::endl;
        return;
    }
    
    std::cout << "\n正在导出所有结果..." << std::endl;
    
    // 1. 导出原始图
    graph->exportToDot("data/original_graph.dot", false);
    graph->renderGraph("data/original_graph.dot", "data/original_graph.png");
    
    // 2. 导出DFS生成树
    int start;
    std::cout << "请输入DFS起点: ";
    std::cin >> start;
    
    graph->buildDFSTree(start);
    graph->exportToDot("data/dfs_tree.dot", true, true);
    graph->renderGraph("data/dfs_tree.dot", "data/dfs_tree.png");
    
    // 3. 导出BFS生成树
    std::cout << "请输入BFS起点: ";
    std::cin >> start;
    
    graph->buildBFSTree(start);
    graph->exportToDot("data/bfs_tree.dot", true, false);
    graph->renderGraph("data/bfs_tree.dot", "data/bfs_tree.png");
    
    // 4. 导出最短路径示例
    int target;
    std::cout << "请输入目标顶点: ";
    std::cin >> target;
    
    auto path = graph->shortestPathBFS(start, target);
    if (!path.empty()) {
        std::cout << "最短路径已计算，长度: " << path.size() - 1 << std::endl;
        std::cout << "路径: ";
        graph->printPath(path);
    }
    
    std::cout << "\n所有结果已导出到 data/ 目录：" << std::endl;
    std::cout << "1. original_graph.png - 原始图" << std::endl;
    std::cout << "2. dfs_tree.png       - DFS生成树" << std::endl;
    std::cout << "3. bfs_tree.png       - BFS生成树" << std::endl;
    std::cout << "4. *.dot 文件         - Graphviz源文件" << std::endl;
}

// 添加带权边的手动输入
void addWeightedEdgesManually(Graph* graph) {
    if (!graph) {
        std::cout << "请先创建图！" << std::endl;
        return;
    }
    
    std::cout << "\n手动添加带权边 (输入0 0 0结束):" << std::endl;
    int v1, v2, weight;
    int count = 0;
    
    while (true) {
        std::cout << "输入边和权重 (格式: 顶点1 顶点2 权重): ";
        std::cin >> v1 >> v2 >> weight;
        
        if (v1 == 0 && v2 == 0 && weight == 0) {
            break;
        }
        
        if (v1 < 1 || v2 < 1 || v1 > graph->getVertexCount() || v2 > graph->getVertexCount()) {
            std::cout << "警告：顶点编号应在 1-" << graph->getVertexCount() << " 范围内" << std::endl;
            continue;
        }
        
        if (weight <= 0) {
            std::cout << "警告：权重应为正数" << std::endl;
            continue;
        }
        
        graph->addWeightedEdge(v1, v2, weight);
        count++;
    }
    
    std::cout << "成功添加 " << count << " 条带权边" << std::endl;
}

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    Graph* graph = nullptr;
    std::string filename;
    std::string choice;
    
    while (true) {
        clearScreen();
        printMenu();
        std::cin >> choice;
        
        
        if (choice == "1") { // 1. 加载图数据文件
            std::cout << "txt文件内第一行应为: 顶点数量 边数量" << std::endl;
            std::cout << "随后每行边的格式应为: 顶点1 顶点2 权重(可选)" << std::endl;
            std::cout << "\n请输入数据文件名 (如: data/graph_data.txt): ";
            std::cin >> filename;
            
            if (graph) delete graph;
            graph = new Graph(0, false);  // 无向图
            
            if (graph->loadFromFile(filename)) {
                std::cout << "图数据加载成功！" << std::endl;
                printGraphInfo(graph);
            } else {
                std::cout << "加载失败，请检查文件格式" << std::endl;
            }
        }
        
        else if (choice == "2") { // 2. 手动添加带权边
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                addWeightedEdgesManually(graph);
            }
        }
        
        else if (choice == "3") { // 3. 显示图结构信息
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                printGraphInfo(graph);
            }
        }
        
        else if (choice == "4") { // 4. 深度优先遍历（递归）
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start;
                std::cin >> start;
                graph->DFS(start);
            }
        }
        
        else if (choice == "5") { // 5. 深度优先遍历（非递归）
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start;
                std::cin >> start;
                graph->DFS_NonRecursive(start);
            }
        }
        
        else if (choice == "6") { // 6. 广度优先遍历
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start;
                std::cin >> start;
                graph->BFS(start);
            }
        }
        
        else if (choice == "7") { // 7. 构建深度优先生成树
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start;
                std::cin >> start;
                graph->buildDFSTree(start);
            }
        }
        
        else if (choice == "8") { // 8. 构建广度优先生成树
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start;
                std::cin >> start;
                graph->buildBFSTree(start);
            }
        }
        
        else if (choice == "9") { // 9. 树形结构输出
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "选择树类型 (1=DFS树, 2=BFS树): ";
                int treeType;
                std::cin >> treeType;
                
                if (treeType == 1) {
                    graph->printDFSTree();
                } else {
                    graph->printBFSTree();
                }
            }
        }
        
        else if (choice == "10") { // 10. 计算最短路径（BFS，无权）
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start, target;
                std::cin >> start;
                std::cout << "请输入目标顶点编号: ";
                std::cin >> target;
                
                auto path = graph->shortestPathBFS(start, target);
                if (path.empty()) {
                    std::cout << "不存在从 " << start << " 到 " << target << " 的路径" << std::endl;
                } else {
                    std::cout << "✓ 最短路径长度: " << path.size() - 1 << std::endl;
                    std::cout << "路径: ";
                    graph->printPath(path);
                }
            }
        }

        else if (choice == "11") { // 11. 计算最短路径（Dijkstra，带权）
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start, target;
                std::cin >> start;
                std::cout << "请输入目标顶点编号: ";
                std::cin >> target;
                
                auto path = graph->shortestPathDijkstra(start, target);
                if (path.empty()) {
                    std::cout << "不存在从 " << start << " 到 " << target << " 的路径" << std::endl;
                } else {
                    std::cout << "✓ Dijkstra最短路径长度: " << path.size() - 1 << std::endl;
                    std::cout << "路径: ";
                    graph->printPath(path);
                }
            }
        }

        else if (choice == "12") { // 12. 可视化原始图（带权重）
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                graph->exportToDot("data/graph_original.dot", false);
                graph->renderGraph("data/graph_original.dot", "data/graph_original.png");
                std::cout << "✓ 原始图已生成，请查看 data/graph_original.png" << std::endl;
            }
        }

        else if (choice == "13") { // 13. 可视化生成树
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "选择生成树类型 (1=DFS, 2=BFS): ";
                int treeType;
                std::cin >> treeType;
                
                std::cout << "请输入起点顶点编号: ";
                int start;
                std::cin >> start;
                
                if (treeType == 1) {
                    graph->buildDFSTree(start);
                    graph->exportToDot("data/dfs_tree.dot", true, true);
                    graph->renderGraph("data/dfs_tree.dot", "data/dfs_tree.png");
                    std::cout << "✓ DFS生成树已生成，请查看 data/dfs_tree.png" << std::endl;
                } else {
                    graph->buildBFSTree(start);
                    graph->exportToDot("data/bfs_tree.dot", true, false);
                    graph->renderGraph("data/bfs_tree.dot", "data/bfs_tree.png");
                    std::cout << "✓ BFS生成树已生成，请查看 data/bfs_tree.png" << std::endl;
                }
            }
        }

        else if (choice == "14") { // 14. 可视化最短路径
            if (!graph) {
                std::cout << "请先加载图数据！" << std::endl;
            } else {
                std::cout << "请输入起点顶点编号: ";
                int start, target;
                std::cin >> start;
                std::cout << "请输入目标顶点编号: ";
                std::cin >> target;
                
                auto path = graph->shortestPathBFS(start, target);
                if (!path.empty()) {
                    // 创建带高亮路径的图
                    std::vector<std::pair<int, int>> allEdges;
                    // 这里需要获取所有边（简化处理）
                    // 实际应该从adjList获取
                    std::cout << "最短路径可视化功能开发中..." << std::endl;
                    std::cout << "路径: ";
                    graph->printPath(path);
                }
            }
        }

        else if (choice == "15") { // 15. 导出所有结果
            exportAllResults(graph);
        }
        
        else if (choice == "0") {
            if (graph) delete graph;
            std::cout << "感谢使用，再见！" << std::endl;
            return 0;
        }

        else {
            std::cout << "无效选择，请重新输入！" << std::endl;
        }
        
        
        std::cout << "\n按回车键继续...";
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}