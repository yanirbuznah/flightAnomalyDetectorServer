#include "CLI.h"
#include "commands.h"



using namespace std;
CLI::CLI(DefaultIO* dio) {
    this->_dio = dio;
    this->createMap();
}

void CLI::start(){
    string option;
    while (true){
        _dio->write(
                "Welcome to the Anomaly Detection Server.\n"
                "Please choose an option:\n");
        for (pair<string,Command*> command: _commandMap){
            string s = command.second->getDescription();
            _dio->write(s);
        }
                _dio->write("6.exit\n");
        option = _dio->read();
        if (option == "6"){
            break;
        }
        Command* command  = _commandMap.find(option)->second;
        command->execute();
    }
}


CLI::~CLI() {
}

void CLI :: createMap(){
    _commandMap.insert({"1", new UploadTimeSeries(_dio,&_anomalyReports,&_anomalyDetector)});
    _commandMap.insert({"2", new AlgorithmSettings(_dio,&_anomalyReports,&_anomalyDetector)});
    _commandMap.insert({"3", new DetectAnomalies(_dio,&_anomalyReports,&_anomalyDetector)});
    _commandMap.insert({"4", new DisplayResults(_dio,&_anomalyReports,&_anomalyDetector)});
    _commandMap.insert({"5", new UploadAndAnalyze(_dio,&_anomalyReports,&_anomalyDetector)});
}