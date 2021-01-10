
#ifndef TIMESERIES_H
#define TIMESERIES_H


#include <string>
#include <fstream>
#include <vector>
#include <sstream>


using namespace std;

class TimeSeries {

public:
    TimeSeries(const char *CSVfileName);
    void addRecord(vector<float> record);
    float find(int i, int j);
    int numOfElements() const;
    vector<vector<float>> getRecords() const;
    vector<string> getElementsNames() const;
    vector<vector<float>> getSortedRecords() const;
    int numOfFlights() const;
    virtual ~TimeSeries();
private:
    vector<vector<float>> _records;
    vector<string> _elementsName;
    vector<vector<float>> _sortedRecords;
    void fileToRecords(const char *fileName);
};




#endif //TIMESERIES_H
