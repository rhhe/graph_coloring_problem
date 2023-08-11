#include <iostream>
#include "algorithm/Graph.h"
#include "algorithm/AlgorithmTools.h"
#include "algorithm/RandomTools.h"
#include "algorithm/SimpleLocalSearch.h"
#include "algorithm/TabuSearch.h"

using namespace std;

Graph MakeGraph(const char *fileName = "../data_set/DSJC500.5.col") {
    Graph graph;
    graph.ReadFromFile(fileName);
    graph.PreprocessMultiModeDataStruct();
    return graph;
}

void TestRandomColor() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools());
    int k = 10;
    auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
    std::set<int> colorSet(colors.begin(), colors.end());
    std::vector<int> allColor;
    allColor.assign(colorSet.begin(), colorSet.end());
    assert(allColor.size() == k);
}

void TestConflictCount() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools());
    int k = 1;
    auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
    auto adjacentColorTable = AlgorithmTools::MakeAdjacentColorTable(graph, colors, k);
    int nConflict = AlgorithmTools::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
    assert(nConflict == graph.nEdge_);
}

void TestConflictCount2() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools());
    int k = 1;
    auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
    auto adjacentColorTable = AlgorithmTools::MakeAdjacentColorTable(graph, colors, k);
    int nConflict = AlgorithmTools::CountConflictWithGraph(colors, graph);
    assert(nConflict == graph.nEdge_);
}

void TestCalculatingDeltaConflictNum() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools());

    int k = 5;
    auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
    auto adjacentColorTable = AlgorithmTools::MakeAdjacentColorTable(graph, colors, k);
    int nConflict = AlgorithmTools::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
    int iNode = 20;
    int oldColor = colors.at(iNode);
    int newColor = (oldColor + 1) % k;
    int deltaConflictNum = AlgorithmTools::CalculateDeltaConflictNum(adjacentColorTable, iNode, oldColor, newColor);
    colors.at(iNode) = newColor;
    adjacentColorTable = AlgorithmTools::MakeAdjacentColorTable(graph, colors, k);
    int nConflictNew = AlgorithmTools::CountConflictWithAdjacentColorTable(colors, adjacentColorTable);
    assert(nConflictNew - nConflict == deltaConflictNum);
}

void TestUpdateAdjacentColorTable() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools());

    int k = 5;
    auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
    auto adjacentColorTable = AlgorithmTools::MakeAdjacentColorTable(graph, colors, k);
    int iNode = 20;
    int oldColor = colors.at(iNode);
    int newColor = (oldColor + 1) % k;
    colors.at(iNode) = newColor;
    auto adjacentColorTable2 = AlgorithmTools::MakeAdjacentColorTable(graph, colors, k);
    AlgorithmTools::UpdateAdjacentColorTable(adjacentColorTable, graph, iNode, oldColor, newColor);
    for (int iRow = 0; iRow < graph.nNode_; ++iRow) {
        for (int iCol = 0; iCol < k; ++iCol) {
            assert(adjacentColorTable.at(iRow).at(iCol) == adjacentColorTable2.at(iRow).at(iCol));
        }
    }
}

void TestSimpleLocalSearch() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools(12111));
    int kStart = 100;
    int kMin = SimpleLocalSearch::SearchMinColorNum(graph, randomTools, kStart);
    std::cout << "SimpleLocalSearch: kMin=" << kMin << std::endl;
}

void TestTabuSearch() {
    auto graph = MakeGraph();
    std::shared_ptr<RandomTools> randomTools(new RandomTools(12111));
    int kStart = 100;
    int kMin = kStart;
    for (int k = kStart; k > 0; k--) {
        auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
        TabuSearch tabuSearch(graph, colors, k);
        tabuSearch.SetRandomTools(randomTools);
        tabuSearch.Search();
        std::cout << "k: " << k << ", conflictNum: " << tabuSearch.bestConflictNum_ << std::endl;
        if (tabuSearch.bestConflictNum_ == 0) {
            kMin = k;
        } else { break; }
    }
    std::cout << "TabuSearch: kMin=" << kMin << std::endl;
}

int main() {
    TestRandomColor();
    TestConflictCount();
    TestConflictCount2();
    TestCalculatingDeltaConflictNum();
    TestUpdateAdjacentColorTable();
    TestSimpleLocalSearch();
    TestTabuSearch();
    return 0;
}
