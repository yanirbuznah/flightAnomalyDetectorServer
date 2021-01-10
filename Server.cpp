

#include "Server.h"

using namespace std;

Server::Server(int port) throw(const char *) {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw "Socket failure!";
    _server.sin_family = AF_INET;
    _server.sin_addr.s_addr = INADDR_ANY;
    _server.sin_port = htons(port); //  htons convert to network format
    if (bind(_fd, (struct sockaddr *) &_server, sizeof(_server)) < 0)
        throw "Bind failure!";
    if (listen(_fd, _clientLimit) < 0)
        throw "Listen failure!";
}

void Server::start(ClientHandler &ch) throw(const char *) {
    _t = new thread([&ch,this](){// lambda expression, using this for _fd and _client
        while(!_stop){
            cout << "Waiting for a client" <<endl;
            socklen_t clientSize= sizeof (_client);
            int aClient = accept(_fd, (struct  sockaddr*)&_client,&clientSize);
            if(aClient < 0)
                throw "Accept failure!";
            cout << "Client connected" <<endl;
            ch.handle(aClient,_fd);
            close(aClient);
       }

        close(_fd);
    });

}

void Server::stop() {
    _stop = true;
    _t->join(); // do not delete this!

}

Server::~Server() {
}
