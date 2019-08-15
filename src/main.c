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
#include "threads.c"

void GetPrimaryIp(char *, size_t);

int main(int argc, char * argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    char * port = argv[1];
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char hostbuffer[256];
    GetPrimaryIp(hostbuffer, sizeof(hostbuffer));
    printf("%s\n", hostbuffer);
    printf("This is the port: %s\n", port);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failure");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(port));

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {

        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0) {

        perror("listen");
        exit(EXIT_FAILURE);
    }

    if((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    do{
        valread = read(new_socket, buffer, 1024);
        char * exit = "exit\n";
        printf("%s\n", buffer);
        char * message = "\nAck\n\n";
        send(new_socket, message, strlen(message), 0);
        char data;
        flag = strcmp(buffer, exit);
        if(!flag){
            close(new_socket);
        }
        recv(new_socket,&data,1, MSG_PEEK);
        memset(buffer, 0, sizeof(buffer));
    }while(flag);

    close(new_socket);

    return 0;
}

void GetPrimaryIp(char* buffer, size_t buflen) 
{

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    const char* kGoogleDnsIp = "8.8.8.8";
    uint16_t kDnsPort = 53;
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(kDnsPort);

    int err = connect(sock, (const struct sockaddr*) &serv, sizeof(serv));

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);

    inet_ntop(AF_INET, &name.sin_addr, buffer, buflen);

    close(sock);
}
