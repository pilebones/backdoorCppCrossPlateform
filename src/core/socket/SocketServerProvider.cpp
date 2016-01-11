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

        // this->manageQueries();
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

void SocketServerProvider::manageQueries() {

    map <string, SocketWrapper>::iterator iterator;
    for(iterator = this->getSocketClientMap().begin(); iterator != this->getSocketClientMap().end(); iterator++)
    {
        string message = this->readFromClientAsString((*iterator).second.getSlug());
        if (message.length() > 0) {
            cout << "Message From " << (*iterator).first << message << endl;
        }
    }
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
 * Put data to the opened socket client
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

/**
 * Read data to the opened socket client
 */
string SocketServerProvider::readFromClientAsString(string clientSlug) {
    SOCKET clientSocket = this->getSocketWrapperBySlug(clientSlug).getSocket();
    string response;
    char buffer[BUFFER_POOL_LENGHT];
    ssize_t nbBytes = 0;
    bool toContinue = true;

    while (toContinue) {
        // 	nbBytes = recv(this->getSocket(), buffer, 1, 0); // Previously : read bytes by bytes until the end
        nbBytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); // Blocking sys-call by default
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

void SocketServerProvider::broadcastMessage(string data) {

    map <string, SocketWrapper>::iterator iterator;
    for(iterator = this->getSocketClientMap().begin(); iterator != this->getSocketClientMap().end(); iterator++)
    {
        SocketWrapper socketWrapper = (*iterator).second;
        this->writeToClientAsString(socketWrapper.getSlug(), data);
    }
}