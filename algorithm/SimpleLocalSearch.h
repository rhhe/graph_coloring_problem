#ifndef GRAPH_COLORING_PROBLEM_SIMPLELOCALSEARCH_H
#define GRAPH_COLORING_PROBLEM_SIMPLELOCALSEARCH_H

#include "Graph.h"
#include "RandomTools.h"


/**
 * 简单的局部搜索
 */
class SimpleLocalSearch {
public:
    /**
     * 对一个图搜索最小颜色数
     * @param graph 图
     * @param randomTools 随机数工具
     * @param kStart 颜色数的初始值
     * @return 能够搜到的最小颜色数
     */
    static int SearchMinColorNum(
            const Graph &graph,
            std::shared_ptr<RandomTools> &randomTools,
            int kStart = -1
    );

public:
    /**
     * 初始化
     * @param graph 图
     * @param colors 点的颜色
     * @param k 颜色的数量
     */
    SimpleLocalSearch(const Graph &graph, const std::vector<int> &colors, int k)
            : graph_(graph), k_(k) {
        colors_ = colors;
    }

    /**
     * 设置随机数工具
     * @param randomTools 随机数工具
     */
    void SetRandomTools(std::shared_ptr<RandomTools> &randomTools) {
        randomTools_ = randomTools;
    }

    /**
     * 搜索；
     * 找到所有的邻域中最好的，如果最好的邻域，也使冲突数增加或不变，退出。
     * 否则，随机抽取一个，最好的邻域，移动到该邻域。
     */
    void Search();

    /// 冲突数
    int conflictNum{};

protected:
    /// 图实例
    const Graph &graph_;
    /// 各个点的颜色。
    std::vector<int> colors_{};
    /// 颜色个数
    int k_{};
    /// 邻接颜色矩阵
    std::vector<std::vector<int>> adjacentColorTable_{};
    std::shared_ptr<RandomTools> randomTools_;


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
