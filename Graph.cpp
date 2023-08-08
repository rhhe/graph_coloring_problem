#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Graph.h"


void Graph::ReadFromFile(const char *fileName) {
    std::ifstream inStream(fileName);
    if (!inStream) {
        throw std::runtime_error("open file failed, file name: " + std::string(fileName));
    }
    char line_head_char = ' ';
    int nodeI = -1;
    int nodeJ = -1;
    std::string tmp;
    while (!inStream.eof()) {
        inStream >> line_head_char;
        if (inStream.eof()) { break; }
        if (line_head_char == '\n') {
            getline(inStream, tmp);
            continue;
        }
        if (line_head_char == 'c') {
            getline(inStream, tmp);
            continue;
        }
        if (line_head_char == 'p') {
            inStream >> tmp >> this->nNode_ >> this->nEdge_;
            this->nEdge_ /= 2;
            this->edges_.reserve(this->nEdge_);
            getline(inStream, tmp);
            continue;
        }
        if (line_head_char == 'e') {
            inStream >> nodeI >> nodeJ;
            getline(inStream, tmp);
            this->edges_.emplace_back(nodeI - 1, nodeJ - 1);
            continue;
        }
    }
    if (int(this->edges_.size()) != this->nEdge_) {
        throw std::runtime_error(
                "int(this->edges_.size()) != this->nEdge_, "
                + std::to_string(this->edges_.size()) + "!="
                + std::to_string(this->nEdge_)
        );
    }
    inStream.close();
}


void Graph::PreprocessMultiModeDataStruct() {
    adjacencyMatrix_.clear();
    adjacencyMatrix_.resize(nNode_, std::vector<int>(nNode_, 0));
    for (const auto &edge : edges_) {
        adjacencyMatrix_.at(edge.first).at(edge.second) = 1;
        adjacencyMatrix_.at(edge.second).at(edge.first) = 1;
    }

    neighborLists_.resize(nNode_, std::vector<int>());
    for (auto &neighborList: neighborLists_) {
        neighborList.reserve(nNode_);
    }
    for (const auto &edge: edges_) {
        neighborLists_.at(edge.first).push_back(edge.second);
        neighborLists_.at(edge.second).push_back(edge.first);
    }

    neighborSets_.resize(nNode_, std::unordered_set<int>());
    for (const auto &edge: edges_) {
        neighborSets_.at(edge.first).insert(edge.second);
        neighborSets_.at(edge.second).insert(edge.first);
    }
}


std::string Graph::ToString(bool isIdFrom0) const {
    std::stringstream ss;
    ss << "nNode=" << this->nNode_ << ";\n"
       << "nEdge=" << this->nEdge_ << ";\n";
    if (isIdFrom0) {
        for (const auto &edge: this->edges_) {
            ss << edge.first << " " << edge.second << "\n";
        }
    } else {
        for (const auto &edge: this->edges_) {
            ss << edge.first + 1 << " " << edge.second + 1 << "\n";
        }
    }
    return ss.str();
}
