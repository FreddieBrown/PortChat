
#include "threads.h"
#include "queue.h"
#include "tools.h"

int main(int argc, char * argv[]) {
    int socket = setup(argv[1]);
    int valread;
    char buffer[1024] = {0};


    // Create threads to listen to STDIN and traffic over port
    // Need to create port info struct 
    // Need to create ids
    struct queue * que = malloc(sizeof(struct queue));
    init_q(que);
    struct thread * create = malloc(sizeof(struct thread));
    create->flag = 0;
    create->que = que;
    struct thread * sendM = malloc(sizeof(struct thread));
    sendM->flag = 0;
    sendM->que = que;
    sendM->socket = socket;
    if(pthread_create(&(create->id), NULL, createMessage, (void *) create) != 0){
        printf("Error didn't create thread\n");
    }
    if(pthread_create(&(sendM->id), NULL, sendMessage, (void *) sendM) != 0){
        printf("Error didn't create thread\n");
    }
    int flag = 1;
    do{
        valread = read(socket, buffer, 1024);
        char * exit = "exit\n";
        printf("From Client: %s\n", buffer);
        char * message = "\nAck\n\n";
        send(socket, message, strlen(message), 0);
        char data;
        flag = strcmp(buffer, exit);
        if(!flag){
            close(socket);
        }
        recv(socket,&data,1, MSG_PEEK);
        memset(buffer, 0, sizeof(buffer));
    }while(flag);

    // Clean up
    close(socket);
    // Set a flag telling it to close
    printf("Closing down Main function\n");
    create->flag = 1;
    sendM->flag = 1;
    pthread_join(create->id, NULL);
    pthread_join(sendM->id, NULL);
    free_q(que);
    free(create);
    free(sendM);

    return 0;
}
