#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "threads.h"
#include "tools.h"

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

	int sock = setup(argv[1]);
	printf("Socket: %i\n", sock);
	int flag = 1;

	// Create threads to listen to STDIN and traffic over port
	// Need to create port info struct
	// Need to create ids
	struct thread* create = malloc(sizeof(struct thread));
	create->flag = &flag;
	create->socket = sock;

	struct thread* readM = malloc(sizeof(struct thread));
	readM->flag = &flag;
	readM->socket = sock;

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
	close(sock);

	return 0;
}
