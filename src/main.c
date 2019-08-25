#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include "server.h"
#include "tools.h"
#include "client.h"

#define MAX_BUFFER_LEN 256

void start(int, char*);
void sig_handler(int);
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
    int sock = 0;
    char hostname[MAX_BUFFER_LEN] = {0};
    get_primary_ip(hostname, sizeof(hostname));
	printf("%s\n", hostname);

	if (argc < 2) {
		fprintf(stderr, "Please enter a port number to use for the setup.\n");
		return 1;
	}

    if (signal(SIGINT, sig_handler) == SIG_ERR){
        printf("\n Failed to catch signal \n");
    }
    int i;
    int set = 0;
    for (i = 1; i<argc; i++) {

        if ((strcmp("-c", argv[i]) == 0 || strcmp("--client", argv[i]) == 0) && !set) {
            i += 2;
            sock = setup_client(argv[i], argv[i-1]);
            printf("CLIENT Socket: %i\n", sock);
            set = 1;
        
        }
        else if ((strcmp("-s", argv[i]) == 0 || strcmp("--server", argv[i]) == 0) && !set){
            sock = setup_server(argv[++i]);
            printf("SERVER Socket: %i\n", sock);
            set = 1;
        }
        else {
            printf("\nPortChat\n=========================\n");
            printf("\nThis is the help function!\n");
            printf("To open a socket to receive input, type: -s <port>\n");
            printf("To connect to an open server, type: -c <address> <port>\n\n");
        }
    }
    if (set) {
        start(sock, hostname);
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
void start(int port, char* hostname) {
    int flag = 1;
	struct thread* create = malloc(sizeof(struct thread));
	create->flag = &flag;
	create->socket = port;
    create->host = hostname;

	struct thread* readM = malloc(sizeof(struct thread));
	readM->flag = &flag;
	readM->socket = port;
    readM->host = hostname;

	if (pthread_create(&(create->id), NULL, create_message, (void*) create) != 0){
		printf("Failed to initialise or create the create_message thread.\n");
	}

	if (pthread_create(&(readM->id), NULL, read_message, (void*) readM) != 0){
		printf("Failed to initialise or create the read_message thread.\n");
	}

	// Clean up
	pthread_join(create->id, NULL);
	pthread_join(readM->id, NULL);
	free(create);
	free(readM);
	close(port);
}

/**
 * @brief Signal Handler
 *
 * This is a signal handler for the program. This is invoked when
 * ^C is used to kill the program. It allows for a safe exit point
 * in the program.
 *
 * @param signo Signal code
 */
void sig_handler(int signo) {
    printf("\nSIGNAL NUMBER: %i\n", signo);
    exit(EXIT_SUCCESS);
}
