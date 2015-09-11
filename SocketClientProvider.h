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
	#include <iostream>
	#include <cstdlib>
	#include <stdio.h>

	// defined usefull constant & macro
	#define BUFFER_POOL_LENGHT 256

	#ifdef __unix__
		#define OS_Linux
	#elif defined(_WIN32) || defined(WIN32)
		#define OS_Windows
	#endif

	// Cross plateform compatibility
	#if defined(OS_Windows)
	    #include <winsock2.h>
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

	class SocketClientProvider {
	private:
	    SOCKET clientSocket;
	    SOCKADDR socketAddr;
	    SOCKADDR_IN socketAddrIn;
	public:
		SocketClientProvider(int timeout = 50);
		virtual ~SocketClientProvider();

		SOCKET getClientSocket();
		void setClientSocket(SOCKET clientSocket);

		SOCKADDR getSocketAddr();
		void setSocketAddr(SOCKADDR socketAddr);

		string readAsString(bool nonBlockingMode = false);
		bool writeAsString(string data);

		bool connection(string host,int port);
		string resolvAddress(string hostname);

		void setNonBlock();
	};

#endif /* SOCKETCLIENTPROVIDER_H_ */
