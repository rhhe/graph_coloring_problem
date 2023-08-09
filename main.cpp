#include <iostream>
#include "Graph.h"
#include "algorithm.h"

using namespace std;

int main() {
    std::cout << "hello world!!!" << std::endl;
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    std::cout << "nEdge: " << graph.nEdge_ << std::endl;
    graph.PreprocessMultiModeDataStruct();
    int k = 10;
    for (k = 100; k > 0; --k) {
        int seed = 1999;
        auto colors = Algorithm::ColorRandomly(graph, k, seed);
        auto adjacentColorTable = Algorithm::MakeAdjacentColorTable(graph, colors, k);
        int nConflict = Algorithm::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
        std::cout << "k:" << k << ", nConflict: " << nConflict << std::endl;
    }
    return 0;
}
