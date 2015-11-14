//
// Created by Pilebones on 11/09/2015.
//

#ifndef CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETSERVERPROVIDER_H
#define CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETSERVERPROVIDER_H

#include "SocketProvider.h"

using namespace std;

class SocketServerProvider : public SocketProvider  {
private:
    // TODO: Mono client pour le moment => à évoluer
    SOCKET socketClient;
    SOCKADDR_IN socketAddrInClient;
public:
    SocketServerProvider(string hostname, int port);
    virtual ~SocketServerProvider();

    void start();

    SOCKET getSocketClient();
    SocketServerProvider & setSocketClient(SOCKET socketClient);

    SOCKADDR_IN getSocketAddrInClient();
    SocketServerProvider & setSocketAddrInClient(SOCKADDR_IN socketAddrInClient);
};
#endif //CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETSERVERPROVIDER_H
