
#include "SimpleAnomalyDetector.h"
#include <iostream>
#include <vector>


SimpleAnomalyDetector::~SimpleAnomalyDetector() {

}


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    int numOfFeature = ts.numOfElements();
    int numOfFlights = ts.numOfFlights();
    Point **points = nullptr;
    correlatedFeatures cf;

    for (int i = 0; i < numOfFeature; ++i) {
        cf.feature1 = ts.getElementsNames().at(i);
        cf.corrlation = 0;
        float maxOffset = 0;
        for (int j = 0; j < numOfFeature; ++j) {
            if (i != j) {
                string feature2 = ts.getElementsNames().at(j);
                float newCorrlation = fabs(
                        pearson(ts.getSortedRecords().at(i).data(), ts.getSortedRecords().at(j).data(), numOfFlights));

                if (cf.corrlation < newCorrlation) {
                    freeArray(points, numOfFlights);
                    cf.corrlation = newCorrlation;
                    cf.feature2 = feature2;
                    points = recordsToPoints(ts.getSortedRecords().at(i), ts.getSortedRecords().at(j), numOfFlights);
                    cf.lin_reg = linear_reg(points, numOfFlights);
                }
            }
        }
        for (int j = 0; j < numOfFlights; ++j) {
            float offset = fabs(dev(*points[j], cf.lin_reg));
            if (offset > maxOffset) {
                maxOffset = offset;
            }
        }

        cf.maxOffset = 1.1 * maxOffset;
        if(cf.corrlation > 0.5){
            if(cf.corrlation < _threshold){
                cf.minCirle = findMinCircle(points,numOfFlights);
                cf.minCirle.radius *= 1.1;
            }
            _cf.push_back(cf);
        }
    }
    for (int i = 0; i < _cf.size(); ++i) {
        for (int j = 0; j < _cf.size(); ++j) {
            if (_cf.at(i).feature1 == _cf.at(j).feature2 && _cf.at(i).feature2 == _cf.at(j).feature1) {
                _cf.erase(_cf.begin() + j);
                break;
            }
        }
    }
    freeArray(points, numOfFlights);
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyReports;
    int feature1Index, feature2Index;
    for (int i = 0; i < this->_cf.size(); ++i) {
        if (this->_cf[i].corrlation >= _threshold) {
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
                if (dev(p, this->_cf[i].lin_reg) > _cf[i].maxOffset) {
                    AnomalyReport ar = AnomalyReport(_cf[i].feature1 + "-" + _cf[i].feature2, j + 1);
                    anomalyReports.push_back(ar);
                }
            }


        }

    }
    return anomalyReports;
}

Point **SimpleAnomalyDetector::recordsToPoints(vector<float> feature1Records, vector<float> feature2Records, int size) {
    Point **array = new Point *[size];
    for (int i = 0; i < size; ++i) {
        array[i] = new Point(feature1Records[i], feature2Records[i]);
    }
    return array;
}

void SimpleAnomalyDetector::freeArray(Point **array, int sizeOfArray) {
    if (array != nullptr) {
        for (int i = 0; i < sizeOfArray; i++) {
            delete[] array[i];
        }

        delete[] array;
        array = nullptr;
    }
}

float SimpleAnomalyDetector::getThreshold(){
    return _threshold;
}

void SimpleAnomalyDetector::setThreshold(float newThreshold){
    this->_threshold = newThreshold;
}