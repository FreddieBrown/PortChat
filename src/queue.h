#ifndef QUEUE_H
#define QUEUE_H

#include "tools.h"

typedef struct data{
    char * message;
    struct data * next;
}data;

typedef struct queue{
    struct data * front;
    struct data * rear;
    pthread_mutex_t rwmutex;
    pthread_cond_t filled;
    int len;
}queue;

void init_q(struct queue * que);
int push(struct data * data, struct queue * que);
struct data * pull(struct queue * que);
void free_q(struct queue * que);

#endif