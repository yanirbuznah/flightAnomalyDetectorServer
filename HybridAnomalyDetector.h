//
// Created by yanir on 29/12/2020.
//

#ifndef EX4_HYBRIDANOMALYDETECTOR_H
#define EX4_HYBRIDANOMALYDETECTOR_H


#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <vector>
class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

};

#endif //EX4_HYBRIDANOMALYDETECTOR_H
