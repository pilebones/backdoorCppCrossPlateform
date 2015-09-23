//==================core/socket/==========================================================
// Name        : main.cpp
// Author      : pilebones
// Version     : 0.2
// Description : backdoorCppCrossPlateform C++, Ansi-style
//============================================================================

#include <string>
#include <iostream>
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
        if (verbose) cout << "Init server mode : feature not fully implemented yet work in progress" << endl;
        // Init provider
        SocketServerProvider* server = new SocketServerProvider(host, port);
        server->start();
    } else {
        if (verbose) cout << "Init client mode" << endl;
        try {

            // Resolv target before connect
            string hostname = SocketClientProvider::resolvAddress(host);
            if (host != hostname) {
                cout << "Target resolved to " << hostname << endl;
            }

            // Init provider
            SocketClientProvider* client = new SocketClientProvider(hostname, port, SOCKET_TIMEOUT);
            // Init connection
            bool status = client->connection();
            if(status) {
                cout << "Connected to " << hostname << ":" << port << endl << endl;

                string data;
                data = client->readAsString(); // Read first response after connection
                if (0 < data.size()) {
                    cout << endl << data << endl;
                }

                cout << "Please input what you want to send..." << endl;
                cout << "> ";

                string raw;
                char const crlf[] = "\x0D\x0A";
                while(getline(cin, raw)) {    // get message from stdin
                    // Clean properly stdin
                    cin.clear();
                    //  Ajout du special char CRLF at the end of string for query likes HTTP for example
                    raw.append(crlf);

                    // Send request for testing
                    /*
                    raw = "GET / HTTP/1.1\r\n"
                        "Host: " + host + "\r\n"
                        "Connection: close\r\n"
                        "\r\n";
                    */

                    bool isSent = client->writeAsString(raw);
                    if (!isSent) {
                        cerr << "Unable to send : " << endl << raw << endl;
                        break;
                    } else {
                        if (verbose) cout << raw << endl;
                    }

                    data.clear();
                    data = client->readAsString();
                    if (0 < data.size()) {
                        cout << data << endl;
                    }

                    cout << "> ";
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
