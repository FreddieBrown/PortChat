#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct thread{
    pthread_t id;
    struct queue * que;
    int socket;

}thread;

typedef struct queue{
    struct data * front;
    struct data * rear;
    pthread_mutex_t rwmutex;
    pthread_cond_t filled;
    int len;
}queue;

void* sendMessage(void * arg);
void* receiveMessage(void * arg);
void* createMessage(void* arg);

#endif