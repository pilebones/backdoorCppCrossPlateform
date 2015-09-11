/*
 * SocketClientProvider.cpp
 *
 *  Created on: 3 sept. 2015
 *      Author: pilebones
 *
 * @see http://sdz.tdct.org/sdz/les-sockets.html
 * @see http://c.developpez.com/cours/sockets-c-cpp-demystifies/
 * @see http://sylvainmarechal.chez-alice.fr/prog/win32/socket/socket.html
 */

#include <fcntl.h>
#include "SocketClientProvider.h"

/**
 * Sockets Constructor
 */
SocketClientProvider::SocketClientProvider(int timeout) {

	#if defined(OS_Windows)
		WSADATA WSAData;
		int error = WSAStartup(MAKEWORD(2,2), &WSAData);
	#elif defined(OS_Linux)
		int error = 0;
	#endif

	if(!error) {
		/* Create Socket */
		this->setClientSocket(socket(AF_INET, SOCK_STREAM, 0));

		#if defined(OS_Windows)
			// TODO: Implement timeout setter for OS_Windows with D_WORD param for setsockopt
		#elif defined(OS_Linux)
			struct timeval tv;
			tv.tv_sec = timeout;
			tv.tv_usec = 0;
			setsockopt(this->getClientSocket(), SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(struct timeval));
			setsockopt(this->getClientSocket(), SOL_SOCKET, SO_SNDTIMEO,(char *)&tv,sizeof(struct timeval));
		#endif
	} else {
		throw "Unable to init socket";
	}
}

/**
 * Sockets Destructor
 */
SocketClientProvider::~SocketClientProvider() {
    /* Close socket with same function name for each OS (see: macro inside *.h) */
	closesocket(this->getClientSocket());

	/* Close only on Windows socket extra-setting */
	#if defined(OS_Windows)
		WSACleanup();
	#endif
}

/**
 * Resolv IP to do querying a valid target
 *
 * \param string hostname - IP or DNS record must be resolv
 */
string SocketClientProvider::resolvAddress(string hostname) {
	//setup address structure
	if(-1 == inet_addr(hostname.c_str())) {
		struct hostent *he;
		struct in_addr **addr_list;

		//resolve the hostname, its not an ip address
		if ( NULL == (he = gethostbyname(hostname.c_str() ) )) {
			//gethostbyname failed
			throw "Unable to resolv" + hostname;
		}

		//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		addr_list = (struct in_addr **) he->h_addr_list;
		string result;
		for(int i = 0; addr_list[i] != NULL; i++) {
			result = inet_ntoa(*addr_list[i]);
			break;
		}
		return string(result);
	}

	//plain ip address
	return hostname;
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
		this->getClientSocket(),
		(SOCKADDR*) &socketAddrIn,
		sizeof(socketAddrIn)
	)) {
		return true;
	}

    perror("Connect failed. Error");
	return false;
}

void SocketClientProvider::setNonBlock() {
	#if defined(OS_Linux)
		int flags = fcntl(this->getClientSocket(), F_GETFL, 0);
		flags |= O_NONBLOCK;
		fcntl(this->getClientSocket(), F_SETFL, flags);
	#endif
}

/**
 * Read data from the opened socket
 *
 * \return string - data read
 */
string SocketClientProvider::readAsString(bool nonBlockingMode) {

	string response;
	char buffer[BUFFER_POOL_LENGHT];
	ssize_t nbBytes = 0;
	bool toContinue = true;

	if (nonBlockingMode) {
		this->setNonBlock();
	}

	while (toContinue) {
		// 	nbBytes = recv(this->getClientSocket(), buffer, 1, 0); // Previously : read bytes by bytes until the end
		nbBytes = recv(this->getClientSocket(), buffer, sizeof(buffer) - 1, 0); // Blocking sys-call by default
		if (0 == nbBytes) { // No data receive
			toContinue = false;
		} else if (nbBytes > 0) { // Data has been received
			buffer[nbBytes] = '\0'; // Terminate buffer
			response += string(buffer);
		} else { // There is an error => what it is
			int errorNumber = errno; // Save errno to avoid data lost
			switch (errorNumber) {
				#if defined(OS_Windows)
				case WSAEWOULDBLOCK: // Socket is NONBLOCK and there is no data available
					toContinue = false;
					break;
                #elif defined (OS_Linux)
				case EWOULDBLOCK: // There is no data available for reading on a non-blocking socket => should run the recv() again
					toContinue = false;
					break;
                #endif
				case EINTR: // An interrupt (signal) has been catched => should be ingore in most cases
					break;
				default: // socket has an error, no valid anymore
					cerr << "Error n°" << errorNumber << endl;
					perror("Receive failed. ");
					toContinue = false;
					break;
			}
		}
	}

	return response;
}

/**
 * Put data to the opened socket
 */
bool SocketClientProvider::writeAsString(string data) {
	if(-1 == send(
		this->getClientSocket(),
		data.c_str(),
		data.length(),
		0
	) ) {
		perror("Send failed. Error");
		return false;
	}
	return true;
}

/**
 * Getter for the socket
 *
 * \return SOCKET the socket
 */
SOCKET SocketClientProvider::getClientSocket() {
	return this->clientSocket;
}

/**
 * Setter for the socket
 * \param SOCKET clientSocket - Init socket
 */
void SocketClientProvider::setClientSocket(SOCKET clientSocket) {
	this->clientSocket = clientSocket;
}

/**
 * Getter for the socket addr
 *
 * \return SOCKADDR the socket target
 */
SOCKADDR SocketClientProvider::getSocketAddr() {
	return this->socketAddr;
}

/**
 * Setter for the socket addr
 * \param SOCKADDR socketAddr - Init socket target
 */
void SocketClientProvider::setSocketAddr(SOCKADDR socketAddr) {
	this->socketAddr = socketAddr;
}
