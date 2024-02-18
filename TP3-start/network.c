#include "network.h"

#include <stdio.h>
#include <string.h>

int bind_socket(int sock, int port)
{
	
	    if(bind(sock,))){
            perror("hi");
        return EXIT_FAILURE;
    }
	

	return 1;
}

struct addrinfo* get_host_info(const char* name, const char* service)
{
	 struct addrinfo hints , *res;
    // création d'un socket pour communiquer avec l'encoder (serveur)
    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    if(getaddrinfo(name,service,&hints,&res) ==-1){
		perror("getinfo");
		return NULL;
	}

	return res;
}

void print_host_info(const struct addrinfo* host_info)
{

}
