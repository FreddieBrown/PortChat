#ifndef TOOLS_H
#define TOOLS_H

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <errno.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

void get_primary_ip(char *, size_t);
int setup(char * port);

#endif
