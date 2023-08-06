//
// Created by USER_NAME: Ron on 2023/8/7.
//

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
