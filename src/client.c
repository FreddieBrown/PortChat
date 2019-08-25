#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>


/**
 * @brief Sets up client sock connection
 * 
 * This function will take an address and a socket 
 * and will use it to connect to a device and port 
 * which is specified using port and addr.
 * 
 * @param port Port number to connect to 
 * @param addr IP address to connect to
 * @return int socket value taken from connection details
 */
int setup_client(char* port, char* addr) {
    printf("%s, %s", port, addr);

    int server_fd = 0;
    struct sockaddr_in serv_addr;
    


	printf("This is the port: %s\n", port);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failure");
		exit(EXIT_FAILURE);
	}

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(atoi(port)); 

    if(inet_pton(AF_INET, addr, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    return server_fd;
}