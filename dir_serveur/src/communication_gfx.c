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
	char	*msg;
	int		y;
	int		x;
	t_world_case **world_board;

	// sending world size: "msz X Y\n"
	asprintf(&msg, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	write_buffer(&gfx_client->buff_send, msg, strlen(msg));
	free(msg);

	// sending server time: "sgt T\n"
	asprintf(&msg, "sgt %f\n", serv->world_hdl.t_unit);
	write_buffer(&gfx_client->buff_send, msg, strlen(msg));
	free(msg);

	// sending map content: "bct X Y q q q q q q q\n" * nbr_cases
	x = 0;
	y = 0;
	world_board = serv->world_hdl.world_board;
	while (y < serv->world_hdl.map_y)
	{
		while (x < serv->world_hdl.map_x)
		{
			asprintf(&msg, "bct %d %d %d %d %d %d %d\n",
				world_board[y][x].ressources.food,
				world_board[y][x].ressources.linemate,
				world_board[y][x].ressources.deraumere,
				world_board[y][x].ressources.sibur,
				world_board[y][x].ressources.mendiane,
				world_board[y][x].ressources.phiras,
				world_board[y][x].ressources.thystame);
			write_buffer(&gfx_client->buff_send, msg, strlen(msg));
			free(msg);
			x++;
		}
		y++;
		x = 0;
	}
}