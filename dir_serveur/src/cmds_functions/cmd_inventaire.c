#include "../../includes/serveur.h"

void	cmd_inventaire(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	(void)				param;
	(void)				serv;
	t_player_inventory	*inventory;
	t_player			*cur_player;
	char				*msg;

	cur_player = &(client_cur->player);
	inventory = &(cur_player->inventory);

	// player client response.
	asprintf(&msg, "{nourriture %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d}\n",  // nourriture ??
		inventory->food,
		inventory->linemate,
		inventory->deraumere,
		inventory->sibur,
		inventory->mendiane,
		inventory->phiras,
		inventory->thystame);
	write_buffer(&client_cur->buff_send, msg, strlen(msg));
	free(msg);
}
