//
// Created by mfranc on 25/11/15.
//

#include "Network.h"

/**
 * Resolv IP to do querying a valid target
 *
 * \param string hostname - IP or DNS record must be resolv
 */
string Network::resolvAddress(string hostname) {
    //setup address structure
    if(-1 == inet_addr(hostname.c_str())) {
        struct hostent *he;
        struct in_addr **addr_list;
        //resolve the hostname, its not an ip address
        if ( NULL == (he = gethostbyname(hostname.c_str() ) )) {
            //gethostbyname failed
            throw logic_error("Unable to resolv hostname : " + hostname);
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;
        string result;
        for(int i = 0; addr_list[i] != NULL; i++) {
            result = inet_ntoa(*addr_list[i]);
            break;
        }
        return string(result);
    }

    //plain ip address
    return hostname;
}