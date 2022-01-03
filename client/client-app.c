#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the msg: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From server: %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client exit\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    //create + verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        exit(0);
    } else
        printf("Socket successfuly created\n");
    bzero(&servaddr, sizeof(servaddr));

    struct hostent *he;
    if ((he = gethostbyname("www.unifesp.br")) == NULL)
        exit(1);

    memcpy(&servaddr.sin_addr, he->h_addr_list[0], he->h_length);

    //assign IP, PORT
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    //conn client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Conn with the server failed\n");
        exit(0);
    } else
        printf("Conn to the server\n");

    //chat func
    chat(sockfd);

    //close socket
    close(sockfd);
}
