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
    struct sockaddr_in client;
    char ipstr[INET_ADDRSTRLEN];
    memset(&client,0,sizeof client);
    time_t client_start;
    
    int sockfd;

    char buf[256];

    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(argv[1], argv[2], &hints, &res);

    // make a socket:
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    inet_ntop(AF_INET,res->ai_addr, ipstr, 256);
    printf("%s\n",ipstr);

    // bind it to the port we passed in to getaddrinfo():
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    socklen_t clientlen = sizeof client;
    while (1)
    {
        /* code */
        recvfrom(sockfd, &client_start, sizeof(client_start), 0,(struct sockaddr *)&client, &clientlen);
        //print hour:minute:second
        printf("%s\n",ctime(&client_start));


       // sendto(sockfd, "salut", 5, 0, (struct sockaddr *)&client, clientlen);

    }    

close(sockfd);
    return 0;

    


}