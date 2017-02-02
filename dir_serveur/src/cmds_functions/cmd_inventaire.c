#include "../../includes/serveur.h"

void	cmd_inventaire(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	(void)				param;
	(void)				serv;
	t_player			*cur_player;
	char				*msg;
	long				time_left;
	struct timespec		timespec_life_left;
	struct timespec		now;

	cur_player = &(client_cur->player);

	if (!regex_match(param, "^inventaire\n$"))
	{
		printf(KMAG "Bad format to cmd [inventaire] "
					"from sock %d\n" KRESET, client_cur->sock);
		return ;
	}
	// we set the time at food - 1 for the untouched food
	time_left = (cur_player->inventory[FOOD] - 1) * FOOD_LIFE_TIME;

	// now we want the status of the current food.
	get_time(&now);
	timespec_life_left = timespec_diff(&now, &cur_player->next_dinner_time);

	// Time conversion to nanoseconds for precise time remaining.
	while (timespec_life_left.tv_sec > 0)
	{
		timespec_life_left.tv_sec -= 1;
		timespec_life_left.tv_nsec += 1000000000;
	}
	// calculation to turn remaining time into (int)t_unit time.
	time_left += (int)roundf((float)(timespec_life_left.tv_nsec / (float)1000000000) / serv->world_hdl.t_unit);
	// player client response.
	asprintf(&msg, "{nourriture %ld, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d}\n",  // nourriture ??
		time_left,
		cur_player->inventory[LINEMATE],
		cur_player->inventory[DERAUMERE],
		cur_player->inventory[SIBUR],
		cur_player->inventory[MENDIANE],
		cur_player->inventory[PHIRAS],
		cur_player->inventory[THYSTAME]);
	write_buffer(&client_cur->buff_send, msg, strlen(msg));
	free(msg);
}
