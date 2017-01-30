#include "../../includes/serveur.h"

void	gfx_cmd_mct(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)param;
	(void)gfx_client;
	int					x;
	int					y;
	t_world_case		**world_board;
	char				*gfx_msg;

	if (!regex_match(param, "^mct\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [mct] "
					"from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	x = 0;
	y = 0;
	world_board = serv->world_hdl.world_board;
	while (y < serv->world_hdl.map_y)
	{
		while (x < serv->world_hdl.map_x)
		{
			asprintf(&gfx_msg, "bct %d %d %d %d %d %d %d %d %d\n",
				x,
				y,
				world_board[y][x].ressources[FOOD],
				world_board[y][x].ressources[LINEMATE],
				world_board[y][x].ressources[DERAUMERE],
				world_board[y][x].ressources[SIBUR],
				world_board[y][x].ressources[MENDIANE],
				world_board[y][x].ressources[PHIRAS],
				world_board[y][x].ressources[THYSTAME]);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
			x++;
		}
		y++;
		x = 0;
	}
}
