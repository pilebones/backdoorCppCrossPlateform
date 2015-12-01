//
// Created by mfranc on 25/11/15.
//

#ifndef BACKDOORCPPCROSSPLATEFORM_SOCKETWRAPPER_H
#define BACKDOORCPPCROSSPLATEFORM_SOCKETWRAPPER_H

#include "../utils/Network.h"
#include <sstream>

class SocketWrapper {

private:
    SOCKET handle;
    SOCKADDR_IN socketAddrIn;
    SOCKADDR socketAddr;
public:
    string getSlug();

    string getIp();
    int getPort();

    SOCKET getSocket();
    SocketWrapper & setSocket(SOCKET handle);

    SOCKADDR_IN getSocketAddrIn();
    SocketWrapper & setSocketAddrIn(SOCKADDR_IN socketAddrIn);

    SOCKADDR getSocketAddr();
    SocketWrapper & setSocketAddr(SOCKADDR socketAddrIn);
};


#endif //BACKDOORCPPCROSSPLATEFORM_SOCKETWRAPPER_H
