//
// Created by yanir on 03/01/2021.
//

#ifndef EX6_SERVER_H
#define EX6_SERVER_H

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

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID, int fd)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID, int fd){
        SocketIO socketIO(fd,clientID);
        CLI cli(&socketIO);
        cli.start();
//        char buffer[1024];
//        bzero(buffer,1024);
//        int n= read(clientID, buffer,100);
//        cout << buffer <<endl;
//        const char* hello= "Hello from server";
//        send(clientID, hello,strlen(hello),0);

    }
};


// implement on Server.cpp
class Server {
    thread* _t; // the thread to run the start() method in
    int _clientLimit = 3;
    int _fd;
    sockaddr_in _server;
    sockaddr_in _client;
    bool _stop = false;
    // you may add data members

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};


#endif //EX6_SERVER_H
