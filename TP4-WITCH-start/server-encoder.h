#ifndef __ENCODER_H__
#define __ENCODER_H__

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

uint8_t test_code(const char* proposition, uint8_t* good_place, uint8_t* good_number);
void str_remove(char * string, char to_remove);

#endif