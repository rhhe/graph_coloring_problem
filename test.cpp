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
    auto adjacentColorTable = Algorithm::MakeAdjacentColorTable(graph, colors, k);
    int nConflict = Algorithm::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
    assert(nConflict == graph.nEdge_);
}

void TestConflictCount2() {
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    graph.PreprocessMultiModeDataStruct();
    int k = 1;
    int seed = 1999;
    auto colors = Algorithm::ColorRandomly(graph, k, seed);
    auto adjacentColorTable = Algorithm::MakeAdjacentColorTable(graph, colors, k);
    int nConflict = Algorithm::CountConflictWithGraph(colors, graph);
    assert(nConflict == graph.nEdge_);
}

void TestCalculatingDeltaConflictNum() {
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    graph.PreprocessMultiModeDataStruct();
    int k = 5;
    int seed = 1999;
    auto colors = Algorithm::ColorRandomly(graph, k, seed);
    auto adjacentColorTable = Algorithm::MakeAdjacentColorTable(graph, colors, k);
    int nConflict = Algorithm::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
    int iNode = 20;
    int oldColor = colors.at(iNode);
    int newColor = (oldColor + 1) % k;
    int deltaConflictNum = Algorithm::CalculateDeltaConflictNum(adjacentColorTable, iNode, oldColor, newColor);
    colors.at(iNode) = newColor;
    adjacentColorTable = Algorithm::MakeAdjacentColorTable(graph, colors, k);
    int nConflictNew = Algorithm::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
    assert(nConflictNew - nConflict == deltaConflictNum);
}

void TestUpdateAdjacentColorTable() {
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    graph.PreprocessMultiModeDataStruct();
    int k = 5;
    int seed = 1999;
    auto colors = Algorithm::ColorRandomly(graph, k, seed);
    auto adjacentColorTable = Algorithm::MakeAdjacentColorTable(graph, colors, k);
    int iNode = 20;
    int oldColor = colors.at(iNode);
    int newColor = (oldColor + 1) % k;
    colors.at(iNode) = newColor;
    auto adjacentColorTable2 = Algorithm::MakeAdjacentColorTable(graph, colors, k);
    Algorithm::UpdateAdjacentColorTable(adjacentColorTable, graph, iNode, oldColor, newColor);
    for (int iRow = 0; iRow < graph.nNode_; ++iRow) {
        for (int iCol = 0; iCol < k; ++iCol) {
            assert(adjacentColorTable.at(iRow).at(iCol) == adjacentColorTable2.at(iRow).at(iCol));
        }
    }
}

int main() {
    TestRandomColor();
    TestConflictCount();
    TestConflictCount2();
    TestCalculatingDeltaConflictNum();
    TestUpdateAdjacentColorTable();
    return 0;
}
