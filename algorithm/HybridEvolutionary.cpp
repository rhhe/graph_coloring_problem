#include <iostream>

#include "HybridEvolutionary.h"
#include "AlgorithmTools.h"
#include "TabuSearch.h"

//#define _PRINT

void HybridEvolutionary::Search() {
    timeStart_ = std::chrono::system_clock::now();
    Preprocess();
    InitializePopulation();
    for (int iGene = 0; iGene < populationSize_; ++iGene) {
        conflictNumList_.at(iGene) = LocalSearch(colorsList_.at(iGene));
        if (conflictNumList_.at(iGene) == 0) {
            bestColors_ = colorsList_.at(iGene);
            bestConflictNum_ = 0;
            return;
        }
    }
    FindBest();
    for (int iStep = 0; iStep < stepsUpperBoundary_; ++iStep) {
        int parentIdA = -1, parentIdB = -1;
        std::tie(parentIdA, parentIdB) = ChooseParent();
        std::vector<int> child = Recombine(colorsList_.at(parentIdA), colorsList_.at(parentIdB));
        int childConflictNum = LocalSearch(child);
        if (childConflictNum == 0) {
            bestColors_ = child;
            bestConflictNum_ = childConflictNum;
            timeEnd_ = std::chrono::system_clock::now();
            return;
        }
        RefreshBest(child, childConflictNum, 0);
        UpdatePopulation(child, childConflictNum);
        if (ShouldBeStopped(iStep)) { break; }
#ifdef _PRINT
        if (!(iStep % 100)) {
            std::cout << "k:" << k_ << ", iStep: " << iStep << ", nConflict: " << bestConflictNum_ << std::endl;
        }
#endif
    }
    timeEnd_ = std::chrono::system_clock::now();
}

void HybridEvolutionary::Preprocess() {
    tmpParentColorNodes_.resize(k_, std::vector<int>{});
    for (auto &nodes: tmpParentColorNodes_) {
        nodes.reserve(graph_.nNode_);
    }
    tmpParentNodesRest_.clear();
    tmpParentNodesRest_.reserve(graph_.nNode_);
}

void HybridEvolutionary::InitializePopulation() {
    colorsList_.clear();
    colorsList_.reserve(populationSize_);
    for (int iGene = 0; iGene < populationSize_; ++iGene) {
        auto colors = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
        colorsList_.push_back(colors);
    }
    conflictNumList_.clear();
    conflictNumList_.resize(populationSize_, -1);
}

int HybridEvolutionary::LocalSearch(std::vector<int> &colors) {
    TabuSearch tabuSearch(graph_, colors, k_);
    tabuSearch.SetRandomTools(randomTools_);
    tabuSearch.Search();
    colors.clear();
    colors.insert(colors.end(), tabuSearch.bestColors_.begin(), tabuSearch.bestColors_.end());
    return tabuSearch.bestConflictNum_;
}

void HybridEvolutionary::FindBest() {
    if (colorsList_.empty()) { throw std::runtime_error("[HEA] empty colors list."); }
    int iGeneBest = static_cast<int>(std::min_element(conflictNumList_.begin(), conflictNumList_.end())
                                     - conflictNumList_.begin());
    bestColors_ = colorsList_.at(iGeneBest);
    bestConflictNum_ = conflictNumList_.at(iGeneBest);
}

std::pair<int, int> HybridEvolutionary::ChooseParent() const {
    int parentA = randomTools_->SelectUniformRandomly(populationSize_);
    int parentB = (randomTools_->SelectUniformRandomly(populationSize_ - 1) + parentA + 1) % populationSize_;
    if (parentA == parentB) {
        throw std::runtime_error("bad choose parent: parentA == parentB = " + std::to_string(parentA));
    }
    return {parentA, parentB};
}

std::vector<int> HybridEvolutionary::Recombine(const std::vector<int> &parentA, const std::vector<int> &parentB) {
    // 初始化子代个体
    std::vector<int> child(graph_.nNode_, 0);

    // 初始化子代中未被指定颜色的点的列表。
    tmpParentNodesRest_.resize(graph_.nNode_);
    std::iota(tmpParentNodesRest_.begin(), tmpParentNodesRest_.end(), 0);

    // 定义列表大小的比较函数，用于挑选被选点集。
    static auto CmpSize = [](const std::vector<int> &a, const std::vector<int> &b) { return a.size() < b.size(); };

    // 前k-1个颜色，会被批量挑选
    int nPickBatch = k_ - 1;
    for (int iPickBatch = 0; iPickBatch < nPickBatch; ++iPickBatch) {
        // 指定亲代个体，交替选择亲代个体A和亲代个体B
        const std::vector<int> &parent = (iPickBatch % 2) ? parentB : parentA;
        // 将亲代个体的各个颜色，放入对应的颜色点集中。
        if (tmpParentNodesRest_.empty()) { break; }
        for (auto &colorNodes: tmpParentColorNodes_) {
            colorNodes.clear();
        }
        for (int iNode: tmpParentNodesRest_) {
            tmpParentColorNodes_.at(parent.at(iNode)).push_back(iNode);
        }
        // 找到最大的颜色点集，并赋予子代个体，颜色为批次id（这里，具体的颜色不重要，所以采用批次个数作为颜色）
        int colorOfMostNodes = static_cast<int>(
                std::max_element(tmpParentColorNodes_.begin(), tmpParentColorNodes_.end(), CmpSize) -
                tmpParentColorNodes_.begin());
        const std::vector<int> &selectedNodes = tmpParentColorNodes_.at(colorOfMostNodes);
        for (const auto &iNode: selectedNodes) {
            child.at(iNode) = iPickBatch;
        }
        // 重新汇总未被选择的点。
        tmpParentNodesRest_.clear();
        for (int color = 0; color < k_; ++color) {
            if (color == colorOfMostNodes) { continue; }
            const auto &unselectedNodes = tmpParentColorNodes_.at(color);
            tmpParentNodesRest_.insert(tmpParentNodesRest_.end(), unselectedNodes.begin(), unselectedNodes.end());
        }
    }
    // 经过k-1轮选择剩下的点，随机分配到各种颜色。因为子代会经历局部搜索，因此，这种随机分配问题不大。
    for (const auto &iNode: tmpParentNodesRest_) {
        child.at(iNode) = randomTools_->SelectUniformRandomly(k_);
    }
    return child;
}

void HybridEvolutionary::RefreshBest(const std::vector<int> &child, int childConflictNum, int iStep) {
    if (childConflictNum >= bestConflictNum_) { return; }
    bestColors_ = child;
    bestConflictNum_ = childConflictNum;
    bestStep_ = iStep;
}

void HybridEvolutionary::UpdatePopulation(const std::vector<int> &child, int childConflictNum) {
    int iWorst = static_cast<int>(std::max_element(conflictNumList_.begin(), conflictNumList_.end())
                                  - conflictNumList_.begin());
    if (childConflictNum > conflictNumList_.at(iWorst)) {
        return;
    }
    colorsList_.at(iWorst) = child;
    conflictNumList_.at(iWorst) = childConflictNum;
}

bool HybridEvolutionary::ShouldBeStopped(int iStep) {
    timeEnd_ = std::chrono::system_clock::now();
    if (bestConflictNum_ == 0) { return true; }
    if (iStep - bestStep_ >= stepsEarlyStop_) { return true; }
    auto secondsCost = std::chrono::duration_cast<std::chrono::seconds>(timeEnd_ - timeStart_).count();
    if (secondsCost >= timeLimitSeconds_) { return true; }
    return false;
}

