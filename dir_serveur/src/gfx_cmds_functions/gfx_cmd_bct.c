/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_bct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:19:27 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:19:28 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command bct: asking for a case content.
*/

void	gfx_cmd_bct(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	int			x;
	int			y;

	if (!regex_match(param, "^bct [0-9]+ [0-9]+\n$"))
	{
		printf(KMAG "Bad format to cmd [bct] from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	else
	{
		errno = 0;
		get_param_values(param, &x, &y);
		if (errno == ERANGE || errno == EINVAL ||
			x >= serv->world_hdl.map_x || y >= serv->world_hdl.map_y)
		{
			printf(KMAG "Bad format to cmd [bct] "
					"from gfx client (out of map case)\n" KRESET);
			write_buffer(&gfx_client->buff_send, "sbp\n", 4);
			return ;
		}
		send_gfx_bct_msg(serv, x, y);
	}
}

void	get_param_values(char *param, int *x, int *y)
{
	char		*arg;

	arg = strchr(param, ' ') + 1;
	*x = strtol(arg, NULL, 10);
	arg = strchr(arg, ' ') + 1;
	*y = strtol(arg, NULL, 10);
}

void	send_gfx_bct_msg(t_serveur *serv, int x, int y)
{
	char		*gfx_msg;

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
