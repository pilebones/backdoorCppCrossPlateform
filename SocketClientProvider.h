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
        SocketClientProvider(int timeout = 50);
		virtual ~SocketClientProvider();

		bool connection(string host,int port);
	};

#endif /* SOCKETCLIENTPROVIDER_H_ */
