//
// Created by yanir on 29/12/2020.
//

#ifndef EX4_CLI_H
#define EX4_CLI_H

#include <map>
#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO* _dio;
    map<string, Command*> _commandMap;
    vector<AnomalyReport> _anomalyReports;
    HybridAnomalyDetector _anomalyDetector;
    // you can add data members
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
    void createMap();
};

#endif //EX4_CLI_H
