#include <iostream>
#include "Graph.h"
#include "AlgorithmTools.h"
#include "SimpleLocalSearch.h"

using namespace std;

int main() {
    std::cout << "hello world!!!" << std::endl;
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    graph.PreprocessMultiModeDataStruct();
    for (int k = 100; k > 0; --k) {
        int seed = 1999 + k;
        auto colors = AlgorithmTools::ColorRandomly(graph, k, seed);
        SimpleLocalSearch simpleLocalSearch(graph, colors, k);
        simpleLocalSearch.Search();
        std::cout << "k:" << k
                  << ", simpleLocalSearch.conflictNum:" << simpleLocalSearch.conflictNum
                  << std::endl;
        if (simpleLocalSearch.conflictNum > 0) {
            break;
        }
    }
    return 0;
}
