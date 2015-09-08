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

#include "SocketClientProvider.h"

/**
 * Sockets Constructor
 */
SocketClientProvider::SocketClientProvider() {

	#if defined (WIN32)
		WSADATA WSAData;
		int error = WSAStartup(MAKEWORD(2,2), &WSAData);
	#else
		int error = 0;
	#endif

	this->buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

	if(!error) {
		/* Create Socket */
		this->setClientSocket(socket(AF_INET, SOCK_STREAM, 0));
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
	#if defined (WIN32)
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

/*void SocketClientProvider::setNonBlock() {
    int flags;
    int socket = this->getClientSocket();
    flags = fcntl(socket, F_GETFL, 0);
    assert(flags != -1);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}*/

/**
 * Read data from the opened socket
 *
 * \return string - data read
 */
string SocketClientProvider::readAsString() {
	char* buffer = this->getBuffer();
	string response;
	signed int nbBytes = 0;

	do {
		nbBytes = recv(this->getClientSocket(), buffer, 1, 0); // read bytes by bytes until the end
		// cout << "nb bytes : " << nbBytes << endl;
		if (-1 == nbBytes) { // There is an error => what it is
			int errorNumber = errno; // Save errno to avoid data lost
			switch (errorNumber) {
				#if defined (WIN32) // If windows OS used
					case WSAEWOULDBLOCK: // Socket is NONBLOCK and there is no data available
						cout << "WSAEWOULDBLOCK" << endl;
					break;
				#elif defined (linux) // Else unix OS used
					case EWOULDBLOCK: // Socket is NONBLOCK and there is no data available
						cout << "WSAEWOULDBLOCK" << endl;
					break;
				#endif
				case EAGAIN: // There is no data available for reading on a non-blocking socket => should run the recv() again
					cout << "EAGAIN" << endl;
					break;
				case EINTR: // An interrupt (signal) has been catched => should be ingore in most cases
					cout << "EINTR" << endl;
					break;
				default:
					perror("Receive failed. Error");
					// socket has an error, no valid anymore
					break;
		   }
		} else if (nbBytes > 0) { // Data has been received
			// cout << buffer << endl;
			response += string(buffer);
		}
	} while (0 < nbBytes);

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
 * Getter for the data inside socket's buffer
 *
 * \return char* - The data inside buffer
 */
char* &SocketClientProvider::getBuffer() {
	return this->buffer;
}

/**
 * Set data inside socket's buffer
 */
void SocketClientProvider::setBuffer(char* buffer) {
	this->buffer = buffer;
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
