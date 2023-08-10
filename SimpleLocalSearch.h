#ifndef GRAPH_COLORING_PROBLEM_SIMPLELOCALSEARCH_H
#define GRAPH_COLORING_PROBLEM_SIMPLELOCALSEARCH_H

#include "Graph.h"

class SimpleLocalSearch {
public:
    /**
     * 简单的局部搜索
     * @param graph 图
     * @param colors 点的颜色
     * @param k 颜色的数量
     */
    SimpleLocalSearch(const Graph &graph, const std::vector<int> &colors, int k)
            : graph_(graph), k_(k) {
        colors_ = colors;
    }

    void Search();

    int conflictNum{};

protected:
    const Graph &graph_;
    std::vector<int> colors_{};
    int k_{};
    std::vector<std::vector<int>> adjacentColorTable_{};

    /**
     * 找到最好的邻域动作
     * @param adjacentColorTable 邻接颜色表
     * @param bestNeighbors 最好的邻居列表
     * @param bestDeltaObj 最好的增量
     * @return
     */
    void FindBestNeighbor(std::vector<std::pair<int, int>> &bestNeighbors, int &bestDeltaObj);

};


#endif //GRAPH_COLORING_PROBLEM_SIMPLELOCALSEARCH_H
