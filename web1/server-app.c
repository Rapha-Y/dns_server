#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

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

int main(int argc, char **argv) {
    //./server-app <porta>
   
    if(argc != 2){
	printf("Uso: %s <porta>", argv[0]);
	return 0;
      }
 
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    //cria e verifica o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //cria endpoint para comunicacao
    if (sockfd == -1) {
        perror("socket");
        return 0;
    } else
        printf("Socket criado com sucesso\n");
    bzero((&servaddr), sizeof(servaddr));

    //atribuindo IP e PORTA
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    //bind socket para o IP
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind falhou\n");
        exit(0);
    } else
        printf("Socket bind fncionou\n");

    //listen 
    if ((listen(sockfd, 5)) != 0) { //fila com 5 clientes
        printf("Listen erro\n");
        exit(0);
    } else 
        printf("Servidor escutando...\n");
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
