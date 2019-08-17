#include "threads.h"


void* sendMessage(void * arg) {
    struct thread * info = (thread*) arg;
    struct queue * que = info->que;
    //Take port and monitor message queue
    int flag = 1;
    do{
        // Once message is found, send via port
        if(info->flag){
            return arg;
        }

        if(que->len > 0) {
            struct data * dat = pull(que);
            printf("Message is: %s\n", dat->message);
            printf("Socket: %i\n", info->socket);
            if (send(info->socket, dat->message, strlen(dat->message), 0) < 0) {
                puts("Send failed");
            }
            free(dat);
        }
    }while(flag);
    return arg;
}

void* createMessage(void* arg){
    thread * info = (thread*) arg;
    // Read from command line, create message 
    // and pass it to message queue
    int flag = 1;
    char buffer[1024] = {0};
    while(flag) {
        if(info->flag){
            return arg;
        }
        memset(buffer, 0, sizeof(buffer));
        scanf("%s[\n]", buffer);
        struct data * dat = malloc(sizeof(struct data));
        dat->message = buffer;
        push(dat, info->que);

    }
    return arg;
}