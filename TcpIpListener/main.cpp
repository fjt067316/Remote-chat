#include <iostream>
#include <string>
#include "Tcplistener.h"

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);

int main() {

    CTcpListener server("127.0.0.1", 54000, Listener_MessageReceived);

    if(server.Init()){
        server.Run();
    }

    return 0;
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg){

    std::string serverMsg{};

    std::cout << msg << std::endl;
    std::cin >> serverMsg;

    listener->Send(client,serverMsg);


    //listener->Send(client,msg); //echo message back to client

}