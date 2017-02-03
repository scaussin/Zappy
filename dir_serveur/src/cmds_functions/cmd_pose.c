#include "../../includes/serveur.h"

void	cmd_pose(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	int		res_i;
	char	*gfx_msg;

	if (!param || !regex_match(param, "^[a-zA-Z0-6]+\n$"))
	{
		printf(KMAG "Bad format to cmd [pose] "
					"from sock %d\n" KRESET, client_cur->sock);
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		return ;
	}
	else
	{
		if ((res_i = parse_ressource_index(param)) >= 0 && // from cmd_prend.c
			(try_to_drop_ressource(&client_cur->player, res_i)) >= 0)
		{
			write_buffer(&client_cur->buff_send, "ok\n", 3);
			
			//"pgt #n i\n"
			asprintf(&gfx_msg, "pdr #%d %d\n", client_cur->sock, res_i);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);

			// send inventaire "pin #n X Y q q q q q q q\n" through cmd gfx_cmd_pin.
			asprintf(&gfx_msg, "pin #%d\n", client_cur->sock);
			gfx_cmd_pin(serv, serv->client_hdl.gfx_client, gfx_msg);
			free(gfx_msg);

			// gfx world block ressource update.
			asprintf(&gfx_msg, "bct %d %d\n",
				client_cur->player.pos.x,
				client_cur->player.pos.y);
			gfx_cmd_bct(serv, serv->client_hdl.gfx_client, gfx_msg);
			free(gfx_msg);
		}
		else
			write_buffer(&client_cur->buff_send, "ko\n", 3);
	}
}

/*
**	I strongly believe that a player will not try to drop an item
**	he doesnt have, but we never know.
*/

int try_to_drop_ressource(t_player *player, int res_nb)
{
	if (player->inventory[res_nb] > 0)
	{
		player->inventory[res_nb] -= 1;
		player->cur_case->ressources[res_nb] += 1;
		return (res_nb);
	}
	return (-1);
}
