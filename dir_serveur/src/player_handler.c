#include "../includes/serveur.h"

/*
** This file contains the functions to handle clients' players.
*/

/*
** Randomly assign player position on the server's board.
*/

void		assign_random_player_position(t_serveur *serv, t_player *player)
{
	srand(time(NULL));
	player->pos.x = rand() % serv->world_hdl.map_x;
	player->pos.y = rand() % serv->world_hdl.map_y;
	player->dir = rand() % 3;

	player->cur_case = &(serv->world_hdl
						.world_board[player->pos.y][player->pos.x]);
}


/*
**	Assign the time at which a player will consume one food in its inventory(dinner time).
*/

void		assign_player_time_of_dinner(t_serveur *serv, t_player *player)
{
	get_time(&player->next_dinner_time);

	player->next_dinner_time.tv_nsec += FOOD_LIFE_TIME * serv->world_hdl.t_unit * 1000000000;
	// check it we got through seconds.
	while (player->next_dinner_time.tv_nsec > 1000000000)
	{
		player->next_dinner_time.tv_nsec -= 1000000000;
		player->next_dinner_time.tv_sec += 1;
	}
	//player->next_dinner_time.tv_sec += FOOD_LIFE_TIME * serv->world_hdl.t_unit;
}