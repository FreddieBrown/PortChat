#ifndef MAIN_H
#define MAIN_H

enum Mode {
	UNKNOWN,
	CLIENT,
	SERVER
};

void start(int, char*, FILE*);
void sig_handler(int);
void help();

#endif /* ifndef MAIN_H */
