//
// Created by mfranc on 25/11/15.
//

#include "SocketWrapper.h"


/**
 * Generate a token like a unique socket string identifier (slug)
 *
 * \return SOCKADDR_IN the socket
 */
string SocketWrapper::getSlug() {
    string addr = this->getIp();
    int port = this->getPort();

    ostringstream stream;
    stream << this->getSocket() << '-' << addr << ':' << port;
    return  stream.str();
}

string SocketWrapper::getIp() {
    return string(inet_ntoa(this->getSocketAddrIn().sin_addr));
}

int SocketWrapper::getPort() {
    return (int) this->getSocketAddrIn().sin_port;
}

/**
 * Getter for the socket
 *
 * \return SOCKET the socket
 */
SOCKET SocketWrapper::getSocket() {
    return this->handle;
}

/**
 * Setter for the socket
 * \param SOCKET handle - Init socket
 */
SocketWrapper & SocketWrapper::setSocket(SOCKET handle) {
    this->handle = handle;
    return * this;
}
/**
 * Getter for the socket addr in
 *
 * \return SOCKADDR_IN the socket
 */
SOCKADDR_IN SocketWrapper::getSocketAddrIn() {
    return this->socketAddrIn;
}

/**
 * Setter for the socket
 * \param SOCKADDR_IN socketAddrIn - Init socket addr in
 */
SocketWrapper & SocketWrapper::setSocketAddrIn(SOCKADDR_IN socketAddrIn) {
    this->socketAddrIn = socketAddrIn;
    return * this;
}

/**
 * Getter for the socket addr in
 *
 * \return SOCKADDR_IN the socket
 */
SOCKADDR SocketWrapper::getSocketAddr() {
    return this->socketAddr;
}

/**
 * Setter for the socket
 * \param SOCKADDR socketAddr - Init socket addr
 */
SocketWrapper & SocketWrapper::setSocketAddr(SOCKADDR socketAddr) {
    this->socketAddr = socketAddr;
    return * this;
}

