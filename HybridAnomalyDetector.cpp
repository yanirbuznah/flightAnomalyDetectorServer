
#include "HybridAnomalyDetector.h"
#include <vector>

HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {

}
vector<AnomalyReport>  HybridAnomalyDetector ::  detect(const TimeSeries& ts){
    vector<AnomalyReport> anomalyReports = SimpleAnomalyDetector :: detect(ts);
    int feature1Index, feature2Index;
    for (int i = 0; i < this->_cf.size(); ++i) {
        if (this->_cf[i].corrlation < _threshold) {
            for (int j = 0; j < ts.getElementsNames().size(); ++j) {
                if (ts.getElementsNames().at(j) == _cf[i].feature1) {
                    feature1Index = j;
                }
                if (ts.getElementsNames().at(j) == _cf[i].feature2) {
                    feature2Index = j;
                }
            }
            for (int j = 0; j < ts.getRecords().size(); ++j) {
                Point p = Point(ts.getRecords().at(j).at(feature1Index), ts.getRecords().at(j).at(feature2Index));
                if (!pointInsideCircle(p,_cf[i].minCirle)){
                    AnomalyReport ar = AnomalyReport(_cf[i].feature1 + "-" + _cf[i].feature2, j + 1);
                    anomalyReports.push_back(ar);
                }
            }
        }

    }
    return anomalyReports;
}