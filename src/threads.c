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
            printf("Length: %i\n", que->len);
            struct data * dat = pull(que);
            send(info->socket, dat->message, strlen(dat->message), 0);
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
        printf("Checking info flag\n");
        if(info->flag){
            printf("Closing down\n");
            return arg;
        }
        memset(buffer, 0, sizeof(buffer));
        scanf("%s[\n]", buffer);
        struct data * dat = malloc(sizeof(struct data));
        dat->message = buffer;
        push(dat, info->que);
        // Add to message queue
        printf("Output: %s\n", buffer);

    }
    return arg;
}