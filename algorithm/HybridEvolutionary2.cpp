#include <iostream>
#include "AlgorithmTools.h"
#include "HybridEvolutionary2.h"
#include "TabuSearch.h"

#define _PRINT

void HybridEvolutionary2::Search() {
    timeStart_ = std::chrono::system_clock::now();
    Preprocess();

    // 初始化种群，初始化相对最优解
    for (auto &colors: *pParents_) {
        colors = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
    }
    bestColors_ = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
    bestConflictNum_ = AlgorithmTools::CountConflictWithGraph(bestColors_, graph_);

    // 种群中的每个个体局部搜索，并找到现对最优解
    for (int iGene = 0; iGene < populationSize_; ++iGene) {
        auto &parent = pParents_->at(iGene);
        auto &conflictNum = pConflictNumParents_->at(iGene);
        conflictNum = LocalSearch(parent);
        static const int CYCLE_DEFAULT = -1;
        RefreshBest(parent, conflictNum, CYCLE_DEFAULT);
        if (bestConflictNum_ == 0) { return; }
    }

    *pElite2_ = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
    *pConflictNumElite2_ = AlgorithmTools::CountConflictWithGraph(*pElite1_, graph_);
    // 主循环
    for (int iCycle = 0; iCycle < cyclesUpperBoundary_; ++iCycle) {
        // 初始化引导解
        *pElite1_ = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
        *pConflictNumElite1_ = AlgorithmTools::CountConflictWithGraph(*pElite1_, graph_);
        for (int iGenerate = 0; iGenerate < generationsUpperBoundary_; ++iGenerate) {
            // 产生子代，并更新引导解
            CrossOver(pParents_->at(0), pParents_->at(1), pChildren_->at(0));
            CrossOver(pParents_->at(1), pParents_->at(0), pChildren_->at(1));
            for (int iGene = 0; iGene < populationSize_; ++iGene) {
                auto &child = pChildren_->at(iGene);
                auto &conflictNum = pConflictNumChildren_->at(iGene);
                conflictNum = LocalSearch(child);
                if (FoundAndSaveBestSolution(child, conflictNum, iCycle)) { return; }
                if (conflictNum < *pConflictNumElite1_) {
                    *pElite1_ = child;
                    *pConflictNumElite1_ = conflictNum;
                }
            }
            std::swap(pParents_, pChildren_);
            std::swap(pConflictNumParents_, pConflictNumChildren_);
        }
        RefreshBest(*pElite1_, *pConflictNumElite1_, iCycle);
        if (ShouldBeStopped(iCycle)) { break; }
        pParents_->at(0) = *pElite2_;
        pConflictNumParents_->at(0) = *pConflictNumElite2_;
        std::swap(pElite1_, pElite2_);
        std::swap(pConflictNumElite1_, pConflictNumElite2_);
        if (iCycle > bestCycle_ && ((iCycle - bestCycle_) % cyclesEarlyRandomElite_) == 0) {
            *pElite2_ = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
            *pConflictNumElite2_ = AlgorithmTools::CountConflictWithGraph(*pElite2_, graph_);
        }
        if (iCycle > bestCycle_ && ((iCycle - bestCycle_) % cyclesEarlyRandomParent_) == 0) {
            pParents_->at(1) = AlgorithmTools::ColorRandomly(graph_, k_, randomTools_);
            pConflictNumParents_->at(1) = AlgorithmTools::CountConflictWithGraph(*pElite2_, graph_);
        }
#ifdef _PRINT
        if (!(iCycle % 100)) {
            std::cout << "k:" << k_ << ", iCycle: " << iCycle << ", nConflict: " << bestConflictNum_ << std::endl;
        }
#endif
    }
    timeEnd_ = std::chrono::system_clock::now();
}

