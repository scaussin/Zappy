#include "../../includes/serveur.h"

void	gfx_cmd_bct(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	(void)serv;
	(void)gfx_client;
	(void)param;

	char		*arg;
	char		*gfx_msg;
	int			x;
	int			y;

	if (!regex_match(param, "^bct [0-9]+ [0-9]+\n$"))
	{
		printf(KMAG "Bad format to cmd [bct] "
					"from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	else
	{
		// transform string to int.
		errno = 0;
		arg = strchr(param, ' ') + 1;
		x = strtol(arg, NULL, 10);
		arg = strchr(param, ' ') + 1;
		y = strtol(arg, NULL, 10); // the ptr advanced.

		// boundary check -> protects against overflow and map too big.
		if (errno == ERANGE || errno == EINVAL ||
			x >= serv->world_hdl.map_x || y >= serv->world_hdl.map_y)
		{
			printf(KMAG "Bad format to cmd [bct] "
					"from gfx client (out of map case)\n" KRESET);
			write_buffer(&gfx_client->buff_send, "sbp\n", 4);
			return ;
		}
		asprintf(&gfx_msg, "bct %d %d %d %d %d %d %d %d %d\n",
				x,
				y,
				serv->world_hdl.world_board[y][x].ressources[FOOD],
				serv->world_hdl.world_board[y][x].ressources[LINEMATE],
				serv->world_hdl.world_board[y][x].ressources[DERAUMERE],
				serv->world_hdl.world_board[y][x].ressources[SIBUR],
				serv->world_hdl.world_board[y][x].ressources[MENDIANE],
				serv->world_hdl.world_board[y][x].ressources[PHIRAS],
				serv->world_hdl.world_board[y][x].ressources[THYSTAME]);
		push_gfx_msg(serv, gfx_msg);
		free(gfx_msg);
	}
}