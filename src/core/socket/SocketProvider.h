//
// Created by Pilebones on 11/09/2015.
//

#ifndef CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETPROVIDER_H
#define CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETPROVIDER_H

#define BUFFER_POOL_LENGHT 256

#ifdef __unix__
    #define OS_Linux
#elif defined(_WIN32) || defined(WIN32)
    #define OS_Windows
#endif

// Std librairies
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <stdexcept>

// Cross plateform compatibility
#if defined(OS_Windows)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#elif defined(OS_Linux)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include<netdb.h> //hostent
    #include <unistd.h>

    // Rewrite function name with macro to be the same name as Windows API
    #define closesocket(s) close (s)
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif

using namespace std;

class SocketProvider {
    private:
        SOCKET handle;
        SOCKADDR_IN socketAddrIn;
        SOCKADDR socketAddr;
    public:
        SocketProvider(string hostname, int port);
        virtual ~SocketProvider();

        SOCKET getSocket();
        void setSocket(SOCKET handle);

        SOCKADDR_IN getSocketAddrIn();
        void setSocketAddrIn(SOCKADDR_IN socketAddrIn);

        SOCKADDR getSocketAddr();
        void setSocketAddr(SOCKADDR socketAddrIn);

        string readAsString(bool nonBlockingMode = false);
        bool writeAsString(string data);

        static string resolvAddress(string hostname);
        void setNonBlock();
};

#endif //CPPCROSSPLATEFORMSOCKETPROVIDER_SOCKETPROVIDER_H
