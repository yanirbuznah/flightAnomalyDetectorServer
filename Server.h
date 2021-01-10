//
// Created by yanir on 03/01/2021.
//

#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

#include <pthread.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"


using namespace std;

class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};



class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        SocketIO socketIO(clientID);
        CLI cli(&socketIO);
        cli.start();
    }
};



class Server {
    thread* _t; // the thread to run the start() method in
    int _clientLimit = 3;
    int _fd;
    sockaddr_in _server;
    sockaddr_in _client;
    bool _stop = false;


public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};


#endif //SERVER_H
