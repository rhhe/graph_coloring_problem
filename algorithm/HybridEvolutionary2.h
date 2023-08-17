#ifndef GRAPH_COLORING_PROBLEM_HYBRIDEVOLUTIONARY2_H
#define GRAPH_COLORING_PROBLEM_HYBRIDEVOLUTIONARY2_H


#include "Graph.h"
#include "RandomTools.h"

class HybridEvolutionary2 {
public:
    /**
     * 构造函数
     * @param graph 图
     * @param k 颜色数
     */
    HybridEvolutionary2(const Graph &graph, int k) : graph_(graph), k_(k) {};

    virtual ~HybridEvolutionary2() = default;

    /**
     * 搜索
     */
    void Search();

public:
    // 输入
    std::shared_ptr<RandomTools> randomTools_; // 随机数工具
    int timeLimitSeconds_ = 300; // 总时间上限
    int cyclesUpperBoundary_ = 100000; // 轮数上线
    int generationsUpperBoundary_ = 15; // 每轮的遗传的迭代步数
    int cyclesEarlyStop_ = 100000; // 轮数迭代早停步数
    int cyclesEarlyRandomElite_ = 20; // 最优解多少步没有更新则随机化精英解。
    int cyclesEarlyRandomParent_ = 150; // 最优解多少步没有更新则随机化亲代解。
    int tabuStepsEarlyStop_ = 200; // 禁忌搜索早停步数
    // 输出
    std::vector<int> bestColors_{}; // 最好的个体
    int bestConflictNum_ = -1; // 最好的个体的冲突数
protected:
    const int populationSize_ = 2; // 种群大小，固定为2
    const Graph &graph_; // 图
    const int k_{}; // 颜色个数
    std::vector<std::vector<int>> colorsListA_{}; // 一对个体，可能作为亲代或者子代
    std::vector<std::vector<int>> colorsListB_{}; // 一对个体，可能作为子代或者亲代
    std::vector<int> conflictNumListA_{}; // colorsPairA 对应的冲突个数
    std::vector<int> conflictNumListB_{}; // colorsPairB 对应的冲突个数
    std::vector<std::vector<int>> *pParents_{}; // 亲代
    std::vector<std::vector<int>> *pChildren_{}; // 子代
    std::vector<int> *pConflictNumParents_{}; // 亲代对应的冲突个数
    std::vector<int> *pConflictNumChildren_{}; // 子代对应的冲突个数
    std::vector<int> eliteA_{}; // 精英个体
    std::vector<int> eliteB_{}; // 精英个体
    int conflictNumEliteA_{}; // 精英个体的冲突数
    int conflictNumEliteB_{}; // 精英个体的冲突数
    std::vector<int> *pElite1_{}; // 精英个体指针
    std::vector<int> *pElite2_{}; // 精英个体指针
    int *pConflictNumElite1_{}; // 精英个体的冲突数指针
    int *pConflictNumElite2_{}; // 精英个体的冲突数指针
    int bestCycle_ = -1; // 找到最优解的轮数
    decltype(std::chrono::system_clock::now()) timeStart_;
    decltype(std::chrono::system_clock::now()) timeEnd_;
    std::vector<std::vector<int>> tmpParentColorNodes_{}; // 临时变量，用于提前申请交叉算子中亲代A个体各个颜色的点的id
    std::vector<int> tmpParentNodesRest_{}; // 临时变量，用于存储交叉算子中未被挑选的。
    std::vector<int> tmpMostColorIds_{}; // 临时变量，用于存储点最多的颜色。
    bool randomMostColorSelection = false; // 是否随机选择最多的颜色，当有多个颜色同时最多的时候生效。
protected:
    /**
     * 预处理，生成中间变量等。
     */
    void Preprocess();

    /**
     * 局部搜索
     * @param colors 个体
     * @param conflictNum 冲突数
     */
    int LocalSearch(std::vector<int> &colors);

    /**
     * 如果找到了没有冲突的绝对最优解，存储最优解，并返回True。
     * @param colors 个体
     * @param conflictNum 冲突数
     * @param iCycle 当前的迭代轮数
     * @return 是否找到了无冲突的染色
     */
    bool FoundAndSaveBestSolution(const std::vector<int> &colors, int conflictNum, int iCycle = -1);

    /**
     * 重组（交叉变异），生成一个新个体。
     */
    void CrossOver(const std::vector<int> &parentA, const std::vector<int> &parentB,
                   std::vector<int> &child);

    /**
     * 刷新最优解
     * @param colors 新生成的解
     * @param conflictNum 新生成的解的冲突数
     * @param iCycle 当前迭代步数
     */
    void RefreshBest(std::vector<int> &colors, int conflictNum, int iCycle);

    /**
     * 是否应该停止
     * @param iCycle 当前迭代次数
     * @return 是否应该停止
     */
    bool ShouldBeStopped(int iCycle);
};


#endif //GRAPH_COLORING_PROBLEM_HYBRIDEVOLUTIONARY2_H
