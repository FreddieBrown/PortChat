#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "threads.h"

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
    thread * info = (thread*) arg;
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
    thread * info = (thread*) arg;
    int valread;
    char buffer[1024] = {0};
    // Read from command line, create message
    // and pass it to message queue
    do {
        valread = read(info->socket, buffer, 1024);
        char * exit = "exit\n";
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
