/*
 * Server.cpp
 *
 *  Created on: 2010.02.27.
 *      Author: Agoston
 */

#include "Server.h"

Server::Server() {
	sockfd = 0;
	m_messageCallback = 0;
	listening = false;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		client_active[i] = false;
	}
	client_len = sizeof(client_addr);
	client_num = 0;
	strcpy(buffer, "");

	tv.tv_sec = 0;
	tv.tv_usec = 0;
}

void Server::SetMessageCallback(messageCallback cb) {
	m_messageCallback = cb;
}

bool Server::Listen(int portno) {
	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cerr << "ERROR opening socket" << std::endl;
		return false;
	}

	int on = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		std::cerr << "ERROR on setsockopt" << std::endl;
	}

	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		std::cerr << "ERROR on binding: " << errno << std::endl;
		switch (errno) {
		case EADDRINUSE:
			std::cerr << "EADDRINUSE" << std::endl;
			break;
		default:
			break;
		}
		return false;
	}

	/* Now start listening for the clients, here
	 * process will go in sleep mode and will wait
	 * for the incoming connection
	 */
	if (listen(sockfd, 5) < 0) {
		std::cerr << "ERROR on listen" << std::endl;
		return false;
	}
	listening = true;
	return true;
}

bool Server::Process() {
	if (!listening) {
		return false;
	}

	/*
	 * beallitjuk a figyelendo filedescriptorokat
	 */
	int max_fd = sockfd;
	FD_ZERO(&rfd);
	FD_SET(sockfd, &rfd);
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (client_active[i]) {
			FD_SET(client_sock[i], &rfd);
			if (client_sock[i] > max_fd)
				max_fd = client_sock[i];
		}
	}

	/*
	 * varunk amig nem tortenik valami
	 */
	switch (select(1 + max_fd, &rfd, 0, 0, &tv)) {
	case -1:
		std::cerr << "select error" << std::endl;
		return false;
		break;
	case 0:
		break;
	default:
		/*
		 * csatlakozott kliensek olvasasa
		 */
		for (int i = 0; i < MAX_CONNECTIONS; i++) {
			if (client_active[i]) {
				// csatlakozott kliens kuldott-e valamit
				if (FD_ISSET(client_sock[i], &rfd)) {
					// kiolvassuk milyen hosszu uzenet jott
					int size_read = recv(client_sock[i], buffer, sizeof(msglen_t), 0);
					if (size_read > 0) {
						msglen_t msg_len = *((msglen_t*) buffer);
						size_read = recv(client_sock[i], buffer, msg_len, 0);
						while (size_read > 0 && size_read < msg_len) {
							size_read += recv(client_sock[i], buffer+size_read, msg_len - size_read, 0);
						}
					}
					if (size_read < 1) {
						// disconnect
						//std::cout << i << " disconnected" << std::endl;
						client_active[i] = false;
						if (m_messageCallback != 0) {
							(*m_messageCallback)(i, "", 0);
						}
						client_num--;
						close(client_sock[i]);
					} else {
						//this->Trim(buffer);
						if (m_messageCallback != 0) {
							(*m_messageCallback)(i, buffer, size_read);
						}
					}
				}
			}
		}
		/*
		 * uj kliens szeretne csatlakozni
		 */
		if (FD_ISSET(sockfd, &rfd)) {
			client_num++;
			//std::cout << "Incoming connection: " << client_num << std::endl;
			//alapbol az utolso, fenntartott helyre csatlakoztatjuk
			int next_client = MAX_CONNECTIONS - 1;
			// keresunk egy szabad helyet neki
			if (client_num < MAX_CONNECTIONS) {
				for (int i = 0; i < MAX_CONNECTIONS - 1; i++) {
					if (!client_active[i]) {
						next_client = i;
						break;
					}
				}
			}
			client_sock[next_client] = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t*) &client_len);
			if (client_sock[next_client] < 0) {
				std::cerr << "ERROR on accept " << next_client << std::endl;
				close(client_sock[next_client]);
				client_num--;
			} else if (next_client == MAX_CONNECTIONS - 1) {
				// ha csak az utolso helyre sikerult csatlakoztatni ledobjuk
				strcpy(buffer, "Too many connections\r\n");
				send(client_sock[next_client], buffer, strlen(buffer), 0);
				close(client_sock[next_client]);
				client_num--;
			} else {
				client_active[next_client] = true;
			}
		}
	}
	return true;
}

bool Server::Send(int n, const void* message, msglen_t size) {
	if (client_active[n]) {
		if (send(client_sock[n], &size, sizeof(msglen_t), 0) < 0) {
			std::cerr << n << " send error" << std::endl;
			return false;
		}
		if (send(client_sock[n], message, size, 0) < 0) {
			std::cerr << n << " send error" << std::endl;
			return false;
		}
		return true;
	}
	return false;
}

Server::~Server() {
	//std::cout << "Szerver kilep" << std::endl;
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (client_active[i]) {
			close(client_sock[i]);
		}
	}

	if (shutdown(sockfd, SHUT_RDWR)) {
		std::cerr << "ERROR on shutdown" << std::endl;
	}
	if (close(sockfd)) {
		std::cerr << "ERROR on close" << std::endl;
	}
}
