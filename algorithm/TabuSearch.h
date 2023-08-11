#ifndef GRAPH_COLORING_PROBLEM_TABUSEARCH_H
#define GRAPH_COLORING_PROBLEM_TABUSEARCH_H

#include <chrono>
#include <vector>
#include "Graph.h"
#include "RandomTools.h"


/**
 * 邻域动作
 */
class Neighbor {
public:
    Neighbor() = default;

    Neighbor(int iNode, int oldColor, int newColor, int deltaConflictNum) :
            iNode_(iNode),
            oldColor_(oldColor),
            newColor_(newColor),
            deltaConflictNum_(deltaConflictNum) {};

    int iNode_; // 点的id
    int oldColor_; // 原始颜色
    int newColor_; // 新的颜色
    int deltaConflictNum_; // 冲突数的变化
};

/**
 * 禁忌搜索
 */
class TabuSearch {
public:
    /**
     * 构造函数
     * @param graph 图
     * @param colors 初始染色
     * @param k 颜色个数
     */
    TabuSearch(const Graph &graph, const std::vector<int> &colors, int k) : graph_(graph), k_(k) {
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
     * 搜索，目的是最小化冲突数。
     */
    void Search();

    /// 能够找到的最小冲突数
    std::vector<int> bestColors_{};
    int bestConflictNum_{};
public:
    // 高级设置参数
    int stepUpperBound_ = 100000; // 最大步数
    int stepEarlyStop_ = 50; // 早停步数
    int timeLimitSeconds_ = 300; // 时间限制
protected:
    const Graph &graph_; // 图
    std::vector<int> colors_; // 染色
    int conflictNum_{}; // 当前染色的冲突数
    const int k_{}; // 颜色个数
    std::shared_ptr<RandomTools> randomTools_; // 随机数工具
    std::vector<std::vector<int>> tabuTable_{}; // 禁忌表，记录解禁步数
    std::vector<std::vector<int>> adjacentColorTable_{}; // 邻接颜色矩阵
    decltype(std::chrono::system_clock::now()) timeStart{};
    decltype(std::chrono::system_clock::now()) timeEnd{};
    int bestStep_ = -1;
protected:
    /**
     * 初始化，具体包含的内容在实现的注释中。
     */
    void Initialize();

    /**
     * 找到最好的邻域动作
     * @param bestNeighborsAbsolute 绝对的最好的邻域动作，不考虑是否被禁忌。
     * @param bestDeltaConflictedNumAbsolute 绝对的最好的邻域动作的冲突数变化。
     * @param bestNeighborsNonTabu 没有被禁忌的最好的邻域动作。
     * @param bestDeltaConflictedNumNonTabu 没有被禁忌的最好的邻域动作的冲突数的变化。
     * @param iStep 当前的步数。
     */
    void FindBestNeighbor(
            std::vector<Neighbor> &bestNeighborsAbsolute,
            int &bestDeltaConflictedNumAbsolute,
            std::vector<Neighbor> &bestNeighborsNonTabu,
            int &bestDeltaConflictedNumNonTabu,
            int iStep
    );

    /**
     * 是否应该解禁
     * @param deltaConflictedNumAbsolute 绝对的最好的邻域动作的冲突数变化。
     * @param deltaConflictedNumNonTabu 没有被禁忌的最好的邻域动作的冲突数的变化。
     * @return 是否应该解禁
     */
    bool IsShouldBeResolved(
            int deltaConflictedNumAbsolute,
            int deltaConflictedNumNonTabu
    ) const;

    /**
     * 移动到邻域位置
     * @param neighbor 邻域
     */
    void Move(const Neighbor &neighbor);

    /**
     * 刷新最优解
     * @param neighbor 邻域动作
     * @param iStep 当前步数
     */
    void RefreshBestSolution(const Neighbor &neighbor, int iStep);

    /**
     * 刷新禁忌表
     * @param neighbor 邻域
     * @param iStep 当前步数
     */
    void RefreshTabuTable(const Neighbor &neighbor, int iStep);

    /**
     * 是否停止搜索
     * @param iStep 当前步数
     * @return 是否停止搜索
     */
    bool IsShouldBeStopped(int iStep);
};


#endif //GRAPH_COLORING_PROBLEM_TABUSEARCH_H
