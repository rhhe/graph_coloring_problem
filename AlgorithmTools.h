#ifndef GRAPH_COLORING_PROBLEM_ALGORITHMTOOLS_H
#define GRAPH_COLORING_PROBLEM_ALGORITHMTOOLS_H

#include "Graph.h"

class AlgorithmTools {
public:
    /**
     * 随机生成每个点的颜色
     * @param graph 图
     * @param k 颜色数
     * @param seed 随机种子
     * @return 数组，每个点的颜色值，用一个0到k-1之间的整数表示
     */
    static std::vector<int> ColorRandomly(const Graph &graph, int k, int seed);

    /**
     * 生成邻接矩阵（仇人表）
     * @param graph 图类实例
     * @param colors 各个点的颜色向量
     * @param k 颜色个数
     * @return 二维矩阵，第一层key是点的index，从0到点数-1，第二层key是颜色的index，从0到k-1，value是邻居中该颜色的点个数
     */
    static std::vector<std::vector<int>> MakeAdjacentColorTable(
            const Graph &graph, const std::vector<int> &colors, int k);

    /**
     * 用邻接颜色表（仇人表）统计所有的冲突数
     * @param colors 各个点的颜色
     * @param adjacentColorTable 仇人表
     * @return 冲突数
     */
    static int CountConflictWithAdjacentColorTable(
            const std::vector<int> &colors, const std::vector<std::vector<int>> &adjacentColorTable);

    /**
     * 用图来统计冲突数
     * @param colors 各个点的颜色
     * @param graph 图实例
     * @return 冲突数
     */
    static int CountConflictWithGraph(const std::vector<int> &colors, const Graph &graph);

    /**
     * 如果一个点的从一个颜色，变成另一个颜色，冲突数的变化是多少。
     * @param adjacentColorTable 邻接颜色表
     * @param iNode 点的id
     * @param oldColor 原来的颜色
     * @param newColor 另一种颜色
     * @return
     */
    static int CalculateDeltaConflictNum(const std::vector<std::vector<int>> &adjacentColorTable,
                                         int iNode, int oldColor, int newColor);

    /**
     * 更新邻接颜色表（仇人表）
     * @param adjacentColorTable 邻接颜色表
     * @param iNode 点的id
     * @param oldColor 原来的颜色
     * @param newColor 新的颜色
     */
    static void UpdateAdjacentColorTable(std::vector<std::vector<int>> &adjacentColorTable,
                                         const Graph &graph, int iNode, int oldColor, int newColor);
};

#endif //GRAPH_COLORING_PROBLEM_ALGORITHMTOOLS_H
