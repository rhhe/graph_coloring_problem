#include <random>
#include <iostream>

#include "SimpleLocalSearch.h"
#include "AlgorithmTools.h"

/**
 * 搜索；
 * 找到所有的邻域中最好的，如果最好的邻域，也使冲突数增加或不变，退出。
 * 否则，随机抽取一个，最好的邻域，移动到该邻域。
 */
void SimpleLocalSearch::Search() {
    int obj = AlgorithmTools::CountConflictWithGraph(colors_, graph_);
//    std::cout << "init" << ", obj: " << obj << std::endl;

    std::random_device randomDevice;
    std::mt19937 mt(randomDevice());
    int seed = 100;
    mt.seed(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    adjacentColorTable_ = AlgorithmTools::MakeAdjacentColorTable(graph_, colors_, k_);
    std::vector<std::pair<int, int>> bestNeighbors{};
    int bestDeltaObj = 0;
    int iStep = 0;
    while (true) {
        FindBestNeighbor(bestNeighbors, bestDeltaObj);
        if (bestDeltaObj >= 0) { break; }
        int iNeighbor = static_cast<int>(distribution(mt) * static_cast<double>(bestNeighbors.size()))
                        % static_cast<int>(bestNeighbors.size());
        const auto &neighbor = bestNeighbors.at(iNeighbor);
        const auto &iNode = neighbor.first;
        const auto &oldColor = colors_.at(iNode);
        const auto &newColor = neighbor.second;
        AlgorithmTools::UpdateAdjacentColorTable(adjacentColorTable_, graph_, iNode, oldColor, newColor);
        colors_.at(iNode) = newColor;
        obj += bestDeltaObj;
//        std::cout << "iStep: " << iStep << ", obj: " << obj_ << std::endl;
        iStep++;
    }
    int objDoubleCheck = AlgorithmTools::CountConflictWithGraph(colors_, graph_);
//    std::cout << "objDoubleCheck: " << objDoubleCheck << std::endl;
    if (objDoubleCheck != obj) {
        throw std::runtime_error("objDoubleCheck!= obj, " + std::to_string(objDoubleCheck) + "," + std::to_string(obj));
    }
    conflictNum = obj;
}

void SimpleLocalSearch::FindBestNeighbor(std::vector<std::pair<int, int>> &bestNeighbors, int &bestDeltaObj) {
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
