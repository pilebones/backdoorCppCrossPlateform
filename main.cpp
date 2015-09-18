//============================================================================
// Name        : main.cpp
// Author      : pilebones
// Version     :
// Description : Socket client C++, Ansi-style
//============================================================================

#include "core/socket/SocketClientProvider.h"
#include "core/socket/SocketServerProvider.h"
#include "core/utils/Array.h"
#include <boost/program_options.hpp>
#include <string>
#include <ctype.h>
#include <getopt.h>

#define SOCKET_TIMEOUT 2

using namespace std;
using namespace boost::program_options;


/**
 * Display usage for help
 */
void displayUsage(char* name) {
    cerr 	<< "Usage: " << endl
    << name << " -p <port> -h <host> [-l] [-f <path>] [-h] [-v]" << endl
    << endl << "Details :" << endl
        << "-h|--help : Display usage" << endl
        << "-v|--verbose : Enable mode verbose" << endl
        << "-l|--listen : Enable listen mode (server socket mode)" << endl
        << "-p|--port <port> : Connection to or from this port" << endl
        << "-H|--host <host> : Connection to or from this host" << endl
        << "-f|--file <path> : File wich conains query to send (doesn't work with -l option)" << endl
    << endl;
}

/**
 * Initialize programm option from CLI with C++ Boost lib
 */
bool initProgramOptions(int argc, char* argv[]) {

    namespace po = boost::program_options;
    po::options_description optDesc("Allowed options");
    optDesc.add_options()
            ("version,v", "Display version of programme")
            ("help,h", "Display usage message")
            ("port,p", po::value<int>(), "Set port number")
            ("host,H", po::value<string>(), "Set hostname")
    ;

    po::variables_map variablesMap;

    try{
        po::store(po::parse_command_line(argc, argv, optDesc), variablesMap);
        po::notify(variablesMap);

        if (variablesMap.count("help")) {
            cout << optDesc << endl;
        }

    } catch(exception &e) {
        cerr << "Error: "
            << e.what() << endl
            << optDesc << endl ;
        return false;
    }

    if (1 == argc) {
        cerr << "Missing some required arguments ! (nb passed: " << (argc - 1) << ")" << endl;
        cout << optDesc << endl;
        return false;
    }

    return true;
}

/**
 * Main runner
 */
int main(int argc, char* argv[]) {

    // Validate arguments
    if (!initProgramOptions(argc, argv)) {
        return -1;
    }

    return -1;
    // Retrieve arguments
    string host = string(argv[1]);
    int port = atoi(argv[2]);

    cout << "Init client connection" << endl;
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
    } catch (exception e) {
        cerr << "Error: " << e.what() << endl;
    }

    cout << "Program ending.." << endl;
    return 0;
}
