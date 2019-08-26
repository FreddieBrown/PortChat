#ifndef TOOLS_H
#define TOOLS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct thread{
	pthread_t id;
	int socket;
	int* flag;
    char* host;
    FILE* log;
} thread;

void get_primary_ip(char*, size_t);

void* create_message(void*);
void* read_message(void*);

void logger(char*, FILE*, int);

#endif
