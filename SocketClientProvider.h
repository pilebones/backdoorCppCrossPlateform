/*
 * SocketClientProvider.h
 *
 *  Created on: 3 sept. 2015
 *      Author: Pilebones
 */

#ifndef SOCKETCLIENTPROVIDER_H_
#define SOCKETCLIENTPROVIDER_H_

	using namespace std;

	// Std librairies
	#include <string>
	#include <string.h>    //strlen
	#include <iostream>
	#include <cstdlib>
	#include <stdio.h>

	// defined usefull constant & macro
	#define BUFFER_SIZE 2048

	// Cross plateform compatibility
	#if defined (WIN32) //Si nous sommes sous Windows
	    #include <winsock2.h>
	    #pragma comment(lib, "ws2_32.lib")
		typedef int socklen_t;
	#elif defined (linux) // Sinon, si nous sommes sous Linux
	    #include <sys/types.h>
	    #include <sys/socket.h>
	    #include <netinet/in.h>
	    #include <arpa/inet.h>
		#include<netdb.h> //hostent
	    #include <unistd.h>

		// Rewrite function name to be the same name as Windows API
		#define closesocket(s) close (s)
		#define INVALID_SOCKET -1
		#define SOCKET_ERROR -1
	    typedef int SOCKET;
	    typedef struct sockaddr_in SOCKADDR_IN;
	    typedef struct sockaddr SOCKADDR;
	#endif

	class SocketClientProvider {
	private:
	    SOCKET clientSocket;
	    SOCKADDR socketAddr;
	    SOCKADDR_IN socketAddrIn;
	    char* buffer;
	public:
		SocketClientProvider();
		virtual ~SocketClientProvider();

		SOCKET getClientSocket();
		void setClientSocket(SOCKET clientSocket);

		SOCKADDR getSocketAddr();
		void setSocketAddr(SOCKADDR socketAddr);

		char* & getBuffer();
		void setBuffer(char* buffer);

		string readAsString();
		bool writeAsString(string data);

		// sockaddr_in getSocketAddrIn();
		// void setSocketAddrIn(sockaddr_in socketAddrIn);

		bool connection(string host,int port);
		string resolvAddress(string hostname);
	};

#endif /* SOCKETCLIENTPROVIDER_H_ */
