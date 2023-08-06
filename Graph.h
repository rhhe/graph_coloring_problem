//
// Created by USER_NAME: Ron on 2023/8/7.
//

#ifndef GRAPH_COLORING_PROBLEM_GRAPH_H
#define GRAPH_COLORING_PROBLEM_GRAPH_H

#include <vector>
#include <cstdlib>
#include <cstdio>

class Graph {
public:
    int nNode_;
    int nEdge_;
    std::vector<std::pair<int, int>> edges_{};
public:
    /**
     *
     * @param fileName
     */
    void ReadFromFile(const char *fileName);

    /**
     *
     * @param isIdFrom0
     * @return
     */
    std::string ToString(bool isIdFrom0) const;
};


#endif //GRAPH_COLORING_PROBLEM_GRAPH_H
