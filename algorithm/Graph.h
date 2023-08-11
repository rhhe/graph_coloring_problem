#ifndef GRAPH_COLORING_PROBLEM_GRAPH_H
#define GRAPH_COLORING_PROBLEM_GRAPH_H

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <set>
#include <unordered_set>

/**
 * 图类，包含读取数据集，和多种图的表示方式。
 */
class Graph {
public:
    /// 点的个数
    int nNode_;
    /// 边的个数
    int nEdge_;
    /// 边的列表
    std::vector<std::pair<int, int>> edges_{};
    /// 邻接矩阵的图表示
    std::vector<std::vector<int>> adjacencyMatrix_{};
    /// 各个点的邻接点集合的图表示
    std::vector<std::unordered_set<int>> neighborSets_{};
    /// 各个点的邻接点列表的图表示
    std::vector<std::vector<int>> neighborLists_{};
public:
    /**
     * 读取数据集文件
     * @param fileName
     */
    void ReadFromFile(const char *fileName);

    /**
     * 预处理，生成图的多种数据结构
     */
    void PreprocessMultiModeDataStruct();

    /**
     * 转化为字符串
     * @param isIdFrom0 点的id是否从0开始。
     * @return 字符串
     */
    std::string ToString(bool isIdFrom0 = false) const;
};


#endif //GRAPH_COLORING_PROBLEM_GRAPH_H
