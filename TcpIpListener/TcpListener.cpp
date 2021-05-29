//
// Created by Josh on 2021-05-25.
//
#include <iostream>
#include "Tcplistener.h"

#define MAX_BUFFER_SIZE (49152)

CTcpListener::CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler)
    : m_ipAdress(ipAddress), m_port(port), MessageReceived(handler) // the : after construtor allows initialization of variables before the body runs so you can modify const values -- in this case we are just initializing private member variables
{

}

CTcpListener::~CTcpListener(){
    Cleanup(); // If cleanup function isn't explicitly called we will call it anyway during the destructor
}

//send a message to the specified client
void CTcpListener::Send(int clientSocket, std::string msg){
    send(clientSocket, msg.c_str(), msg.size()+1,0); //client socket is a buffer that will be overwritten with the c-style string msg and it has size + 1 because c style strings end with a null char -- we converted our c++ string to c with the c_str() function
    //note send is the winsock function not our method
}

//Initialize winsock

bool CTcpListener::Init(){
    //pass in version number and structure
    WSAData data{};
    WORD ver = MAKEWORD(2,2); //version we are using

    int wsInit = WSAStartup(ver, &data); // WSAStartup initiates the Winsock DLL process
    if(wsInit != 0){
        std::cerr << "Couldn't start up winsock" << std::endl;
        return false;
    }

    return wsInit == 0; // if its 0 its good
}

// The main processing loop
void CTcpListener::Run(){

    char buf[MAX_BUFFER_SIZE]{};

    while(true){
        // Create a listening socket
        SOCKET listening = CreateSocket();
        if(listening == INVALID_SOCKET){
            break;
        }

        SOCKET client = WaitForConnection(listening); // returns client socket

        if(client != INVALID_SOCKET){ // If we have a valid socket
            closesocket(listening); //Once we have a client connection we will close the listening socket

            int bytesReceived = 0;

            do{
                ZeroMemory(buf, MAX_BUFFER_SIZE); // clear out memory so we dont have weird pieces of a previous message remaining

                bytesReceived = recv(client, *&buf, MAX_BUFFER_SIZE, 0);

                if(bytesReceived > 0){
                    if(MessageReceived != NULL){
                        std::cout << "received one" << std::endl;
                        MessageReceived(this, client, std::string(buf, 0, bytesReceived));
                    }
                }

            } while (bytesReceived > 0);

            closesocket(client);

            // Wait for connection
            // Loop receive / send
        }

    }

}

void CTcpListener::Cleanup(){

    WSACleanup();

}

//create a socket
SOCKET CTcpListener::CreateSocket(){

    SOCKET listening = socket(AF_INET, SOCK_STREAM,0); // AF_INET is ipv4 sock_stream means tcp and 0 means default protocol eg let the computer chose its smarter than me

    if(listening != INVALID_SOCKET){

        sockaddr_in hint{};

        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_port); // Port we want to listen on that is passed into our CTcpListener function
        inet_pton(AF_INET, m_ipAdress.c_str(), &hint.sin_addr); // This converts our string ip address to hexadecimal in ipv4 format and it writes it into the buffer hint.sin_addr so we dont have to set hint.sin_addr

        //we need to bind to the socket so the socket is connected to the ipaddress and port tupled (hint object) so when we refer to the socket it will refer to the port # and ip it was bound to -- then we will listen to the socket
        int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint)); // now listening is bound to the pointer to hint which contains our ip and port number -- listening is the socket we made right before the if statement in this method

        if(bindOk != SOCKET_ERROR){
            int listenOk = listen(listening, SOMAXCONN); // we will listen on the socket listening and will accept SOMAXCONN amount of connections which is a ridiculous amount
            if(listenOk == SOCKET_ERROR){
                std::cerr << "Unable to listen to the socket " << std::endl;
                return -1;
            }
        }else{
            std::cerr << "Binding the socket to the port and ip was not ok " << std::endl;
            return -1;
        }
    } else{
        std::cerr << "Invalid socket" << std::endl;
        return -1;
    }

    return listening; //we return the socket we are listening on
}

//wait for a connection
SOCKET CTcpListener::WaitForConnection(SOCKET listening){

    SOCKET client = accept(listening, NULL, NULL); // First NULL is an option to pass a buffer (memory address) that will store the connecting address
                                                    // Second NULL is an optional buffer (memory address) to point/store the integer length of the connecting address
    return client;
}