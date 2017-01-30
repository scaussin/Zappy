#include "../../includes/serveur.h"

void	cmd_inventaire(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	(void)				param;
	(void)				serv;
	t_player			*cur_player;
	char				*msg;

	cur_player = &(client_cur->player);

	if (!regex_match(param, "^inventaire\n$"))
	{
		printf(KMAG "Bad format to cmd [inventaire] "
					"from sock %d\n" KRESET, client_cur->sock);
		return ;
	}
	// player client response.
	asprintf(&msg, "{nourriture %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d}\n",  // nourriture ??
		cur_player->inventory[FOOD],
		cur_player->inventory[LINEMATE],
		cur_player->inventory[DERAUMERE],
		cur_player->inventory[SIBUR],
		cur_player->inventory[MENDIANE],
		cur_player->inventory[PHIRAS],
		cur_player->inventory[THYSTAME]);
	write_buffer(&client_cur->buff_send, msg, strlen(msg));
	free(msg);
}
