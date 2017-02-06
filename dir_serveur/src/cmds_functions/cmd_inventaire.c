#include "../../includes/serveur.h"

int		on_start_cmd_inventaire(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)	serv;

	if (param)
	{
		printf(KMAG "Bad format to cmd [inventaire] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	return (0);
}

void	on_end_cmd_inventaire(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	// exec command
	cmd_inventaire(serv, client_cur, param);
}

void	cmd_inventaire(struct s_serveur *serv, struct s_client_entity *client_cur, char *param)
{
	(void)				serv;
	(void)				param;
	t_player			*cur_player;
	char				*msg;
	long				nsec_left;
	long				time_left;
	struct timespec		timespec_life_left;
	struct timespec		now;

	cur_player = &(client_cur->player);
	
	// we set the time at food - 1 for the untouched food
	time_left = (cur_player->inventory[FOOD] - 1) * FOOD_LIFE_TIME;

	// now we want the status of the current food.
	get_time(&now);
	timespec_life_left = timespec_diff(&now, &cur_player->next_dinner_time);

	// Time conversion to nanoseconds for precise time remaining.
	nsec_left = convert_timespec_to_nsec(timespec_life_left);

	// calculation to turn remaining time into (int)t_unit time.
	time_left += (int)roundf((float)(nsec_left / (float)1000000000) / serv->world_hdl.t_unit);
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
