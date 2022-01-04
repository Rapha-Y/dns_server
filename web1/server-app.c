#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX 100
#define SA struct sockaddr

void chat(int connfd) {
    char buff[MAX];
    int n;

    //loop infinito
    for (;;) {
        bzero(buff, MAX); // seta como 0 todos os bites do vetor buff

        // le a mensagem  do cliente e copia para o buffer
        read(connfd, buff, sizeof(buff));

        //printa o buffer
        printf("De cliente: %s\t Para cliente: ", buff);
        bzero(buff, MAX);
        n = 0;

        //copia a mensagem do buffer
        while ((buff[n++] = getchar()) != '\n');

        //envia o buffer para o client
        write(connfd, buff, sizeof(buff));

        //encerra o server e o chat
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
    bzero((&servaddr), sizeof(servaddr)); // seta 0's em servaddr

    //atribuindo IP e PORTA
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    //bind socket para o IP
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind falhou\n");
        exit(0);
    } else
        printf("Socket bind funcionou\n");

    //listen 
    if ((listen(sockfd, 5)) != 0) { //fila com 5 clientes
        perror("Listen\n");
        exit(0);
    } else 
        printf("Servidor escutando...\n");
    len = sizeof(cli);

    //accept
    // aceita pacotes do cliente
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        perror("accept\n");
        exit(0);
    } else
        printf("Servidor aceitou o client\n");

    //executa o chat entre o cliente e servidor
    chat(connfd);

    //close
    close(sockfd); //fecha o  socket
}
