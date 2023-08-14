#ifndef GRAPH_COLORING_PROBLEM_HYBRIDEVOLUTIONARY_H
#define GRAPH_COLORING_PROBLEM_HYBRIDEVOLUTIONARY_H

#include <vector>
#include <utility>
#include <chrono>
#include "RandomTools.h"
#include "Graph.h"


/**
 * 混合进化算法（遗传算法+局部搜索/禁忌搜索）
 */
class HybridEvolutionary {
public:
    /**
     * 构造函数
     * @param graph 图
     * @param k 颜色数
     */
    HybridEvolutionary(const Graph &graph, int k) : graph_(graph), k_(k) {};

    virtual ~HybridEvolutionary() = default;

    /**
     * 搜索
     */
    void Search();

public:
    // 输入
    std::shared_ptr<RandomTools> randomTools_; // 随机数工具
    int populationSize_ = 20; // 种群大小
    int timeLimitSeconds_ = 300; // 总时间上限
    int stepsUpperBoundary_ = 100000; // 遗传的迭代步数上限
    int stepsEarlyStop_ = 100000; // 遗传迭代早停步数
    // 输出
    std::vector<int> bestColors_{}; // 最好的个体
    int bestConflictNum_ = -1; // 最好的个体的冲突数
protected:
    const Graph &graph_; // 图
    const int k_{}; // 颜色个数
    std::vector<std::vector<int>> colorsList_{}; // 各个个体的染色
    std::vector<int> conflictNumList_{}; // 各个个体的冲突数
    int bestStep_ = 0; // 找到最优解的步数
    decltype(std::chrono::system_clock::now()) timeStart_;
    decltype(std::chrono::system_clock::now()) timeEnd_;
    std::vector<std::vector<int>> tmpParentColorNodes_{}; // 临时变量，用于提前申请交叉算子中亲代A个体各个颜色的点的id
    std::vector<int> tmpParentNodesRest_{}; // 临时变量，用于存储交叉算子中未被挑选的。
protected:
    /**
     * 预处理，生成中间变量等。
     */
    void Preprocess();

    /**
     * 初始化种群
     */
    void InitializePopulation();

    /**
     * 局部搜索
     * @param colors 个体
     * @param conflictNum 冲突数
     */
    int LocalSearch(std::vector<int> &colors);

    /**
     * 从种群中找到最好的个体。
     */
    void FindBest();

    /**
     * 选择亲代
     * @return 亲代的个体A和个体B在种群中的顺序Id
     */
    std::pair<int, int> ChooseParent() const;

    /**
     * 重组（交叉变异）
     * @param parentA 输入：亲代个体A
     * @param parentB 输入：亲代个体B
     * @return child 输出：子代个体
     */
    std::vector<int> Recombine(const std::vector<int> &parentA, const std::vector<int> &parentB);

    /**
     * 刷新最优解
     * @param child 新生成的解
     * @param childConflictNum 新生成的解的冲突数
     * @param iStep 当前迭代步数
     */
    void RefreshBest(const std::vector<int> &child, int childConflictNum, int iStep);

    /**
     * 更新种群
     * @param child 子代个体
     * @param childConflictNum 子代个体冲突数
     */
    void UpdatePopulation(const std::vector<int> &child, int childConflictNum);

    /**
     * 是否应该停止
     * @param iStep 当前迭代次数
     * @return 是否应该停止
     */
    bool ShouldBeStopped(int iStep);
};


#endif //GRAPH_COLORING_PROBLEM_HYBRIDEVOLUTIONARY_H
