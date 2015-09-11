//============================================================================
// Name        : client.cpp
// Author      : pilebones
// Version     :
// Description : Socket client C++, Ansi-style
//============================================================================

#include "SocketClientProvider.h"
#include <string>

#define SOCKET_TIMEOUT 2

using namespace std;

/**
 * Display usage for help
 */
void displayUsage(char* name) {
	cerr 	<< "Usage: " << endl
			<< name << " <host> <port>" << endl
			<< endl;
}

/**
 * Check/validate all argument passed to script
 */
bool checkArguments(int argc, char* argv[]) {
	if (3 != argc) {
		cerr << "Missing some required arguments !" << endl;
		displayUsage(argv[0]);
		return false;
	}

	try {
		int port = atoi(argv[2]);
	} catch (exception e) {
		cerr << "Port must be an integer !" << endl;
		displayUsage(argv[0]);
		return false;
	}

	return true;
}

/**
 * Main runner
 */
int main(int argc, char* argv[]) {

	// Validate arguments
	if (!checkArguments(argc, argv)) {
		return -1;
	}

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
