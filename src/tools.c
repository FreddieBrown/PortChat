#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tools.h"

#define MAX_MESSAGE_LEN 256

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
void* create_message(void* arg){
	thread* info = (thread*) arg;

	// Allocate a buffer to read into
	char* buffer = calloc(MAX_MESSAGE_LEN, sizeof(char));

	while (1) { 
		// Read the message in from the command line
		fgets(buffer, MAX_MESSAGE_LEN, stdin);

		// Calculate the size of the resulting formatted message
		size_t msg_len = strlen(buffer);
		size_t hst_len = strlen(info->host);
		size_t fmt_len = msg_len + hst_len + 2;

		// Allocate enough memory for the message
		char* formatted_buffer = calloc(fmt_len, sizeof(char));

		// Format the message and send it through the socket
        sprintf(formatted_buffer, "%s: %s", info->host, buffer);
		send(info->socket, formatted_buffer, fmt_len, 0);

		// If something went wrong with the socket connection, free the
		// memory
		if (!*(info->flag)){
			free(buffer);
			free(formatted_buffer);
			printf("Goodbye\n");
			exit(EXIT_SUCCESS);
		}

		// Clear the buffer and free the formatted one
		memset(buffer, 0, strlen(buffer));
		free(formatted_buffer);
	}

	// Free the original buffer we made for messages
	free(buffer);

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
void* read_message(void* arg){
	thread* info = (thread*) arg;
	char buffer[1024] = {0};
	// Read from command line, create message
	// and pass it to message queue
	do {
		int valread = read(info->socket, buffer, 1024);

		if (!valread) {
			fprintf(stderr, "Lost connection to client\n");
            exit(EXIT_SUCCESS);
		}

		char* exit = "exit\n";
		printf("%s", buffer);
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
