#include <random>
#include <iostream>

#include "SimpleLocalSearch.h"
#include "AlgorithmTools.h"

//#define _PRINT


void SimpleLocalSearch::Search() {
    int obj = AlgorithmTools::CountConflictWithGraph(colors_, graph_);
#ifdef _PRINT
    std::cout << "init" << ", obj: " << obj << std::endl;
#endif
    adjacentColorTable_ = AlgorithmTools::MakeAdjacentColorTable(graph_, colors_, k_);
    std::vector<std::pair<int, int>> bestNeighbors{};
    int bestDeltaObj = 0;
    int iStep = 0;
    while (true) {
        FindBestNeighbor(bestNeighbors, bestDeltaObj);
        if (bestDeltaObj >= 0) { break; }
        int iNeighbor = static_cast<int>(randomTools_->distribution_(*randomTools_->mt_) *
                                         static_cast<double>(bestNeighbors.size()))
                        % static_cast<int>(bestNeighbors.size());
        const auto &neighbor = bestNeighbors.at(iNeighbor);
        const auto &iNode = neighbor.first;
        const auto &oldColor = colors_.at(iNode);
        const auto &newColor = neighbor.second;
        AlgorithmTools::UpdateAdjacentColorTable(adjacentColorTable_, graph_, iNode, oldColor, newColor);
        colors_.at(iNode) = newColor;
        obj += bestDeltaObj;
#ifdef _PRINT
        std::cout << "iStep: " << iStep << ", obj: " << obj << std::endl;
#endif
        iStep++;
    }
    int objDoubleCheck = AlgorithmTools::CountConflictWithGraph(colors_, graph_);
#ifdef _PRINT
    std::cout << "objDoubleCheck: " << objDoubleCheck << std::endl;
#endif
    if (objDoubleCheck != obj) {
        throw std::runtime_error("objDoubleCheck!= obj, " + std::to_string(objDoubleCheck) + "," + std::to_string(obj));
    }
    conflictNum = obj;
}

void SimpleLocalSearch::FindBestNeighbor(
        std::vector<std::pair<int, int>> &bestNeighbors,
        int &bestDeltaObj
) {
    bestNeighbors.reserve((k_ - 1) * graph_.nNode_);
    bestNeighbors.clear();
    bestDeltaObj = graph_.nNode_;
    for (int iNode = 0; iNode < graph_.nNode_; ++iNode) {
        const auto &oldColor = colors_.at(iNode);
        const auto &adjacentColorRow = adjacentColorTable_.at(iNode);
        const auto &oldCellValue = adjacentColorRow.at(oldColor);
        for (int newColor = 0; newColor < k_; ++newColor) {
            if (newColor == oldColor) { continue; }
            int deltaObj = adjacentColorRow.at(newColor) - oldCellValue;
            if (deltaObj > bestDeltaObj) { continue; }
            if (deltaObj < bestDeltaObj) {
                bestNeighbors.clear();
                bestDeltaObj = deltaObj;
            }
            bestNeighbors.emplace_back(iNode, newColor);
        }
    }
}

int SimpleLocalSearch::SearchMinColorNum(
        const Graph &graph,
        std::shared_ptr<RandomTools> &randomTools,
        int kStart
) {
    if (graph.nNode_ == 0) { return 0; }
    if (kStart < 0) { kStart = graph.nNode_; }
    int kMin = kStart;
    for (int k = kStart; k > 0; --k) {
        auto colors = AlgorithmTools::ColorRandomly(graph, k, randomTools);
        SimpleLocalSearch simpleLocalSearch = SimpleLocalSearch(graph, colors, k);
        simpleLocalSearch.SetRandomTools(randomTools);
        simpleLocalSearch.Search();
        std::cout << "k: " << k << ", conflictNum: " << simpleLocalSearch.conflictNum << std::endl;
        if (simpleLocalSearch.conflictNum == 0) {
            kMin = k;
        } else { break; }
    }
    return kMin;
}
