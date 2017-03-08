/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_mct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:26:30 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:26:31 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command mct: asking for every case on the board's content.
*/

void	gfx_cmd_mct(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	int					x;
	int					y;

	if (!regex_match(param, "^mct\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [mct] from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
	}
	else
	{
		x = 0;
		y = 0;
		while (y < serv->world_hdl.map_y)
		{
			while (x < serv->world_hdl.map_x)
			{
				send_one_case_content(serv, x, y);
				x++;
			}
			y++;
			x = 0;
		}
	}
}

void	send_one_case_content(t_serveur *serv, int x, int y)
{
	char				*gfx_msg;
	t_world_case		**world_board;

	world_board = serv->world_hdl.world_board;
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
}
