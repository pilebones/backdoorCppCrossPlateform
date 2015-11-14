//
// Created by mfranc on 18/09/15.
//

#include "Interface.h"

Interface::Interface(int argc, const char** argv) {
    this->setFlagIsValidOptions(
        this->setArgc(argc)
                .setArgv(argv)
                .initProgramOptions()
    );
}

/**
 * Initialize programm option from CLI with C++ Boost lib
 */
bool Interface::initProgramOptions() {

    int argc = this->getArgc();
    const char** argv = this->getArgv();

    po::options_description optDesc("Usage");
    optDesc.add_options()
            ("help,h", "Display usage")
            ("version,V", "Display version of programme")
            ("verbose,v", "Enable mode verbose")
            ("listen,l", "Enable listen mode (server socket mode)")
            ("port,p", po::value<int>()->required(), "Set port number to use")
            ("host,H", po::value<string>()->required(), "Set hostname to use")
            ("input-file,f", po::value< vector<string> >(), "File wich conains query to send (doesn't work with -l option)")
            ;

    po::variables_map variablesMap;

    try{
        po::store(po::parse_command_line(argc, argv, optDesc), variablesMap);
        po::notify(variablesMap);

        if (variablesMap.count("help")) {
            cout << optDesc << endl;
            return false;
        }

        if (variablesMap.count("version")) {
            cout    << PRODUCT_NAME << " : Build " << BUILD << " Version " << VERSION << endl
            << "Author : " << AUTHOR << " (see: " << CONTACT << ")" << endl;
            return false;
        }

        if (variablesMap.count("verbose")) {
            cout << "Mode verbose enable" << endl;
            this->setModeVerbose(true);
        }

        if (variablesMap.count("listen")) {
            cout << "Mode listen enable" << endl;
            this->setModeListen(true);
        }

        if (variablesMap.count("input-file")) {
            if (variablesMap.count("listen")) throw std::logic_error("Unable to use -l|--listen option with -f|--input-file option");

            this->setFilePath(variablesMap["input-file"].as<string>());
        }

        /*if (!variablesMap.count("port") || !variablesMap.count("host")) {
            throw logic_error("You must define hostname and port to use");
        }*/

        this->setHost(variablesMap["host"].as<string>());
        this->setPort(variablesMap["port"].as<int>());

        if (1 == argc) {
            char buffer[100];
            string message = "Missing some required arguments ! (nb passed: %d)";
            const char* formatStringMessage = message.c_str();
            int param1 = argc - 1;
            snprintf(buffer, sizeof(buffer), formatStringMessage, param1);
            throw logic_error(buffer);
        }

    } catch(exception &e) {
        cerr << "Error: "
        << e.what() << endl
        << optDesc << endl ;
        return false;
    }

    return true;
}

int Interface::getArgc() {
    return this->argc;
}

Interface & Interface::setArgc(int argc) {
    this->argc = argc;
    return *this;
}

const char** Interface::getArgv() const {
    return this->argv;
}

Interface & Interface::setArgv(const char** argv) {
    this->argv = argv;
    return *this;
}

const string& Interface::getHost() {
    return this->host;
}

Interface & Interface::setHost(const string &host) {
    this->host = host;
    return *this;
}

int Interface::getPort() {
    return this->port;
}

Interface & Interface::setPort(int port) {
    this->port = port;
    return *this;
}

bool Interface::isValidOptions() {
    return this->flagIsValidOptions;
}

Interface & Interface::setFlagIsValidOptions(bool flag) {
    this->flagIsValidOptions = flag;
    return *this;
}

bool Interface::isModeListenEnabled() const {
    return modeListen;
}

Interface & Interface::setModeListen(bool flag) {
    this->modeListen = flag;
    return *this;
}

bool Interface::isModeVerboseEnabled() const {
    return modeVerbose;
}

Interface & Interface::setModeVerbose(bool flag) {
    this->modeVerbose = flag;
    return *this;
}

const string & Interface::getFilePath() const {
    return this->filePath;
}

Interface & Interface::setFilePath(const string &filePath) {
    this->filePath = filePath;
    return *this;
}
