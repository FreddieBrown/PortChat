#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "server.h"
#include "tools.h"
#include "client.h"

void start(int port);
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
        int sock = setup_client(argv[3], argv[2]); 
        printf("CLIENT Socket: %i\n", sock);
        start(sock);
    }
    else if(strcmp("-s", argv[1]) == 0 || strcmp("--server", argv[1]) == 0) {
        int sock = setup_server(argv[2]);
        printf("SERVER Socket: %i\n", sock);
        start(sock);
    }
    else {
        printf( "This is the help function!\n");
    }
	return 0;
}


/**
 * @brief Setup functionality
 * 
 * This function deals with setup and 
 * cleaning. It creates 2 threads which are used to getting 
 * messages from STDIN and then sending them to a connected 
 * device. The other thread will listen to the port for messages 
 * from the connected device.
 * 
 * @param port port which has been setup to listen/send
 */
void start(int port) {
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
