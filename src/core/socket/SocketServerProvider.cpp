//
// Created by Pilebones on 11/09/2015.
//

#include "SocketServerProvider.h"

/**
 * Sockets server Constructor
 */
SocketServerProvider::SocketServerProvider(string hostname, int port) : SocketProvider(hostname, port) {
    SOCKADDR_IN socketAddrIn = this->getSocketAddrIn();
    bind(this->getSocket(), (SOCKADDR *)& socketAddrIn, sizeof(socketAddrIn));
    int returnCode = listen(this->getSocket(), 0);
    if(-1 == returnCode) {
        throw logic_error("Unable to listen on this port !");
    }
}

/**
 * Sockets Destructor
 */
SocketServerProvider::~SocketServerProvider() {}

void SocketServerProvider::start() {

    SocketWrapper * socketWrapper = new SocketWrapper();
    SOCKADDR_IN socketAddrIn = socketWrapper->getSocketAddrIn();

    while(1) {
        socklen_t socketAddrInClientSize = sizeof(socketAddrIn);
        socketWrapper->setSocket(accept(this->getSocket(), (SOCKADDR *)&socketAddrIn, &socketAddrInClientSize));

        if(INVALID_SOCKET != socketWrapper->getSocket()) {
            this->addSocketClient(socketWrapper->getSlug(), socketWrapper);
            this->displayWelcomeToSocketWrapperBySlug(socketWrapper->getSlug());
        }
    }
}

SocketWrapper SocketServerProvider::getSocketWrapperBySlug(string clientSlug) {
    if(!this->hasSocketWrapperBySlug(clientSlug)) {
        ostringstream stream;
        stream << "No client for slug : " << clientSlug;

        throw logic_error(stream.str());
    }

    SocketWrapper socketWrapper = this->getSocketClientMap().find(clientSlug)->second;
    return socketWrapper;
}

void SocketServerProvider::displayWelcomeToSocketWrapperBySlug(string clientSlug) {

    SocketWrapper socketWrapper = this->getSocketWrapperBySlug(clientSlug);
    ostringstream stream;
    stream
    << "Welcome client ID \"" << socketWrapper.getSlug() << '\"' << endl
    << this->getSocketClientMap().size() << " User(s) connected :" << endl;

    map <string, SocketWrapper>::iterator iterator;
    for(iterator = this->getSocketClientMap().begin(); iterator != this->getSocketClientMap().end(); iterator++)
    {
        stream << "- ID : " << (*iterator).first << " / <IP:PORT> : " << (*iterator).second.getIp() << ":" << (*iterator).second.getPort();
        if ((*iterator).first == socketWrapper.getSlug()) {
            stream << " (you)" ;
        }
        stream << endl;
    }
    cout << stream.str();

    this->writeToClientAsString(socketWrapper.getSlug(), stream.str());

    ostringstream streamToBroadcast;
    streamToBroadcast
        << endl
        << ">>>> Broadcasted Message <<<<" << endl
        << "/!\\ Another client join server : " << endl
        << " - ID       = " << socketWrapper.getSocket() << endl
        << " - IP/PORT  = " << socketWrapper.getIp() << ":" << socketWrapper.getPort() << endl
        << ">>>> END Broadcasted Message <<<<" << endl
        << endl;

    this->broadcastMessage(streamToBroadcast.str());

    cout << streamToBroadcast.str();
}

bool SocketServerProvider::hasSocketWrapperBySlug(string clientSlug) {
    if(!this->getSocketClientMap().empty())
    {
        if(this->getSocketClientMap().find(clientSlug)->first != clientSlug)
        {
            return false;
        }
        else
            return true;
    }
    return false;
}

map<string, SocketWrapper> & SocketServerProvider::getSocketClientMap() {
    return this->socketClientMap;
};

SocketServerProvider & SocketServerProvider::setSocketClientMap(map<string, SocketWrapper> socketClientMap) {
    this->socketClientMap = socketClientMap;
    return * this;
}

void SocketServerProvider::addSocketClient(string clientSlug, SocketWrapper * socketClient) {
    this->getSocketClientMap().insert(pair<string,SocketWrapper>(clientSlug, * socketClient));
}

SocketWrapper SocketServerProvider::kickSocketClient(string clientSlug) {
    this->getSocketClientMap().erase(clientSlug);
}

/**
 * Put data to the opened socket
 */
bool SocketServerProvider::writeToClientAsString(string clientSlug, string data) {

    if(-1 == send(
        this->getSocketWrapperBySlug(clientSlug).getSocket(),
        data.c_str(),
        data.length(),
        0
    )) {
        perror("Send failed. Error");
        return false;
    }
    return true;
}

void SocketServerProvider::broadcastMessage(string data) {

    map <string, SocketWrapper>::iterator iterator;
    for(iterator = this->getSocketClientMap().begin(); iterator != this->getSocketClientMap().end(); iterator++)
    {
        SocketWrapper socketWrapper = (*iterator).second;
        this->writeToClientAsString(socketWrapper.getSlug(), data);
    }
}