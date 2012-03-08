#include "Net.h"

Net::Net() {
	s = 0;
}

Net::~Net() {
}

bool Net::ConnectToHost(int PortNo, const char* IPAddress) {
	struct sockaddr_in serverAddress;
	struct hostent *hostInfo;

	hostInfo = gethostbyname(IPAddress);
	if (hostInfo == NULL) {
		return false;
	}
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		return false;
	}
	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(PortNo);

	if (connect(s, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		return false;
	}
	return true;
}

void Net::CloseConnection() {
	//Close the socket if it exists
	if (s > 0)
		close(s);
	s = 0;
}

bool Net::IsConnected() {
	if (s > 0)
		return true;
	return false;
}

bool Net::Send(void* c, msglen_t size) {
	if (s > 0) {
		if (send(s, &size, sizeof(msglen_t), 0) >= 0 && send(s, c, size, 0) >= 0) {
			return true;
		}
	}
	return false;
}

int Net::Receive(void* c, msglen_t size, long int useconds) {
	if (s > 0) {
		struct timeval tv;
		fd_set rfd;

		tv.tv_sec = 0;
		tv.tv_usec = useconds;
		FD_ZERO(&rfd);
		FD_SET(s, &rfd);
		memset(c, 0, size);

		switch (select(1 + s, &rfd, 0, 0, &tv)) {
		case -1:
			std::cerr << "select error" << std::endl;
			return -1;
			break;
		case 0:
			return 0;
			break;
		default:
			if (FD_ISSET(s, &rfd)) {
				int size_read = recv(s, c, sizeof(msglen_t), 0);
				msglen_t msg_len = *((msglen_t*) c);
				size_read = recv(s, c, msg_len, 0);
				if (size_read < 1) {
					CloseConnection();
					return -1;
				}
				return size_read;
			}
			break;
		}
	}
	return 0;
}
