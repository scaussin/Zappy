#include "../includes/serveur.h"

/*
**	Functions for communicating especially to the gfx.
*/

void	push_gfx_msg(t_serveur *serv, char *msg)
{
	if (serv->client_hdl.gfx_client)
	{
		write_buffer(&(serv->client_hdl.gfx_client->buff_send),
						msg, strlen(msg));
	}
}

void	send_current_world_state(t_serveur *serv, t_client_entity *gfx_client)
{
	char			*msg;
	int				y;
	int				x;
	t_world_case	**world_board;

	// ------------------------------------	//
	// sending World size					//
	// ------------------------------------	//
	// "msz X Y\n"
	asprintf(&msg, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	write_buffer(&gfx_client->buff_send, msg, strlen(msg));
	free(msg);

	// ------------------------------------	//
	// sending server time unit				//
	// ------------------------------------	//
	// "sgt T\n"
	asprintf(&msg, "sgt %f\n", serv->world_hdl.t_unit);
	push_gfx_msg(serv, msg);
	free(msg);

	// ------------------------------------	//
	// Sending map content					//
	// ------------------------------------	//
	// "bct X Y q q q q q q q\n" * nbr_cases
	x = 0;
	y = 0;
	world_board = serv->world_hdl.world_board;
	while (y < serv->world_hdl.map_y)
	{
		while (x < serv->world_hdl.map_x)
		{
			asprintf(&msg, "bct %d %d %d %d %d %d %d %d %d\n",
				x,
				y,
				world_board[y][x].ressources[FOOD],
				world_board[y][x].ressources[LINEMATE],
				world_board[y][x].ressources[DERAUMERE],
				world_board[y][x].ressources[SIBUR],
				world_board[y][x].ressources[MENDIANE],
				world_board[y][x].ressources[PHIRAS],
				world_board[y][x].ressources[THYSTAME]);
			push_gfx_msg(serv, msg);
			free(msg);
			x++;
		}
		y++;
		x = 0;
	}

	// ------------------------------------	//
	// Sending team names					//
	// ------------------------------------	//
	x = 0;
	while (x < serv->team_hdl.nb_teams)
	{
		asprintf(&msg, "tna %s\n", serv->team_hdl.array_teams[x].name);
		push_gfx_msg(serv, msg);
		free(msg);
		x++;
	}

	// ------------------------------------	//
	// Sending currently connected players	//
	// ------------------------------------	//
	t_client_entity	*tmp_client;
	tmp_client = serv->client_hdl.list_clients;
	while (tmp_client)
	{
		// we dont want to send non authentified and the gfx
		if (tmp_client->is_gfx == 0 && tmp_client->is_in_game == 1)
		{
			// send "pnw #n X Y O L N\n"
			asprintf(&msg, "pnw #%d %d %d %d %d %s\n",
				tmp_client->sock,
				tmp_client->player.pos.x,
				tmp_client->player.pos.y,
				tmp_client->player.dir + 1, // +1 cause enum start at 0, and gfx protocol wants 1;
				tmp_client->player.level,
				tmp_client->team->name);
			push_gfx_msg(serv, msg);
		}
		tmp_client = tmp_client->next;
	}
}