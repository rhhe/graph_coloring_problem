cmake_minimum_required(VERSION 3.15)
project(graph_coloring_problem)

set(CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp Graph.cpp Graph.h)
add_executable(asc2bin data_process/asc2bin.c)
add_executable(bin2asc data_process/bin2asc.c)
add_executable(showpreamble data_process/showpreamble.c)