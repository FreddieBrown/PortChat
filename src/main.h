#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UNKNOWN 0
#define CLIENT 1
#define SERVER 2

void start(int, char*, FILE*);
void sig_handler(int);
void help();

#endif /* ifndef MAIN_H */
