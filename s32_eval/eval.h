#ifndef __EVAL_H__
#define __EVAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT_QOTD    "17"
#define QOTD_DAYTIME "13"

void print_ip_address(char* hostnamee);
void print_my_ip();
int connect_tcp(char *server_name, char* port);
int client_euclide_udp(char *server_name, char *port, int a, int b);
int serveur_euclide_udp(char *port);

#endif
