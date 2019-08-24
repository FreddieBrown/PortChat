#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tools.h"

/**
 * @brief Get the local IP information of the computer
 *
 * This will make a UDP connection which will give back
 * information on the local IP of the machine. This is then
 * written to the buffer and the method finishes. The UDP
 * socket which was opened, is closed.
 *
 * @param buffer buffer which data is read into
 * @param buflen length of buffer which data is read into
 */
void get_primary_ip(char* buffer, size_t buflen)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	const char* kGoogleDnsIp = "8.8.8.8";
	uint16_t kDnsPort = 53;
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
	serv.sin_port = htons(kDnsPort);

	int err = connect(sock, (const struct sockaddr*) &serv, sizeof(serv));

	if (err) {
		fprintf(stderr, "Failed to connect to the socket.\n");
	}

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*) &name, &namelen);

	if (err) {
		fprintf(stderr, "Failed to get the socket name.\n");
	}

	inet_ntop(AF_INET, &name.sin_addr, buffer, buflen);

	close(sock);
}

/**
 * @brief Creates and sends a message
 *
 * Takes in a thread struct and uses it to send
 * messages via an open port from the commandline.
 *
 * @param arg Informaiton for the thread
 * @return void* struct which was passed into the function
 */
void* createMessage(void* arg){
	thread* info = (thread*) arg;
	char buffer[1024] = {0};
	// Read from command line, create message
	// and pass it to message queue
	while (1) {
		fgets (buffer, 100, stdin);
		send(info->socket, buffer, strlen(buffer), 0);

		if (!*(info->flag)){
			printf("Goodbye\n");
			return arg;
		}

		memset(buffer, 0, sizeof(buffer));
	}

	return arg;
}

/**
 * @brief Listens to open port
 *
 * Listens to an open port and will print any
 * input over this connection. It takes in a thread
 * struct as a void argument and uses this to find
 * the socket details.
 *
 * @param arg Informaiton for the thread
 * @return void* struct which was passed into the function
 */
void* readMessage(void* arg){
	thread* info = (thread*) arg;
	char buffer[1024] = {0};
	// Read from command line, create message
	// and pass it to message queue
	do {
		int valread = read(info->socket, buffer, 1024);

		if (!valread) {
			fprintf(stderr, "Failed to read from the socket into the buffer.\n");
		}

		char* exit = "exit\n";
		printf("From Client: %s", buffer);
		char data;
		*(info->flag) = strcmp(buffer, exit);

		if(!(*(info->flag))){
			close(info->socket);
			return arg;
		}

		recv(info->socket,&data,1, MSG_PEEK);
		memset(buffer, 0, sizeof(buffer));
	} while (1);

	return arg;
}
