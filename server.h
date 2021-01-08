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



using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        char bufffer[1024];
        bzero(bufffer,1024);
        int n= read(clientID, bufffer,100);
        cout << bufffer <<endl;
        const char* hello= "Hello from server";
        send(clientID, hello,strlen(hello),0);

    }
};


// implement on Server.cpp
class Server {
    thread* _t; // the thread to run the start() method in
    int _clientLimit = 3;
    int _fd;
    sockaddr_in _server;
    sockaddr_in _client;
    // you may add data members

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

int main(){
    try{
        Server server(1234);
        AnomalyDetectionHandler ch;
        server.start(ch);
        server.stop();
    }   catch (const char * err){
        cout << err << endl;
    }
    cout << "done" << endl;
    return 0;
}

#endif //EX6_SERVER_H
