#ifndef GAME_H_
#define GAME_H_

#include "config.h"
#include <stdint.h>

struct point2D
{
	float x;
	float y;
};



typedef
struct Player
{
	uint8_t valid;
	struct point2D position;
	uint16_t size;
	uint16_t direction;
	uint8_t speed;
	uint8_t score;
}
Player;

void init_player(Player* p);
void release_player(Player* p);
void update_player_direction(Player* p, uint8_t action);
void move_player(Player* p);
float distance(Player* p1, Player* p2);



typedef
struct GameData
{
	Player players[MAX_PLAYERS];
}
GameData;

void init_gamedata(GameData* g);
uint8_t next_available_id(GameData* g);
void move_players(GameData* g);

#endif // GAME_H_
