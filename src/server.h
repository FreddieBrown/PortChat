#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "tools.h"

typedef struct thread{
	pthread_t id;
	int socket;
	int* flag;
} thread;

void* sendMessage(void* arg);
void* createMessage(void* arg);
void* readMessage(void* arg);

#endif
