#ifndef TOOLS_H
#define TOOLS_H

#include <pthread.h>

typedef struct thread{
	pthread_t id;
	int socket;
	int* flag;
    char* host;
} thread;

void get_primary_ip(char*, size_t);

void* sendMessage(void*);
void* createMessage(void*);
void* readMessage(void*);

#endif
