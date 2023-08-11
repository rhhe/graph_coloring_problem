#include "AlgorithmTools.h"
#include "RandomTools.h"

std::vector<int> AlgorithmTools::ColorRandomly(const Graph &graph, int k,
                                               std::shared_ptr<RandomTools> &randomTools) {
    std::uniform_int_distribution<int> distribution(0, k - 1);
    std::vector<int> colors(graph.nNode_);
    for (auto &color: colors) {
        color = distribution(*randomTools->mt_);
    }
    return colors;
}

std::vector<std::vector<int>>
AlgorithmTools::MakeAdjacentColorTable(const Graph &graph, const std::vector<int> &colors, int k) {
    std::vector<std::vector<int>> adjacentTable(colors.size(), std::vector<int>(k, 0));
    for (const auto &edge: graph.edges_) {
        const auto &iColor = colors.at(edge.first);
        const auto &jColor = colors.at(edge.second);
        adjacentTable.at(edge.first).at(jColor)++;
        adjacentTable.at(edge.second).at(iColor)++;
    }
    return adjacentTable;
}

int AlgorithmTools::CountConflictWithAdjacentColorTable(const std::vector<int> &colors,
                                                        const std::vector<std::vector<int>> &adjacentColorTable) {
    int count = 0;
    auto iterRow = adjacentColorTable.begin();
    auto iterColor = colors.begin();
    for (; iterRow != adjacentColorTable.end(); ++iterRow, ++iterColor) {
        count += iterRow->at(*iterColor);
    }
    count /= 2;
    return count;
}

int AlgorithmTools::CountConflictWithGraph(const std::vector<int> &colors, const Graph &graph) {
    int count = 0;
    for (const auto &edge: graph.edges_) {
        count += colors.at(edge.first) == colors.at(edge.second);
    }
    return count;
}

int AlgorithmTools::CalculateDeltaConflictNum(
        const std::vector<std::vector<int>> &adjacentColorTable,
        int iNode, int oldColor, int newColor) {
    const auto &row = adjacentColorTable.at(iNode);
    return row.at(newColor) - row.at(oldColor);
}

void AlgorithmTools::UpdateAdjacentColorTable(std::vector<std::vector<int>> &adjacentColorTable,
                                              const Graph &graph, int iNode, int oldColor, int newColor) {
    const auto &neighbors = graph.neighborLists_.at(iNode);
    for (const int &neighborNodeId: neighbors) {
        auto &rowNeighbor = adjacentColorTable.at(neighborNodeId);
        rowNeighbor.at(oldColor)--;
        rowNeighbor.at(newColor)++;
    }
}
