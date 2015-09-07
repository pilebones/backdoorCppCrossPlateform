//============================================================================
// Name        : client.cpp
// Author      : pilebones
// Version     :
// Description : Socket client C++, Ansi-style
//============================================================================

#include "SocketClientProvider.h"
#include <iostream>
#include <string>

using namespace std;

void displayUsage(char* name) {
	cerr 	<< "Usage: " << endl
			<< name << " <host> <port>" << endl
			<< endl;
}

int checkArguments(int argc, char* argv[]) {
	if (3 != argc) {
		cerr << "Missing some required arguments !" << endl;
		displayUsage(argv[0]);
		return -1;
	}

	try {
		int port = atoi(argv[2]);
	} catch (exception e) {
		cerr << "Port must be an integer !" << endl;
		displayUsage(argv[0]);
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[]) {

	// Validate arguments
	int isValid;
	isValid = checkArguments(argc, argv);
	if (0 != isValid) {
		return -1;
	}

	// Retrieve arguments
	string host = string(argv[1]);
	int port = atoi(argv[2]);

	cout << "Init client connection" << endl;
	try {
		/**
		 * \var client SocketClientProvider
		 */
		SocketClientProvider* client = new SocketClientProvider();

		// Resolv target before connect
		string hostname = client->resolvAddress(host);
		if (host != hostname) {
			cout << "Target resolved to " << hostname << endl;
		}

		bool status = client->connection(hostname, port);
		if(status) {
			cout << "Connected to " << hostname << ":" << port << endl;

			string data;
			/*string data = client->readAsString();
			if (0 < data.size()) {
				cout << "Read : " << endl << data << endl;
			} else {
				cout << "Nothing to read" << endl;
			}*/

			string raw = "GET / HTTP/1.1\r\n"
					"Host: www.mickael-franc.fr\r\n"
					"\r\n";

			bool isSent = client->writeAsString(raw);
			if (true == isSent) {
				cout << "Write : " << endl << raw << endl;
			} else {
				cout << "Unable to write : " << endl << raw << endl;
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
	// getchar();
	return 0;
}
