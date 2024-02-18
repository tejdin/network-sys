#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 #include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;
    int sockfd;
    time_t client_start;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM ;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if(getaddrinfo(argv[1], argv[2], &hints, &res) != 0)
        perror("getaddrinfo");
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd == -1)
        perror("socket");

        bind(sockfd,res->ai_addr, res->ai_addrlen);
    time_t start = time(NULL);




    if(sendto(sockfd,&start, 5, 0, res->ai_addr, res->ai_addrlen) == -1)
        perror("sendto");

    char buf[256];

signal(SIGALRM, NULL);
        alarm(5);
        if(recvfrom(sockfd,buf,256, 0, res->ai_addr, &res->ai_addrlen) == -1)
        perror("recvfrom");

  

    printf("%s\n",buf);



    return 0;
}
