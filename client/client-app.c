#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 100
#define SA struct sockaddr

void chat(int sockfd) {
    char buff[MAX];
    int n;

    // loop infinito
    for (;;) {
        bzero(buff, sizeof(buff)); // seta 0's no buffer
        printf("Mensagem: ");
        n = 0;
	
	//copia a mensagem do buffer
        while ((buff[n++] = getchar()) != '\n');
	
	//envia o buffer para o servidor
        write(sockfd, buff, sizeof(buff));

        bzero(buff, sizeof(buff));
	
	//le a mensagem do servidor e cpoia para o buffer
        read(sockfd, buff, sizeof(buff));
        printf("De servidor: %s", buff);

	//exit
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client exit\n");
            break;
        }
    }
}

int main(int argc, char **argv) {
    //.client-app <porta>
   if(argc != 3){
	printf("Uso: %s %s <porta>\n",argv[0], argv[1]);
	return 0;
   }
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    //cria e verifica o  socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket\n");
        exit(0);
    } else
        printf("criaçao do socket concluida com sucesso\n");
    bzero(&servaddr, sizeof(servaddr));

    struct hostent *he;
    if ((he = gethostbyname(argv[1])) == NULL)
        exit(1);

    memcpy(&servaddr.sin_addr, he->h_addr_list[0], he->h_length);

    //atribui IP e PORTA
    servaddr.sin_family = AF_INET;
    //atribuicao do sin_addr feita acima "gethostbyname"
    servaddr.sin_port = htons(atoi(argv[2]));

    //conexao entre o socket do servidor e do cliente
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("connect\n");
        exit(0);
    } else
        printf("Conexao com o servidor realizada com sucesso\n");

    //chat
    chat(sockfd);

    //encerra socket
    close(sockfd);
}
