#include "../includes/serveur.h"

/*
** This file contains functions to handle clients' players.
*/

/*
** Randomly assign player position on the server's board.
*/

void		assign_random_player_position(t_serveur *serv, t_player *player)
{
	srand(time(NULL) + serv->loop_nb);
	player->pos.x = rand() % serv->world_hdl.map_x;
	player->pos.y = rand() % serv->world_hdl.map_y;
	player->dir = rand() % 3;

	player->cur_case = &(serv->world_hdl
						.world_board[player->pos.y][player->pos.x]);
	player->cur_case->nb_players += 1;
}


/*
**	Assign the time at which a player will consume one food in its inventory(dinner time).
*/

void		assign_player_time_of_dinner(t_serveur *serv, t_player *player)
{
	get_time(&player->next_dinner_time);
	add_nsec_to_timespec(&player->next_dinner_time,
		FOOD_LIFE_TIME * serv->world_hdl.t_unit * 1000000000);
}

/*
**	Returns the food as a t_unit value for the client.
**	It takes the time of all the food - 1, then calculate the ratio until
**	the next time of dinner for the currently eating food.
*/

long 	get_food_as_time(t_serveur *serv, t_client_entity *client)
{
	long				time_left;
	long				nsec_left;
	struct timespec		now;
	struct timespec		timespec_life_left;

	// we set the time at food - 1 for the untouched food
	time_left = (client->player.inventory[FOOD] - 1) * FOOD_LIFE_TIME;

	// now we want the status of the current food.
	get_time(&now);
	timespec_life_left = timespec_diff(&now, &client->player.next_dinner_time);

	// Time conversion to nanoseconds for precise time remaining.
	nsec_left = convert_timespec_to_nsec(timespec_life_left);

	// calculation to turn remaining time into (int)t_unit time.
	time_left += (long)roundf((float)(nsec_left / (float)1000000000) * (1 / serv->world_hdl.t_unit));
	return (time_left);
}