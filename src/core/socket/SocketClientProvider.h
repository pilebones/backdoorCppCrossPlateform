/*
 * SocketClientProvider.h
 *
 *  Created on: 3 sept. 2015
 *      Author: Pilebones
 */

#ifndef SOCKETCLIENTPROVIDER_H_
#define SOCKETCLIENTPROVIDER_H_

    #include "SocketProvider.h"

    class SocketClientProvider : public SocketProvider  {
	public:
        SocketClientProvider(string host,int port, int timeout = 50);
		virtual ~SocketClientProvider();

        bool connection();
        template <class T> void setTimeout(SOCKET handle, T timeout);
	};

#endif /* SOCKETCLIENTPROVIDER_H_ */
