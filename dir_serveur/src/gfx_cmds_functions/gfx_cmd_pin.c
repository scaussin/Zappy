#include "../../includes/serveur.h"

/*
**	gfx command pin: asking for the inventory of one player by its sock number.
*/

void	gfx_cmd_pin(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	char				*arg;
	int					client_nb;
	t_client_entity		*client_tmp;
	char				*gfx_msg;

	// for time left;
	long				nsec_left;
	long				time_left;
	struct timespec		timespec_life_left;
	struct timespec		now;

	if (!regex_match(param, "^pin #[0-9]+\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [pin] "
					"from gfx client\n" KRESET);
		return ;
	}
	// transform string to int.
	errno = 0;
	arg = strchr(param, '#') + 1;
	client_nb = strtol(arg, NULL, 10);
	// check int overflow
	if (errno == ERANGE || errno == EINVAL)
	{
		printf(KMAG "Bad format to cmd [pin] "
				"from gfx client (bad integer format)\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	// search for client by number;
	client_tmp = serv->client_hdl.list_clients;
	while (client_tmp)
	{
		if (client_tmp->sock == client_nb
			&& !client_tmp->is_gfx
			&& !client_tmp->is_player_dead
			&& !client_tmp->is_disconnecting)
		{
			// client found

		// Calculating the food as time value ------------------------------- //
			// we set the time at food - 1 for the untouched food
			time_left = (client_tmp->player.inventory[FOOD] - 1) * FOOD_LIFE_TIME;

			// now we want the status of the current food.
			get_time(&now);
			timespec_life_left = timespec_diff(&now, &client_tmp->player.next_dinner_time);

			// Time conversion to nanoseconds for precise time remaining.
			nsec_left = convert_timespec_to_nsec(timespec_life_left);

			// calculation to turn remaining time into (int)t_unit time.
			time_left += (int)roundf((float)(nsec_left / (float)1000000000) / serv->world_hdl.t_unit);
		// END -------------------------------- //

			// send inventaire "pin #n X Y q q q q q q q\n"
			asprintf(&gfx_msg, "pin #%d %d %d %ld %d %d %d %d %d %d\n",
			client_tmp->sock,
			client_tmp->player.pos.x,
			client_tmp->player.pos.y,
			time_left,
			client_tmp->player.inventory[LINEMATE],
			client_tmp->player.inventory[DERAUMERE],
			client_tmp->player.inventory[SIBUR],
			client_tmp->player.inventory[MENDIANE],
			client_tmp->player.inventory[PHIRAS],
			client_tmp->player.inventory[THYSTAME]);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
			return ;
		}
		client_tmp = client_tmp->next;
	}
	printf(KMAG "[Serveur]: gfx cmd [pin]: "
				"could not find asked player\n" KRESET);
	asprintf(&gfx_msg, "smg could not find asked player for cmd [pin]\n");
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}
