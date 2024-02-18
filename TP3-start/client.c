#include "config.h"
#include "network.h"
#include "game.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <SDL.h>
#include <SDL_ttf.h>

int sock = -1;
struct addrinfo* server_info = NULL;
GameData g;

// you should use this mutex somewhere..
// pthread_mutex_t game_data_mutex = PTHREAD_MUTEX_INITIALIZER;

int redraw = 1;

uint8_t get_player_id(int sock, struct addrinfo* server_info)
{
	// get an id from the server

	// send 1 byte [REQUEST_ID] & wait
	// server will respond with 2 bytes [REQUEST_ID, id]

	return 0;
}

void* client_listen_loop(void* arg)
{
	// listening thread function
	
	// server will send [GAME_DATA, struct GameData] regularly
	// copy the received data in the local GameData struct g
	// then put redraw to 1

	return NULL;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s serverIP serverPORT\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// get server info
	// (fill struct addrinfo* server_info from given IP & port)

	// create socket

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == -1)
	{
        perror("socket");
		return EXIT_FAILURE;
	}

	int b = bind_socket(sock, atoi(argv[1]));
	if (b == -1)
	{
		perror("bind");
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	// ----------------------------------------------------



	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Could not initialize SDL: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* window;
	window = SDL_CreateWindow("Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if(window == NULL)
	{
		fprintf(stderr, "Error while creating SDL window: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer* renderer;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)
	{
		fprintf(stderr, "Error while creating SDL renderer: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Could not initialize SDL_ttf: %s.\n", TTF_GetError());
		return EXIT_FAILURE;
	}

	TTF_Font* font = TTF_OpenFont("Roboto-Light.ttf", 24);
	SDL_Color White = {255, 255, 255};
	SDL_Surface* text_surface;
	SDL_Texture* text_texture;

	init_gamedata(&g);

	uint8_t player_id = get_player_id(sock, server_info);
	if (player_id == INVALID_ID)
	{
		fprintf(stderr, "Server has no place left for a new player!\n");
		return EXIT_FAILURE;
	}
	else
		printf("received player id: %d\n", player_id);

	// launch client_listen_loop thread 

	SDL_Event e;
	int running = 1;
	char score[32];
	
	while (running)
	{
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							// send 3 bytes [PLAYER_MOVE, player_id, GO_LEFT]
							break;
						case SDLK_RIGHT:
							// send 3 bytes [PLAYER_MOVE, player_id, GO_RIGHT]
							break;
						case SDLK_UP:
							// send 3 bytes [PLAYER_MOVE, player_id, GO_ACC]
							break;
						case SDLK_DOWN:
							// send 3 bytes [PLAYER_MOVE, player_id, GO_DECC]
							break;
					}
					break;
				default:
					break;
			}
		}
		
		if (redraw)
		{
			SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
			SDL_RenderClear(renderer);

			for (int id = 0; id < MAX_PLAYERS; ++id)
			{
				if (g.players[id].valid)
				{
					if (id == player_id)
						SDL_SetRenderDrawColor(renderer, 20, 200, 20, 255);
					else
						SDL_SetRenderDrawColor(renderer, 200, 20, 20, 255);
					draw_circle(renderer, g.players[id].position.x, g.players[id].position.y, g.players[id].size);
				}
			}

			sprintf(score, "Score: %d", g.players[player_id].score);
			text_surface = TTF_RenderText_Solid(font, score, White);
			text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
			SDL_Rect text_rect = {0, 0, text_surface->w, text_surface->h};
			SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
			SDL_FreeSurface(text_surface);
			SDL_DestroyTexture(text_texture);

			SDL_RenderPresent(renderer);

			redraw = 0;
		}

		usleep(100);
	}

	// send 2 bytes [RELEASE_ID, player_id]

	// stop client_listen_loop thread

	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_CloseFont(font);
	TTF_Quit();

	// free server_info struct
	// close socket

	return EXIT_SUCCESS;
}
