#ifndef GRAPH_COLORING_PROBLEM_ALGORITHM_H
#define GRAPH_COLORING_PROBLEM_ALGORITHM_H

#include "Graph.h"

class Algorithm {
public:
    static std::vector<int> ColorRandomly(const Graph &graph, int k, int seed);

    static std::vector<std::vector<int>> MakeAdjacentColorCountTable(
            const Graph &graph, const std::vector<int> &colors, int k);

    static int CountConflictNumWithAdjacentTable(
            const std::vector<int> &colors, const std::vector<std::vector<int>> &adjacentTable);
};


#endif //GRAPH_COLORING_PROBLEM_ALGORITHM_H
