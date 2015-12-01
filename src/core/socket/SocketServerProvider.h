//
// Created by Pilebones on 11/09/2015.
//

#ifndef CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETSERVERPROVIDER_H
#define CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETSERVERPROVIDER_H

#include "SocketProvider.h"
#include <map>

using namespace std;

class SocketServerProvider : public SocketProvider {
private:
    map<string, SocketWrapper> socketClientMap;
    // TODO: Mono client pour le moment => à évoluer
    SOCKET socketClient;
    SOCKADDR_IN socketAddrInClient;
public:
    SocketServerProvider(string hostname, int port);
    virtual ~SocketServerProvider();

    void start();

    map<string, SocketWrapper> &getSocketClientMap();
    SocketServerProvider &setSocketClientMap(map<string, SocketWrapper> socketClientMap);

    bool writeToClientAsString(string clientSlug, string data);
    SocketWrapper getSocketWrapperBySlug(string clientSlug);
    bool hasSocketWrapperBySlug(string clientSlug);
    void displayWelcomeToSocketWrapperBySlug(string clientSlug);

    void addSocketClient(string token, SocketWrapper *socketClient);
    SocketWrapper kickSocketClient(string token);
};
#endif //CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETSERVERPROVIDER_H
