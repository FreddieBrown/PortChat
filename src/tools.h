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

void* create_message(void*);
void* read_message(void*);

#endif
