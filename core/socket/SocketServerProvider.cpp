//
// Created by Pilebones on 11/09/2015.
//

#include "SocketServerProvider.h"

/**
 * Sockets server Constructor
 */
SocketServerProvider::SocketServerProvider(string hostname, int port) : SocketProvider(hostname, port) {
    SOCKADDR_IN socketAddrIn = this->getSocketAddrIn();
    bind(this->getSocket(), (SOCKADDR *)& socketAddrIn, sizeof(socketAddrIn));
    listen(this->getSocket(), 0);
}
/**
 * Sockets Destructor
 */
SocketServerProvider::~SocketServerProvider() {}

void SocketServerProvider::start() {

    SOCKET socketClient = this->getSocketClient();
    SOCKADDR_IN socketAddrInClient = this->getSocketAddrInClient();

    while(1) {
        unsigned int socketAddrInClientSize = sizeof(socketAddrInClient);
        if(socketClient = accept(this->getSocket(), (SOCKADDR *)&socketAddrInClient, &socketAddrInClientSize) != INVALID_SOCKET) {
            this->setSocketClient(socketClient);
            send(this->getSocketClient(), "Welcome !\r\n", 11, 0);
        }
    }
}

SOCKET SocketServerProvider::getSocketClient() {
    return this->socketClient;
}

SocketServerProvider & SocketServerProvider::setSocketClient(SOCKET socketClient) {
    this->socketClient = socketClient;
    return *this;
}

SOCKADDR_IN SocketServerProvider::getSocketAddrInClient() {
    return this->socketAddrInClient;
}

SocketServerProvider & SocketServerProvider::setSocketAddrInClient(SOCKADDR_IN socketAddrInClient) {
    this->socketAddrInClient = socketAddrInClient;
    return *this;
}