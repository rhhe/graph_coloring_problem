cmake_minimum_required(VERSION 3.15)
project(graph_coloring_problem)

set(CMAKE_CXX_STANDARD 14)

set(algorithm_files
        algorithm/Graph.h
        algorithm/Graph.cpp
        algorithm/AlgorithmTools.h
        algorithm/AlgorithmTools.cpp
        algorithm/SimpleLocalSearch.h
        algorithm/SimpleLocalSearch.cpp
        algorithm/RandomTools.h algorithm/TabuSearch.cpp algorithm/TabuSearch.h)

add_executable(main main.cpp ${algorithm_files})
add_executable(test test.cpp ${algorithm_files})
add_executable(asc2bin data_process/asc2bin.c)
add_executable(bin2asc data_process/bin2asc.c)
add_executable(showpreamble data_process/showpreamble.c)