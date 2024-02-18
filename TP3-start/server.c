#include "config.h"
#include "network.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

int sock = -1;
struct sockaddr_in clients_info[MAX_PLAYERS];
GameData g;

// you should use this mutex somewhere..
// pthread_mutex_t game_data_mutex = PTHREAD_MUTEX_INITIALIZER;

void* server_listen_loop(void* arg)
{
	// wait for client messages
	// messages can be:
	// - [REQUEST_ID]: next_available_id, store client sockaddr_in, init_player, send 2 bytes [REQUEST_ID, id]
	// - [RELEASE_ID, id]: release_player
	// - [PLAYER_MOVE, id, MOVE_TYPE]: update_player_direction

	return NULL;
}

void* server_game_loop(void* arg)
{
	// at regular interval:
	// - move_players
	// - send [GAME_DATA, struct GameData g] to all valid players

	// to make regular intervals:
	// - gettimeofday at the beginning of the loop
	// - gettimeofday at the end of the loop
	// - timersub
	// - usleep the number of milliseconds needed to reach FRAME_DURATION

	return NULL;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		return EXIT_FAILURE;
	}
	 
	// create socket


	// bind socket to given port
	

	srand(time(NULL));

	init_gamedata(&g);

	// launch server_listen_loop thread
	// launch server_game_loop thread

	// wait for threads end

	// close socket

	return EXIT_SUCCESS;
}
