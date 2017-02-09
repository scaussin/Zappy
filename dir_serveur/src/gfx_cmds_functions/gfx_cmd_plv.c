#include "../../includes/serveur.h"

/*
**	gfx command plv: asking for the level of one player by its sock number.
*/

void	gfx_cmd_plv(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)param;
	char				*arg;
	int					client_nb;
	t_client_entity		*client_tmp;
	char				*gfx_msg;

	if (!regex_match(param, "^plv #[0-9]+\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [plv] "
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
		printf(KMAG "Bad format to cmd [plv] "
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
			// send gfx : "plv #n L\n"
			asprintf(&gfx_msg, "plv #%d %d\n",
				client_tmp->sock, client_tmp->player.level);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
			return ;
		}
		client_tmp = client_tmp->next;
	}
	printf(KMAG "[Serveur]: gfx cmd [plv]: "
				"could not find asked player\n" KRESET);
	asprintf(&gfx_msg, "smg could not find asked player for cmd [plv]\n");
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}