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
SocketClientProvider::SocketClientProvider(string hostname, int port, int timeout) : SocketProvider(hostname, port) {
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
bool SocketClientProvider::connection() {

	/* Connection settings */
	SOCKADDR_IN socketAddrIn = this->getSocketAddrIn();

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
