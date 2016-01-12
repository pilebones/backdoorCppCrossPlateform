//
// Created by Pilebones on 11/09/2015.
//

#include "SocketProvider.h"

/**
 * Sockets Constructor
 */
SocketProvider::SocketProvider(string hostname, int port) {

#if defined(OS_Windows)
    WSADATA WSAData;
    int error = WSAStartup(MAKEWORD(2,2), &WSAData);
#elif defined(OS_Linux)
    int error = 0;
#endif
    if(!error) {
        // Create the socket
        this->setSocket(socket(AF_INET, SOCK_STREAM, 0));

        // Set up the file descriptor set.
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(this->getSocket(), &fds);

        /* Connection settings */
        SOCKADDR_IN socketAddrIn;
        socketAddrIn.sin_addr.s_addr	= inet_addr((char*) hostname.c_str());
        socketAddrIn.sin_family			= AF_INET;
        socketAddrIn.sin_port			= htons(port);
        this->setSocketAddrIn(socketAddrIn);

    } else {
        throw new logic_error("Unable to init socket");
    }
}

/**
 * Sockets Destructor
 */
SocketProvider::~SocketProvider() {}

/**
 * Set socket to non-blocking mode
 */
void SocketProvider::setSocketNonBlock(SOCKET sock) {
#if defined(OS_Linux)
    int flags = fcntl(sock, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(sock, F_SETFL, flags);
#endif
}

void SocketProvider::setNonBlock() {
    this->setSocketNonBlock(this->getSocket());
}

/**
 * Read data from the opened socket
 *
 * \return string - data read
 */
string SocketProvider::readSocketAsString(SOCKET sock, bool nonBlockingMode) {

    string response = "";
    char buffer[BUFFER_POOL_LENGHT];
    ssize_t nbBytes = 0;
    bool toContinue = true;

    if (nonBlockingMode) {
        this->setSocketNonBlock(sock);
    }

    while (toContinue) {
        // 	nbBytes = recv(sock, buffer, 1, 0); // Previously : read bytes by bytes until the end
        nbBytes = recv(sock, buffer, sizeof(buffer) - 1, 0); // Blocking sys-call by default
        if (0 == nbBytes) { // No data receive
            toContinue = false;
        } else if (nbBytes > 0) { // Data has been received
            buffer[nbBytes] = '\0'; // Terminate buffer
            response += string(buffer);
        } else { // There is an error => what it is
            int errorNumber = errno; // Save errno to avoid data lost
            switch (errorNumber) {
                #if defined(OS_Windows)
                case 0: // No data receive on Windows
                    toContinue = false;
                    break;
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
                    cerr << "Error number : " << errorNumber << "(" << nbBytes << " bytes receive)" << endl;
                    perror("Receive failed. ");
                    toContinue = false;
                    break;
            }
        }
    }

    return response;
}

string SocketProvider::readAsString(bool nonBlockingMode) {
    return this->readSocketAsString(this->getSocket(), nonBlockingMode);
}

/**
 * Put data to the opened socket
 */
bool SocketProvider::writeSocketAsString(SOCKET sock, string data) {
    if(-1 == send(
            sock,
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
 * Put data to the opened socket
 */
bool SocketProvider::writeAsString(string data) {
    return this->writeSocketAsString(this->getSocket(), data);
}
