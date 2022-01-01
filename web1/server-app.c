#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int connfd) {
    char buff[MAX];
    int n;

    //infinite loop
    for (;;) {
        bzero(buff, MAX);
        
        //read msg from client + copy to buff
        read(connfd, buff, sizeof(buff));
        //print buff
        printf("From client: %s\t To client: ", buff);
        bzero(buff, MAX);
        n = 0;
        //copy msg from buff
        while ((buff[n++] = getchar()) != '\n');
        
        //send buff to client
        write(connfd, buff, sizeof(buff));

        //exit server and chat
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server exit\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd, len;
    struct sockaddrs_in servaddr, cli;

    //create + verify socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        exit(0);
    } else
        printf("Socket successfully created\n");
    bzero((&servaddr), sizeof(servaddr));

    //assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    //bind socket to IP + verify
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed\n");
        exit(0);
    } else
        printf("Socket successfully binded\n");

    //listen + verify
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed\n");
        exit(0);
    } else 
        printf("Server listening\n");
    len = sizeof(cli);

    //accept packet from client + verify
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Server accept failed\n");
        exit(0);
    } else
        printf("Server accept the client\n");

    //chat between client and server
    chat(connfd);

    //close socket
    close(sockfd);
}