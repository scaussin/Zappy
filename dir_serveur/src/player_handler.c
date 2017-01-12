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
