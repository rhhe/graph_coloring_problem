#include <iostream>
#include "Graph.h"
#include "algorithm.h"

using namespace std;

void TestRandomColor() {
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    graph.PreprocessMultiModeDataStruct();
    int k = 10;
    int seed = 1999;
    auto colors = Algorithm::ColorRandomly(graph, k, seed);
    std::set<int> colorSet(colors.begin(), colors.end());
    std::vector<int> allColor;
    allColor.assign(colorSet.begin(), colorSet.end());
    assert(allColor.size() == k);
}

void TestConflictCount() {
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    graph.PreprocessMultiModeDataStruct();
    int k = 1;
    int seed = 1999;
    auto colors = Algorithm::ColorRandomly(graph, k, seed);
    auto adjacentColorTable = Algorithm::MakeAdjacentColorCountTable(graph, colors, k);
    int nConflict = Algorithm::CountConflictNumWithAdjacentTable(colors, adjacentColorTable);
    assert(nConflict == graph.nEdge_);
}

int main() {
    TestRandomColor();
    TestConflictCount();
    return 0;
}
