#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void* sendMessage(void * arg);
void* receiveMessage(void * arg);
void* createMessage(void* arg);