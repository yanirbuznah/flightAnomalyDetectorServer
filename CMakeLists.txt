cmake_minimum_required(VERSION 3.17)
project(ex6)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex6 main.cpp server.cpp CLI.cpp timeseries.cpp HybridAnomalyDetector.cpp SimpleAnomalyDetector.cpp
        minCircle.cpp commands.h anomaly_detection_util.cpp StandardIO.cpp)