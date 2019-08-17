#include "threads.h"


// void* sendMessage(void * arg) {
//     //Take port and monitor message queue
//     int flag = 1;
//     int valread;
//     do{
//         // Once message is found, send via port
//         send(new_socket, message, strlen(message), 0);
//         char data;
//         recv(new_socket,&data,1, MSG_PEEK);
//     }while(flag);
// }

// void* receiveMessage(void * arg) {
//     // Get port and setup message queue
//     int flag = 1;
//     int valread;
//     do{
//         // Read messages sent via port
//         valread = read(new_socket, buffer, 1024);
//         char * exit = "exit\n";
//         // Print message
//         printf("%s\n", buffer);
//         char * message = "\nAck\n\n";
//         send(new_socket, message, strlen(message), 0);
//         // test port connection
//         char data;
//         flag = strcmp(buffer, exit);
//         if(!flag){
//             close(new_socket);
//         }
//         recv(new_socket,&data,1, MSG_PEEK);
//         memset(buffer, 0, sizeof(buffer));
//     }while(flag);
// }

void* createMessage(void* arg){
    // Read from command line, create message 
    // and pass it to message queue
    int flag = 1;
    char buffer[1024] = {0};
    while(flag) {
        memset(buffer, 0, sizeof(buffer));
        scanf("%s[\n]", buffer);
        // Add to message queue
        printf("Output: %s\n", buffer);

    }
    return arg;
}