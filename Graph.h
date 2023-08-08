#ifndef GRAPH_COLORING_PROBLEM_GRAPH_H
#define GRAPH_COLORING_PROBLEM_GRAPH_H

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <set>
#include <unordered_set>

class Graph {
public:
    int nNode_;
    int nEdge_;
    std::vector<std::pair<int, int>> edges_{};
    std::vector<std::vector<int>> adjacencyMatrix_{};
    std::vector<std::unordered_set<int>> neighborSets_{};
    std::vector<std::vector<int>> neighborLists_{};
public:
    void ReadFromFile(const char *fileName);

    void PreprocessMultiModeDataStruct();

    std::string ToString(bool isIdFrom0 = false) const;

protected:
};


#endif //GRAPH_COLORING_PROBLEM_GRAPH_H