void HybridEvolutionary2::Preprocess() {

    // 初始化亲代和子代的类实例
    colorsListA_.clear();
    colorsListA_.resize(populationSize_, std::vector<int>(graph_.nNode_));
    colorsListB_.clear();
    colorsListB_.resize(populationSize_, std::vector<int>(graph_.nNode_));
    conflictNumListA_.clear();
    conflictNumListA_.resize(populationSize_, graph_.nEdge_ + 1);
    conflictNumListB_.clear();
    conflictNumListB_.resize(populationSize_, graph_.nEdge_ + 1);

    // 初始化亲代和子代的指针
    pParents_ = &colorsListA_;
    pChildren_ = &colorsListB_;
    pConflictNumParents_ = &conflictNumListA_;
    pConflictNumChildren_ = &conflictNumListB_;

    // 初始化精英个体、冲突数指针
    pElite1_ = &eliteA_;
    pElite2_ = &eliteB_;
    pConflictNumElite1_ = &conflictNumEliteA_;
    pConflictNumElite2_ = &conflictNumEliteB_;

    // 初始化临时变量
    tmpParentColorNodes_.resize(k_, std::vector<int>{});
    for (auto &nodes: tmpParentColorNodes_) {
        nodes.reserve(graph_.nNode_);
    }
    tmpParentNodesRest_.clear();
    tmpParentNodesRest_.reserve(graph_.nNode_);
    tmpMostColorIds_.clear();
    tmpMostColorIds_.reserve(k_);

    // 初始化亲代和子代的存储空间。
    for (int iGene = 0; iGene < populationSize_; ++iGene) {
        pParents_->at(iGene).clear();
        pParents_->at(iGene).resize(graph_.nNode_);
        pChildren_->at(iGene).clear();
        pChildren_->at(iGene).resize(graph_.nNode_);
    }

    // 初始化引导用的个体的空间
    eliteA_.clear();
    eliteA_.resize(graph_.nNode_);
}

int HybridEvolutionary2::LocalSearch(std::vector<int> &colors) {
    TabuSearch tabuSearch(graph_, colors, k_);
    tabuSearch.stepEarlyStop_ = tabuStepsEarlyStop_;
    tabuSearch.SetRandomTools(randomTools_);
    tabuSearch.Search();
    colors.clear();
    colors.insert(colors.end(), tabuSearch.bestColors_.begin(), tabuSearch.bestColors_.end());
    return tabuSearch.bestConflictNum_;
}

bool HybridEvolutionary2::FoundAndSaveBestSolution(const std::vector<int> &colors, int conflictNum, int iCycle) {
    if (conflictNum > 0) { return false; }
    bestColors_ = colors;
    bestConflictNum_ = conflictNum;
    bestCycle_ = iCycle;
    return true;
}

void HybridEvolutionary2::CrossOver(const std::vector<int> &parentA, const std::vector<int> &parentB,
                                    std::vector<int> &child) {
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
        int colorOfMostNodes = -1;
        if (randomMostColorSelection) {
            tmpMostColorIds_.clear();
            if (k_ > 0) { tmpMostColorIds_.push_back(0); }
            for (int color = 1; color < k_; ++color) {
                const auto &size = tmpParentColorNodes_.at(color).size();
                const auto &oldSize = tmpMostColorIds_.front();
                if (size < oldSize) { continue; }
                if (size > oldSize) { tmpMostColorIds_.clear(); }
                tmpMostColorIds_.push_back(color);
            }
            int iSelected = randomTools_->SelectUniformRandomly(static_cast<int>(tmpMostColorIds_.size()));
            colorOfMostNodes = tmpMostColorIds_.at(iSelected);
        } else {
            colorOfMostNodes = static_cast<int>(
                    std::max_element(tmpParentColorNodes_.begin(), tmpParentColorNodes_.end(), CmpSize)
                    - tmpParentColorNodes_.begin());
        }
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
}

void HybridEvolutionary2::RefreshBest(std::vector<int> &colors, int conflictNum, int iCycle) {
    if (conflictNum >= bestConflictNum_) { return; }
    bestColors_ = colors;
    bestConflictNum_ = conflictNum;
    bestCycle_ = iCycle;
}

bool HybridEvolutionary2::ShouldBeStopped(int iCycle) {
    timeEnd_ = std::chrono::system_clock::now();
    if (bestConflictNum_ == 0) { return true; }
    if (iCycle - bestCycle_ >= cyclesEarlyStop_) { return true; }
    auto secondsCost = std::chrono::duration_cast<std::chrono::seconds>(timeEnd_ - timeStart_).count();
    if (secondsCost >= timeLimitSeconds_) {
        std::cout << "stopped by timeLimit: secondsCost = " << secondsCost << std::endl;
        return true;
    }
    return false;
}
