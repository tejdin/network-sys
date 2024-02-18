#ifndef __DECODER_H__
#define __DECODER_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "config.h"


int get_proposition(char* proposition);
void print_result(uint8_t good_place, uint8_t good_number);
void print_proposition(char* proposition);

#endif
