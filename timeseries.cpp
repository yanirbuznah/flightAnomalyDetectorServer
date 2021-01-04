
#include "timeseries.h"
#include <fstream>
#include <iostream>


#include "timeseries.h"


using namespace std;

TimeSeries:: TimeSeries(const char *CSVfileName) {
    fileToRecords(CSVfileName);
}
TimeSeries:: ~TimeSeries() {

}

void TimeSeries:: fileToRecords(const char *fileName) {
    ifstream fin(fileName);
    vector<float> row;
    vector<vector<float>> records;
    vector<vector<float>> sortedRecords;
    vector<float> column;
    vector<string> elementsNames;
    string line, word, temp;


    fin >> temp;
    stringstream s(temp);

    while (getline(s, word, ',')) {
        elementsNames.push_back(word);
    }

    //create X element columns.
    for (int i = 0; i < elementsNames.size(); ++i) {
        sortedRecords.push_back(column);
        column.clear();
    }

    while (fin >> temp) {
        row.clear();
        // used for breaking words
        stringstream s(temp);
        int i = 0;
        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ',')) {
            sortedRecords.at(i++ % elementsNames.size()).push_back(stof(word));
            // add all the column data
            // of a row to a vector
            row.push_back(stof(word));
        }
        records.push_back(row);
    }

    _elementsName = elementsNames;
    _sortedRecords = sortedRecords;
    _records =  records;
    fin.close();
}

void TimeSeries:: addRecord(vector<float> record){
    this->_records.push_back(record);
    for (int i = 0; i <_elementsName.size() ; ++i) {
        this->_sortedRecords.at(i).push_back(record.at(i));
    }
}
float TimeSeries :: find(int i, int j){
    return this->_records.at(i).at(j);
}
int  TimeSeries::numOfElements() const {
    return _elementsName.size();
}
vector<vector<float>> TimeSeries:: getRecords() const{
    return _records;
}
vector<string> TimeSeries:: getElementsNames() const{
    return _elementsName;
}
vector<vector<float>> TimeSeries:: getSortedRecords() const{
    return _sortedRecords;
}
int TimeSeries::numOfFlights() const {
    return _records.size();
}