#ifndef THREADS_H
#define THREADS_H

#include "tools.h"
#include "queue.h"

typedef struct thread{
    pthread_t id;
    struct queue * que;
    int socket;
    int flag;

}thread;

void* sendMessage(void * arg);
void* createMessage(void* arg);

#endif