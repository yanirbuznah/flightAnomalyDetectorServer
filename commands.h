
#ifndef EX4_COMMANDS_H
#define EX4_COMMANDS_H

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}
};
class SocketIO: public DefaultIO{
    int _clientID;

public:
    SocketIO(int clientID):_clientID(clientID){};
    string read() override{
        string serverInput="";
        char c=0;
        ::read(_clientID,&c,sizeof(char));
        while(c!='\n'){
            serverInput+=c;
            ::read(_clientID,&c,sizeof(char));
        }
        return serverInput;
    }
    void write(string text) override{

        ::write(_clientID,text.c_str(),text.length());
    }
    void write(float f) override{
        string fString = to_string(f);
        fString.erase(fString.find_last_not_of('0') + 1, std::string::npos);
        ::write(_clientID,fString.c_str(),fString.length());
    }
    void read(float* f) override{
        string serverInput="";
        char c=0;
        ::read(_clientID,&c,sizeof(char));
        while(c!='\n'){
            serverInput+=c;
            ::read(_clientID,&c,sizeof(char));
        }
        *f = stof( serverInput);
    }
    ~SocketIO() override{}

};

class Command {
protected:
    DefaultIO *_dio;
    HybridAnomalyDetector *_detector;
    vector<AnomalyReport> *_anomalyReports;
    string _description;
public:
    Command(DefaultIO *dio, vector<AnomalyReport> *anomalyReports, HybridAnomalyDetector *anomalyDetector):_dio(dio),
    _anomalyReports(anomalyReports),_detector(anomalyDetector) {}

    Command();

    virtual void execute() = 0;
    virtual string getDescription()=0;
    virtual ~Command() {}

};


class UploadTimeSeries : public Command {
    string _description = "1.upload a time series csv file\n";
public:
    using Command::Command;
    void execute() override {
        string text;
        ofstream anomalyTrain("anomalyTrain.csv");
        ofstream anomalyTest("anomalyTest.csv");
        _dio->write("Please upload your local train CSV file.\n");
        do {
            anomalyTrain << text;
            text = _dio->read();
            text = text + "\n";
        } while (text != "done\n");
        text = "";
        _dio->write("Upload complete.\n");
        _dio->write("Please upload your local test CSV file.\n");
        do {
            anomalyTest << text;
            text = _dio->read();
            text = text + "\n";
        } while (text != "done\n");
        _dio->write("Upload complete.\n");
        if (anomalyTrain.is_open())
            anomalyTrain.close();
        if (anomalyTest.is_open())
            anomalyTest.close();
    }
    string getDescription(){
        return _description;
    }
};

class AlgorithmSettings : public Command {
    string _description = "2.algorithm settings\n";
public:
    using Command::Command;

    void execute() override {
        string threshold = to_string(_detector->getThreshold());
        threshold.erase(threshold.find_last_not_of('0') + 1, std::string::npos);
        string text = "The current correlation threshold is " + threshold + "\n";
        _dio->write(text);
        _dio->write("Type a new threshold\n");
        do {
            string userChoise = _dio->read();
            float newThreshold = stof(userChoise);
            if (newThreshold <= 1 && newThreshold >= 0) {
                _detector->setThreshold(newThreshold);
                break;
            }
            _dio->write("please choose a value between 0 and 1.\n");
        } while (true);

    }
    string getDescription(){
        return _description;
    }
};

class DetectAnomalies : public Command {
    string _description = "3.detect anomalies\n";
public:
    using Command::Command;

    void execute() override {
        TimeSeries anomalyTrainTs = TimeSeries("anomalyTrain.csv");
        TimeSeries anomalyTestTs = TimeSeries("anomalyTest.csv");
        _detector->learnNormal(anomalyTrainTs);
        *_anomalyReports = _detector->detect(anomalyTestTs);
        _dio->write("anomaly detection complete.\n");
    }
    string getDescription(){
        return _description;
    }
};

class DisplayResults : public Command {
    string _description = "4.display results\n";
public:
    using Command::Command;

    void execute() override {
        string text;
        for (const auto &value: *_anomalyReports) {
            text = to_string(value.timeStep) + "\t" + value.description + "\n";
            _dio->write(text);
        }
        _dio->write("Done.\n");
    }
    string getDescription(){
        return _description;
    }
};

class UploadAndAnalyze : public Command {
    string _description = "5.upload anomalies and analyze results\n";
public:
    using Command::Command;

    void execute() override {
        map<int, int> anomalyMap;
        vector<pair<long, long>> unifedReports;
        string start, end;
        string input;
        TimeSeries test = TimeSeries("anomalyTest.csv");
        float n = test.numOfFlights();
        float truePositive = 0, falsePositive = 0;
        _dio->write("Please upload your local anomalies file.\n");
        input = _dio->read();
        while (input != "done") {
            stringstream ss(input);
            getline(ss, start, ',');
            getline(ss, end, ',');
            anomalyMap.insert({stoi(start), stoi(end)});
            input = _dio->read();
        }
        _dio->write("Upload complete.\n");
        float positive = anomalyMap.size();
        for (pair<int, int> anomaly : anomalyMap) {
            n -= (float)(anomaly.second - anomaly.first + 1);
        }
        int anomalyReportMaxIndex = _anomalyReports->size() - 1;
        for (int i = 0; i < anomalyReportMaxIndex; i++) {
            long firstTimeStep = _anomalyReports->at(i).timeStep;
            long lastTimeStep = _anomalyReports->at(i).timeStep;
            while (i < anomalyReportMaxIndex
                   && _anomalyReports->at(i).timeStep + 1 == _anomalyReports->at(i + 1).timeStep
                   && _anomalyReports->at(i).description == _anomalyReports->at(i + 1).description) {
                lastTimeStep = _anomalyReports->at(++i).timeStep;
            }
            unifedReports.emplace_back(firstTimeStep, lastTimeStep);
        }
        for (pair<int, int> anomaly : anomalyMap) {
        for (pair<long, long> report: unifedReports) {

                //if the report between the anomalies:
                if (report.second >= anomaly.first && anomaly.second >= report.first){
                    truePositive++;
                    break;
                }
            }

        }
        for (pair<long, long> report: unifedReports) {
            bool tpDetected = false;
            for (pair<int, int> anomaly : anomalyMap) {
                //if the report between the anomalies:
                if (report.second >= anomaly.first && anomaly.second >= report.first){
                    tpDetected = true;
                }
            }
            if (!tpDetected) {
                falsePositive++;
            }
        }
        if (positive == 0) {
            _dio->write("True Positive Rate: 0\n");
        } else {
            float positiveRate = truePositive / positive;
            positiveRate = floorf(1000*positiveRate)/1000;
            _dio->write("True Positive Rate: ");
            _dio->write(positiveRate);
            _dio->write("\n");
        }
        float falseAlaramRate = falsePositive / n;
        falseAlaramRate = floorf(1000*falseAlaramRate)/1000;
        _dio->write("False Positive Rate: ");
        _dio->write(falseAlaramRate);
        _dio->write("\n");
    }
    string getDescription(){
        return _description;
    }
};

#endif //EX4_COMMANDS_H
