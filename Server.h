/*
 * Server.h
 *
 *  Created on: 2010.02.27.
 *      Author: Agoston
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>

#include "Primitives/Net/Protocol/Protocol.h"

#define MAX_CONNECTIONS 10

typedef void (*messageCallback)(int n, const void* message, msglen_t size);

class Server {
	public:
		Server();
		virtual void setMessageCallback(messageCallback cb);
		bool Listen(int portno);
		bool Process();
		bool Send(int n, const void* message, msglen_t size);
		virtual ~Server();
	private:

		messageCallback m_messageCallback;

		bool listening;
		int sockfd, client_sock[MAX_CONNECTIONS], client_len;
		struct sockaddr_in serv_addr, client_addr;
		bool client_active[MAX_CONNECTIONS];
		int client_num;
		struct timeval tv;
		fd_set rfd;
		char buffer[256];
};

#endif /* SERVER_H_ */
