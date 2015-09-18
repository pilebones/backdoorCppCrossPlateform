//============================================================================
// Name        : main.cpp
// Author      : pilebones
// Version     :
// Description : Socket client C++, Ansi-style
//============================================================================

#include <string>
#include "core/cli/Interface.h"
#include "core/socket/SocketClientProvider.h"
#include "core/socket/SocketServerProvider.h"

#define SOCKET_TIMEOUT 2

/**
 * Main runner
 */
int main(int argc, const char** argv) {

    Interface* cli = new Interface(argc, argv);

    // Validate arguments
    if (! cli->isValidOptions()) {
        return -1;
    }

    string host     = cli->getHost();
    int port        = cli->getPort();
    string filePath = cli->getFilePath();
    bool verbose    = cli->isModeVerboseEnabled();
    bool modeListen = cli->isModeListenEnabled();

    if (modeListen) {
        if (verbose) cout << "Init server mode : feature not implemented yet" << endl;
        return -1;
    } else {
        if (verbose) cout << "Init client mode" << endl;
        try {
            // Init provider
            SocketClientProvider* client = new SocketClientProvider(SOCKET_TIMEOUT);
            // Resolv target before connect
            string hostname = client->resolvAddress(host);
            if (host != hostname) {
                cout << "Target resolved to " << hostname << endl;
            }
            // Init connection
            bool status = client->connection(hostname, port);
            if(status) {
                cout << "Connected to " << hostname << ":" << port << endl << endl;

                string data;
                data = client->readAsString(); // Read first response after connection
                if (0 < data.size()) {
                    cout << endl << data << endl;
                }

                // Send request
                string raw = "GET / HTTP/1.1\r\n"
                        "Host: www.google.fr\r\n"
                        "Connection: close\r\n"
                        "\r\n";

                bool isSent = client->writeAsString(raw);
                if (isSent) {
                    cout << "Data sent : " << endl << raw << endl;
                } else {
                    cout << "Unable to send : " << endl << raw << endl;
                }

                data = client->readAsString();
                if (0 < data.size()) {
                    cout << data << endl;
                }

                delete client;
            } else {
                cout << "Unable to connect to " << host << ":" << port << endl;
            }
        } catch (exception & e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    cout << "Program ending.." << endl;
    return 0;
}
