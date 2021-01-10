cmake_minimum_required(VERSION 3.17)
project(ex6)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_CXX_STANDARD 11)

add_executable(ex6 main.cpp Server.cpp CLI.cpp timeseries.cpp HybridAnomalyDetector.cpp SimpleAnomalyDetector.cpp
        minCircle.cpp commands.h anomaly_detection_util.cpp StandardIO.cpp)