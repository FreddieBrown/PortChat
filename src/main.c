#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "server.h"
#include "tools.h"

void client(int port, char * addr);
void server(int port);
/**
 * @brief Main function
 *
 * This function will create the thread structs and will
 * start up the threads. It will then wait for them to
 * finish before clearing up.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Please enter a port number to use for the setup.\n");
		return 1;
	}

    if(strcmp("-c", argv[1]) == 0 || strcmp("--client", argv[1]) == 0) {
        int sock = atoi(argv[3]);
        char * addr = argv[2];
        printf("CLIENT Socket: %i\n", sock);
        client(sock, addr);
    }
    else if(strcmp("-s", argv[1]) == 0 || strcmp("--server", argv[1]) == 0) {
        int sock = setup_server(argv[2]);
        printf("SERVER Socket: %i\n", sock);
        server(sock);
    }
    else {
        printf( "This is the help function!\n");
    }
	return 0;
}

/**
 * @brief Setup clientside functionality
 * 
 * This side is given an address and port and 
 * connects to a server. This will then listen 
 * to input from a server and will be able to send 
 * messages to the server.
 * 
 * @param port port to connect tp
 * @param addr address to connec to
 */
void client(int port, char * addr) {
    //int flag = 1;
    printf("PORT: %i ADDR: %s\n", port, addr);
    close(port);
}


/**
 * @brief Setup server functionality
 * 
 * This function deals with server specific setup and 
 * cleaning. It creates 2 threads which are used to getting 
 * messages from STDIN and then sending them to a connected 
 * client. The other thread will listen to the port for messages 
 * from the connected client.
 * 
 * @param port port which has been setup to listen/send
 */
void server(int port) {
    int flag = 1;
	struct thread* create = malloc(sizeof(struct thread));
	create->flag = &flag;
	create->socket = port;

	struct thread* readM = malloc(sizeof(struct thread));
	readM->flag = &flag;
	readM->socket = port;

	if(pthread_create(&(create->id), NULL, createMessage, (void*) create) != 0){
		printf("Error didn't create thread\n");
	}
	if(pthread_create(&(readM->id), NULL, readMessage, (void*) readM) != 0){
		printf("Error didn't create thread\n");
	}

	// Clean up
	pthread_join(create->id, NULL);
	pthread_join(readM->id, NULL);
	free(create);
	free(readM);
	close(port);
}
