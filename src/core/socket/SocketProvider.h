//
// Created by Pilebones on 11/09/2015.
//

#ifndef CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETPROVIDER_H
#define CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETPROVIDER_H

#include "SocketWrapper.h"

#define BUFFER_POOL_LENGHT 256

using namespace std;

class SocketProvider : public SocketWrapper {
    public:
        SocketProvider(string hostname, int port);
        virtual ~SocketProvider();

        // Generic function
        string readSocketAsString(SOCKET sock, bool nonBlockingMode = false);
        bool writeSocketAsString(SOCKET sock, string data);
        void setSocketNonBlock(SOCKET sock);

        // Function with current socket (this->getSocket)
        string readAsString(bool nonBlockingMode = false);
        bool writeAsString(string data);
        void setNonBlock();
};

#endif //CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETPROVIDER_H
