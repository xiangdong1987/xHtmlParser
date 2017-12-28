cmake_minimum_required(VERSION 3.5)
project(xHtmlParser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES  Node.cpp Tool.cpp Tree.cpp main.cpp)
add_executable(xHtmlParser ${SOURCE_FILES})