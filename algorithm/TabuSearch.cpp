#include <iostream>
#include "TabuSearch.h"
#include "AlgorithmTools.h"

void TabuSearch::Search() {
    timeStart = std::chrono::system_clock::now();
    Initialize();
    std::vector<Neighbor> neighborsAbsolute{};
    std::vector<Neighbor> neighborsNonTabu{};
    const int DELTA_NOT_FOUND = graph_.nNode_ + 1;
    for (int iStep = 0; iStep < stepUpperBound_; ++iStep) {
        int deltaAbsolute = DELTA_NOT_FOUND;
        int deltaNoTabu = DELTA_NOT_FOUND;
        FindBestNeighbor(neighborsAbsolute, deltaAbsolute, neighborsNonTabu, deltaNoTabu, iStep);
        Neighbor &neighbor =
                (IsShouldBeResolved(deltaAbsolute, deltaNoTabu) || neighborsNonTabu.empty()) ?
                neighborsAbsolute.at(randomTools_->SelectUniformRandomly(neighborsAbsolute.size())) :
                neighborsNonTabu.at(randomTools_->SelectUniformRandomly(neighborsNonTabu.size()));
        AlgorithmTools::UpdateAdjacentColorTable(
                adjacentColorTable_, graph_, neighbor.iNode_, neighbor.oldColor_, neighbor.newColor_);
        Move(neighbor);
        RefreshBestSolution(neighbor, iStep);
        RefreshTabuTable(neighbor, iStep);
//        std::cout << "iStep: " << iStep << ", bestConflictNum: " << bestConflictNum_ << std::endl;
        if (IsShouldBeStopped(iStep)) { break; }
    }
    timeEnd = std::chrono::system_clock::now();
}

/**
 * 初始化，包括：
 * （1）检查初始颜色是否为空，如果为空，生成初始颜色。
 * （2）初始化邻接颜色矩阵。
 * （3）计算初始冲突数。
 * （4）初始化相对最优解。
 * （5）初始化相对最优解优度。
 * （6）初始化禁忌表。
 * （7）初始化均匀分布。
 */
void TabuSearch::Initialize() {
    // 检查初始颜色是否为空，如果为空，生成初始颜色。
    if (colors_.empty()) {
        colors_ = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
    }

    // 初始化邻接颜色矩阵。
    adjacentColorTable_ = AlgorithmTools::MakeAdjacentColorTable(graph_, colors_, k_);

    // 计算初始冲突数。
    conflictNum_ = AlgorithmTools::CountConflictWithAdjacentColorTable(colors_, adjacentColorTable_);

    // 初始化相对最优解。
    bestColors_ = colors_;

    // 初始化相对最优解优度。
    bestConflictNum_ = conflictNum_;

    // 初始化禁忌表。
    tabuTable_.clear();
    static const int DEFAULT_TABU_STEP = -1;
    tabuTable_.resize(graph_.nNode_, std::vector<int>(k_, DEFAULT_TABU_STEP));
}

void TabuSearch::FindBestNeighbor(
        std::vector<Neighbor> &bestNeighborsAbsolute, int &bestDeltaConflictedNumAbsolute,
        std::vector<Neighbor> &bestNeighborsNonTabu, int &bestDeltaConflictedNumNonTabu,
        int iStep
) {
    bestNeighborsAbsolute.clear();
    bestNeighborsNonTabu.clear();
    bestNeighborsAbsolute.reserve(graph_.nNode_ * k_);
    bestNeighborsNonTabu.reserve(graph_.nNode_ * k_);
    bestDeltaConflictedNumAbsolute = graph_.nEdge_;
    bestDeltaConflictedNumNonTabu = graph_.nEdge_;

    for (int iNode = 0; iNode < graph_.nNode_; ++iNode) {
        int oldColor = colors_.at(iNode);
        const auto &adjacentColorRow = adjacentColorTable_.at(iNode);
        int oldCellValue = adjacentColorRow.at(oldColor);
        const auto &tabuRow = tabuTable_.at(iNode);
        for (int newColor = 0; newColor < k_; ++newColor) {
            if (newColor == oldColor) { continue; }
            int deltaObj = adjacentColorRow.at(newColor) - oldCellValue;

            if (deltaObj > bestDeltaConflictedNumNonTabu) { continue; }
            if (iStep >= tabuRow.at(newColor)) {
                if (deltaObj < bestDeltaConflictedNumNonTabu) {
                    bestNeighborsNonTabu.clear();
                    bestDeltaConflictedNumNonTabu = deltaObj;
                }
                bestNeighborsAbsolute.emplace_back(iNode, oldColor, newColor, deltaObj);
            }

            if (deltaObj > bestDeltaConflictedNumAbsolute) { continue; }
            if (deltaObj < bestDeltaConflictedNumAbsolute) {
                bestNeighborsAbsolute.clear();
                bestDeltaConflictedNumAbsolute = deltaObj;
            }
            bestNeighborsAbsolute.emplace_back(iNode, oldColor, newColor, deltaObj);
        }
    }
}

bool TabuSearch::IsShouldBeResolved(int deltaConflictedNumAbsolute, int deltaConflictedNumNonTabu) const {
    if (deltaConflictedNumAbsolute >= deltaConflictedNumNonTabu) { return false; }
    if (deltaConflictedNumAbsolute + conflictNum_ >= bestConflictNum_) { return false; }
    return true;
}

void TabuSearch::Move(const Neighbor &neighbor) {
    // 刷新当前解
    conflictNum_ += neighbor.deltaConflictNum_;
    colors_.at(neighbor.iNode_) = neighbor.newColor_;
}

void TabuSearch::RefreshBestSolution(const Neighbor &neighbor, int iStep) {
    if (conflictNum_ < bestConflictNum_) {
        bestConflictNum_ = conflictNum_;
        bestColors_ = colors_;
        bestStep_ = iStep;
    }
}

void TabuSearch::RefreshTabuTable(const Neighbor &neighbor, int iStep) {
    static const int RANDOM_RANGE_SIZE = 10;
    tabuTable_.at(neighbor.iNode_).at(neighbor.oldColor_) =
            iStep + conflictNum_ +
            static_cast<int>(randomTools_->distribution_(*randomTools_->mt_) * RANDOM_RANGE_SIZE);
}

bool TabuSearch::IsShouldBeStopped(int iStep) {
    timeEnd = std::chrono::system_clock::now();
    if (bestConflictNum_ == 0) { return true; }
    if (iStep - bestStep_ >= stepEarlyStop_) { return true; }
    auto secondsCost = std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();
    if (secondsCost >= timeLimitSeconds_) { return true; }
    return false;
}

