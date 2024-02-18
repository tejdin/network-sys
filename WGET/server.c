#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
    #include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/types.h>
int sockc;
int socks;

void *thread(void *vargp)
{
    int clientfd = *((int *)vargp);
    char buf[256];
    recv(clientfd, buf, 234, 0);
    strtok(buf, "\n");
    char *token = strtok(buf, " ");
    token = strtok(NULL, " ");
    printf("%s\n", token);
    dprintf(clientfd, "HTTP/1.1 200 OK\r\n");   
    dprintf(clientfd, "Content-Type: text/html; charset=UTF-8\r\n\r\n");
    dprintf(clientfd, "\n\n");



    char *path = malloc(strlen(token) + 2);
    strcpy(path, token);
    printf("%s\n", path);
    sprintf(path, ".%s", token);
    printf("%s\n", path);

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return NULL;
    }
    while (read(fd, buf, 25) > 0)
    {
        send(clientfd, buf, 25, 0);
    }
    close(fd);
    

}

void * handler(int signum){
    fclose(socks);
}


int main(int argc,char* argv[])
{

    struct sigaction sa;
    sigaction(SIGINT,);
    printf("server\n");
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);

    struct addrinfo *res;
    struct addrinfo hints;
    int sockfd;
    int clientfd;

    memset(&hints,0,sizeof hints);

    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;


    if(getaddrinfo(argv[1],argv[2],&hints,&res)!=0)
        perror("getaddrinfo");


    sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(sockfd==-1)
        perror("socket");


    socks=sockfd;

    if(bind(sockfd,res->ai_addr,res->ai_addrlen)==-1)
        perror("bind");

    if(listen(sockfd,10)==-1)
        perror("listen");
    while (1)
    {
        struct sockaddr_in client ;
        socklen_t clientlen = sizeof client;
        clientfd = accept(sockfd,(struct sockaddr *)&client,&clientlen);
        if(clientfd==-1)
            perror("accept");


        pthread_t tid;
        pthread_create(&tid, NULL, thread, &clientfd);
        pthread_join(tid, NULL);
        close(clientfd);
    }
    return 0;
}