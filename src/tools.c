#include "tools.h"

/**
 * @brief Get the local IP information of the computer
 * 
 * This will make a UDP connection which will give back 
 * information on the local IP of the machine. This is then 
 * written to the buffer and the method finishes. The UDP 
 * socket which was opened, is closed.
 * 
 * @param buffer buffer which data is read into 
 * @param buflen length of buffer which data is read into
 */
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

/**
 * @brief Sets up the TCP port connection
 * 
 * Starts a conneciton with a client via a socket. 
 * This is all done in this method before returning 
 * the socket details.
 * 
 * @param port port which user wishes to connect to
 * @return int socket details
 */
int setup(char * port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
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
    return new_socket;
}