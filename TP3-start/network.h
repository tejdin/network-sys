#ifndef NETWORK_H_
#define NETWORK_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int bind_socket(int sock, int port);
struct addrinfo* get_host_info(const char* name, const char* service);
void print_host_info(const struct addrinfo* host_info);

#endif // NETWORK_H_
