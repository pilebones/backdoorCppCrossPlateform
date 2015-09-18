//
// Created by mfranc on 18/09/15.
//

#ifndef CPPCROSSPLATEFORMSOCKETPROVIDER_INTERFACE_H
#define CPPCROSSPLATEFORMSOCKETPROVIDER_INTERFACE_H

    #include <boost/program_options.hpp>
    #include <iostream>
    #include <stdexcept>
    #include <string>
    #include <ctype.h>

    #define VERSION 0.2
    #define BUILD "2015-09-18"
    #define PRODUCT_NAME "CppCrossPlateformSocketProvider"
    #define AUTHOR "Pilebones"
    #define CONTACT "http://www.mickael.franc.fr"

    using namespace std;
    using namespace boost::program_options;
    namespace po = boost::program_options;

    class Interface {

        private:
            int argc;
            const char** argv;
            bool flagIsValidOptions;

            string host;
            int port;
            bool modeListen = false;
            bool modeVerbose;
            string filePath;

        public:
            Interface(int argc, const char** argv);
            bool initProgramOptions();

            int getArgc();
            Interface & setArgc(int argc);

            const char** getArgv() const;
            Interface & setArgv(const char** argv);

            const string & getHost();
            Interface & setHost(const string &host);

            int getPort();
            Interface & setPort(int port);

            bool isValidOptions();
            Interface & setFlagIsValidOptions(bool flag);

            bool isModeListenEnabled() const;
            Interface & setModeListen(bool flag);

            bool isModeVerboseEnabled() const;
            Interface & setModeVerbose(bool flag);

            const string & getFilePath() const;
            Interface & setFilePath(const string &filePath);
    };

#endif //CPPCROSSPLATEFORMSOCKETPROVIDER_INTERFACE_H
