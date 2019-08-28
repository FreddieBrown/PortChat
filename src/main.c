#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "server.h"
#include "tools.h"
#include "client.h"

#define MAX_BUFFER_LEN 256

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
    char hostname[MAX_BUFFER_LEN] = {0};
    get_primary_ip(hostname, sizeof(hostname));
	printf("%s\n", hostname);

    int sock = 0;
    FILE* logging = NULL;
    char* port;
    char* addr;

    int set = 0;

	if (argc == 1) {
		help();
		return 0;
	}

    if (signal(SIGINT, sig_handler) == SIG_ERR){
        printf("\n Failed to catch signal \n");
    }

    for (int i = 1; i < argc; i++) {
        if ((strcmp("-c", argv[i]) == 0 || strcmp("--client", argv[i]) == 0) && !set) {
            i += 2;
            set = 1;
            port = argv[i];
            addr = argv[i-1];
      
        }
        else if ((strcmp("-s", argv[i]) == 0 || strcmp("--server", argv[i]) == 0) && !set){
            set = 2;
            port = argv[++i];
        }
        else if (strcmp("-l", argv[i]) == 0 || strcmp("--log", argv[i]) == 0) {
            printf("Setting up logging\n");
            logging = fopen("./messages.txt", "a");
            // Write the date and time to the file
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf(logging, "%s\n", asctime(timeinfo));
        }
        else {
            help();
        }
    }

    switch(set){
        case 1: sock = setup_client(port, addr);
                printf("CLIENT Socket: %i\n", sock);
                break;
        case 2: sock = setup_server(port);
                printf("SERVER Socket: %i\n", sock);
                break;
    }

    if (set) {
        start(sock, hostname, logging);
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
 * @param hostname name of the users machine
 * @param fptr Logging file which output is written to
 */
void start(int port, char* hostname, FILE* fptr) {
    int flag = 1;
	struct thread* create = malloc(sizeof(struct thread));
	create->flag = &flag;
	create->socket = port;
    create->host = hostname;
    create->log = fptr;

	struct thread* readM = malloc(sizeof(struct thread));
	readM->flag = &flag;
	readM->socket = port;
    readM->host = hostname;
    readM->log = fptr;

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

void help() {
    printf("\nPortChat\n=========================\n");
    printf("\nThis is the help function!\n");
    printf("To open a socket to receive input, type: -s <port>\n");
    printf("To connect to an open server, type: -c <address> <port>\n");
    printf("To open logging, add -l\n\n");
}
