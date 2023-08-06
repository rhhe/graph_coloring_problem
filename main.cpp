#include <iostream>
#include <vector>
#include <map>
#include <vector>
#include "Graph.h"

using namespace std;

int main() {
    std::cout << "hello world!!!" << std::endl;
    Graph graph;
    graph.ReadFromFile("../DSJC500.5.col");
    std::cout << graph.ToString(false) << std::endl;
    return 0;
}