//
// Created by Pilebones on 11/09/2015.
//

#include "SocketServerProvider.h"

/**
 * Sockets server Constructor
 */
SocketServerProvider::SocketServerProvider(string hostname, int port) : SocketProvider(hostname, port) {
    SOCKADDR_IN socketAddrIn = this->getSocketAddrIn();

    // Check if the port is still treated as "being in use" by the OS
    int reuse = 1;
    int returnCode = setsockopt(this->getSocket(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (0 == returnCode) {
        returnCode = bind(this->getSocket(), (SOCKADDR *)& socketAddrIn, sizeof(socketAddrIn));
    } else {
        // cerr << "The port is still treated as \"being in use\" by the OS" << endl;
    }

    returnCode = listen(this->getSocket(), 0);
    if(-1 == returnCode) {
        throw logic_error("Unable to listen on this port !");
    }
}

/**
 * Sockets Destructor
 */
SocketServerProvider::~SocketServerProvider() {
    /* Close socket with same function name for each OS (see: macro inside *.h) */
    closesocket(this->getSocket());

    /* Close only on Windows socket extra-setting */
    #if defined(OS_Windows)
        WSACleanup();
    #endif
}

/**
 * Start the server bound to the host:port passed to the constructor
 */
void SocketServerProvider::start() {

    SocketWrapper * socketWrapper = new SocketWrapper();
    SOCKADDR_IN socketAddrIn = socketWrapper->getSocketAddrIn();

    while(1) {
        socklen_t socketAddrInClientSize = sizeof(socketAddrIn);
        socketWrapper->setSocket(accept(this->getSocket(), (SOCKADDR *)&socketAddrIn, &socketAddrInClientSize));

        if(INVALID_SOCKET != socketWrapper->getSocket()) {
            cout << "[Connection] New client : "<< socketWrapper->getSlug() << endl;
            this->addSocketClient(socketWrapper->getSlug(), socketWrapper);
            this->displayWelcomeToSocketWrapperBySlug(socketWrapper->getSlug());
        }

        this->manageQueries();
    }
}

/**
 * Return the client object if a client related to slug in parameter is connected
 */
SocketWrapper SocketServerProvider::getSocketWrapperBySlug(string clientSlug) {
    if(!this->hasSocketWrapperBySlug(clientSlug)) {
        ostringstream stream;
        stream << "No client for slug : " << clientSlug;

        throw logic_error(stream.str());
    }

    SocketWrapper socketWrapper = this->getSocketClientMap().find(clientSlug)->second;
    return socketWrapper;
}

/**
 * Read All client sockets and display message if receive
 */
void SocketServerProvider::manageQueries() {

    map <string, SocketWrapper>::iterator iterator;
    for(iterator = this->getSocketClientMap().begin(); iterator != this->getSocketClientMap().end(); iterator++)
    {
        string message = this->readFromClientAsString((*iterator).second.getSlug());
        if (message.length() > 0) {
            cout << "[Message][Client][" << (*iterator).first << "] " << message << endl;
        } else {
            // cout << "No message from " << (*iterator).first << endl;
        }
    }
}

/**
 * Sent to a connected client the welcome message
 */
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

    // cout << stream.str();
    cout << "[Message][Server]["<< socketWrapper.getSlug() << "] Connected users list message" << endl;

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

    // cout << streamToBroadcast.str();
    cout << "[Message][Server][Broadcast] Welcome message" << endl;
}

/**
 * Return true if a client related to slug in parameter is connected
 */
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

/**
 * Add client socket to the list of client
 */
void SocketServerProvider::addSocketClient(string clientSlug, SocketWrapper * socketClient) {
    this->getSocketClientMap().insert(pair<string,SocketWrapper>(clientSlug, * socketClient));
}

/**
 * Kick a connected client
 */
SocketWrapper SocketServerProvider::kickSocketClient(string clientSlug) {
    this->getSocketClientMap().erase(clientSlug);
}

/**
 * Put data to the opened socket client
 */
bool SocketServerProvider::writeToClientAsString(string clientSlug, string data) {
    SocketWrapper socketWrapper = this->getSocketWrapperBySlug(clientSlug);
    SOCKET clientSocket = socketWrapper.getSocket();
    return this->writeSocketAsString(clientSocket, data);
}

/**
 * Read data to the opened socket client
 */
string SocketServerProvider::readFromClientAsString(string clientSlug) {
    SocketWrapper socketWrapper = this->getSocketWrapperBySlug(clientSlug);
    SOCKET clientSocket = socketWrapper.getSocket();
    return this->readSocketAsString(clientSocket);
}

/**
 * Put data to every client socket
 */
void SocketServerProvider::broadcastMessage(string data) {

    map <string, SocketWrapper>::iterator iterator;
    for(iterator = this->getSocketClientMap().begin(); iterator != this->getSocketClientMap().end(); iterator++)
    {
        SocketWrapper socketWrapper = (*iterator).second;
        this->writeToClientAsString(socketWrapper.getSlug(), data);
    }
}