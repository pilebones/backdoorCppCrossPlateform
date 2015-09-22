/*
 * SocketClientProvider.cpp
 *
 *  Created on: 3 sept. 2015
 *      Author: Pilebones
 */

#include "SocketClientProvider.h"

/**
 * Sockets Constructor
 * \param int timeout - time in second
 */
SocketClientProvider::SocketClientProvider(int timeout) : SocketProvider() {

    #if defined(OS_Windows)
		WSADATA WSAData;
		int error = WSAStartup(MAKEWORD(2,2), &WSAData);
	#elif defined(OS_Linux)
		int error = 0;
	#endif
	if(!error) {
		// Create the socket
        SocketProvider::setSocket(socket(AF_INET, SOCK_STREAM, 0));

        // Set up the socket timeout
        #if defined(OS_Windows)
            timeout = timeout * 1000; // Time out in millisecond for Windows
            this->setTimeout(this->getSocket(), timeout);
        #elif defined (OS_Linux)
            struct timeval tv;
            tv.tv_sec = timeout;
            tv.tv_usec = 0;
            this->setTimeout(this->getSocket(), tv);
        #endif
		// Set up the file descriptor set.
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(SocketProvider::getSocket(), &fds) ;
	} else {
		throw new logic_error("Unable to init socket");
	}
}

/**
 * Function's template to simplify call to setsockopt() across OS to define the socket timeout
 */
template <class T> void SocketClientProvider::setTimeout(SOCKET handle, T timeout){
    setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO,(const char *)&timeout, sizeof(timeout) );
    setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO,(const char *)&timeout, sizeof(timeout) );
}

/**
 * Sockets Destructor
 */
SocketClientProvider::~SocketClientProvider() {
    /* Close socket with same function name for each OS (see: macro inside *.h) */
	closesocket(SocketProvider::getSocket());

	/* Close only on Windows socket extra-setting */
	#if defined(OS_Windows)
		WSACleanup();
	#endif
}

/**
 * To connect to the following target => host:port
 *
 * \param string hostname - target hostname
 * \param  int port - target port
 * \return boolean - true if the connection works else false
 */
bool SocketClientProvider::connection(string hostname, int port) {

	/* Connection settings */
	SOCKADDR_IN socketAddrIn;
	socketAddrIn.sin_addr.s_addr	= inet_addr((char*) hostname.c_str());
	socketAddrIn.sin_family			= AF_INET;
	socketAddrIn.sin_port			= htons(port);

	/* Try to connect */
	if(0 == connect(
        SocketProvider::getSocket(),
		(SOCKADDR*) &socketAddrIn,
		sizeof(socketAddrIn)
	)) {
		return true;
	}

    perror("Connect failed. Error");
	return false;
}
