#ifndef NET_H_
#define NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Protocol/Protocol.h"

class Net {
	public:
		Net();
		virtual ~Net();

		bool ConnectToHost(int PortNo, const char* IPAddress);
		void CloseConnection();
		bool IsConnected();
		bool Send(void* c, msglen_t size);
		int Receive(void* c, msglen_t size, long int useconds);

	protected:

		int s;

};

#endif /*NET_H_*/
