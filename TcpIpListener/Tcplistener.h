//
// Created by Josh on 2021-05-24.
//
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") //winsock library file which contains the executables

//#ifndef TCPIPLISTENER_TCPLISTENER_H
//#define TCPIPLISTENER_TCPLISTENER_H
//
//#endif //TCPIPLISTENER_TCPLISTENER_H
//

// todo: Callback to data received

class CTcpListener;

typedef void (*MessageRecievedHandler)(CTcpListener*, int socketId, std::string msg);

class CTcpListener{

public:

    CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler);

    ~CTcpListener();

    //send a message to the specified client
    void Send(int clientSocket, std::string msg);

    //Initialize winsock

    bool Init();

    // The main processing loop
    void Run();

    void Cleanup();

    //receive loop

    //send back message

    //cleanup

private:

    //create a socket
    SOCKET CreateSocket();

    //wait for a connection
    SOCKET WaitForConnection(SOCKET listening);

    std::string m_ipAdress;
    int m_port;
    MessageRecievedHandler MessageReceived;

};
