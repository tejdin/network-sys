#include "game.h"
#include "config.h"

#include <stdlib.h>
#include <math.h>

void init_player(Player* p)
{
	p->valid = 1;
	p->position.x = (float)(rand() % (SCREEN_WIDTH - 10));
	p->position.y = (float)(rand() % (SCREEN_HEIGHT - 10));
	p->size = 5;
	p->direction = rand() % 360;
	p->speed = 0;
	p->score = 0;
}

void release_player(Player* p)
{
	p->valid = 0;
}

void update_player_direction(Player* p, uint8_t action)
{
	switch (action)
	{
		case GO_LEFT:
			p->direction = (p->direction - 10 + 360) % 360;
			break;
		case GO_RIGHT:
			p->direction = (p->direction + 10) % 360;
			break;
		case GO_ACC:
			p->speed += 1;
			break;
		case GO_DECC:
			if (p->speed > 0) p->speed -= 1;
			break;
	}
}

void move_player(Player* p)
{
	float r = p->direction * M_PI / 180.0;
	float dirx = p->speed * cos(r);
	float diry = p->speed * sin(r);
	p->position.x = fmod(p->position.x + dirx + SCREEN_WIDTH, SCREEN_WIDTH);
	p->position.y = fmod(p->position.y + diry + SCREEN_HEIGHT, SCREEN_HEIGHT);
}

float distance(Player* p1, Player* p2)
{
	float xx = (p1->position.x - p2->position.x) * (p1->position.x - p2->position.x);
	float yy = (p1->position.y - p2->position.y) * (p1->position.y - p2->position.y);
	return sqrt(xx + yy);
}

void init_gamedata(GameData* g)
{
	for (uint8_t id = 0; id < MAX_PLAYERS; ++id)
		g->players[id].valid = 0;
}

uint8_t next_available_id(GameData* g)
{
	for (int id = 0; id < MAX_PLAYERS; ++id)
	{
		if (!g->players[id].valid)
			return id;
	}
	return INVALID_ID;
}

void move_players(GameData* g)
{
	for (uint8_t id = 0; id < MAX_PLAYERS; ++id)
	{
		if (g->players[id].valid)
			move_player(g->players+id);
	}
	
	for (uint8_t id = 0; id < MAX_PLAYERS; ++id)
	{
		if (!g->players[id].valid)
			continue;
		for (uint8_t other_id = id+1; other_id < MAX_PLAYERS; ++other_id)
		{
			if (!g->players[other_id].valid)
				continue;
			if (distance(g->players+id, g->players+other_id) < g->players[id].size + g->players[other_id].size)
			{
				int total = g->players[id].size + g->players[other_id].size;
				int combat = rand() % total;
				uint8_t winner_id, loser_id;
				if (combat <= g->players[id].size)
				{
					winner_id = id;
					loser_id = other_id;
				}
				else
				{
					winner_id = other_id;
					loser_id = id;
				}
				
				g->players[winner_id].size += g->players[loser_id].size * 0.75;
				// if (g->players[winner_id].speed > 1)
				// 	g->players[winner_id].speed--;
				g->players[winner_id].score++;
				init_player(g->players+loser_id);
			}
		}	
	}
}
