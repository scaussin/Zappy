/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_expulse_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 16:50:58 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 16:51:00 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

void	expulse_client_in_dir(t_serveur *serv, t_client_entity *client,
								int dir)
{
	char		*msg;
	char		*msg_client;
	t_player	*player;

	player = &(client->player);
	expulse_apply(serv, client, dir);
	asprintf(&msg_client, "deplacement %d\n", dir + 1);
	write_buffer(&client->buff_send, msg_client, strlen(msg_client));
	free(msg_client);
	asprintf(&msg, "ppo #%d %d %d %d\n", client->sock,
		player->pos.x,
		player->pos.y,
		player->dir + 1);
	push_gfx_msg(serv, msg);
	free(msg);
}

void	expulse_apply(t_serveur *serv, t_client_entity *client,
								int dir)
{
	t_player	*player;

	player = &(client->player);
	if (dir == UP)
		player->pos.y += 1;
	else if (dir == RIGHT)
		player->pos.x += 1;
	else if (dir == DOWN)
		player->pos.y -= 1;
	else if (dir == LEFT)
		player->pos.x -= 1;
	if (player->pos.x >= serv->world_hdl.map_x)
		client->player.pos.x = 0;
	else if (player->pos.x < 0)
		client->player.pos.x = serv->world_hdl.map_x - 1;
	if (player->pos.y >= serv->world_hdl.map_y)
		client->player.pos.y = 0;
	else if (player->pos.y < 0)
		client->player.pos.y = serv->world_hdl.map_y - 1;
	player->cur_case = &(serv->world_hdl.
			world_board[player->pos.y][player->pos.x]);
}
