/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_prend.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:07:59 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:08:01 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_prend(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	int			res;
	char		*gfx_msg;

	if (!param || !regex_match(param, "^[a-zA-Z0-6]+\n$"))
	{
		printf(KMAG "Bad format to cmd [prend] "
					"from sock %d\n" KRESET, client_cur->sock);
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		return (-1);
	}
	if (serv->settings_hdl.is_pickup_instant == B_TRUE)
		cmd_prend(serv, client_cur, param);
	res = parse_ressource_index(param);
	asprintf(&gfx_msg, "pgt #%d %d\n", client_cur->sock, res);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
	return (0);
}

void	on_end_cmd_prend(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	char *gfx_msg;

	if (serv->settings_hdl.is_pickup_instant == B_FALSE)
		cmd_prend(serv, client_cur, param);
	if (client_cur->list_cmds->success == 1)
	{
		write_buffer(&client_cur->buff_send, "ok\n", 3);
		asprintf(&gfx_msg, "pin #%d\n", client_cur->sock);
		gfx_cmd_pin(serv, serv->client_hdl.gfx_client, gfx_msg);
		free(gfx_msg);
		asprintf(&gfx_msg, "bct %d %d\n", client_cur->player.pos.x,
			client_cur->player.pos.y);
		gfx_cmd_bct(serv, serv->client_hdl.gfx_client, gfx_msg);
		free(gfx_msg);
	}
	else
		write_buffer(&client_cur->buff_send, "ko\n", 3);
}

void	cmd_prend(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	int		res_i;

	(void)serv;
	if ((res_i = parse_ressource_index(param)) >= 0 &&
		(try_to_take_res(&client_cur->player, res_i)) >= 0)
		client_cur->list_cmds->success = 1;
	else
		client_cur->list_cmds->success = 0;
}

/*
**	Get index for ressource from param string, splitting it at the space.
**	Returns the index of the ressource (0-6 cf gfx_protocol) or -1
**	if no ressource is found.
**	Example : "prend 1" and "pose linemate" will both return (1);
*/

int		parse_ressource_index(char *param)
{
	int		arg_len;

	arg_len = strlen(param) - 1;
	if (dual_strncmp(param, "0", "nourriture", arg_len) == 0)
		return (0);
	else if (dual_strncmp(param, "1", "linemate", arg_len) == 0)
		return (1);
	else if (dual_strncmp(param, "2", "deraumere", arg_len) == 0)
		return (2);
	else if (dual_strncmp(param, "3", "sibur", arg_len) == 0)
		return (3);
	else if (dual_strncmp(param, "4", "mendiane", arg_len) == 0)
		return (4);
	else if (dual_strncmp(param, "5", "phiras", arg_len) == 0)
		return (5);
	else if (dual_strncmp(param, "6", "thystame", arg_len) == 0)
		return (6);
	else
		printf(KMAG "Invalid parameter for cmd [%s]\n" KRESET, param);
	return (-1);
}

/*
** Client try to take res on its current case.
** Returns the res index if success, -1 otherwise.
*/

int		try_to_take_res(t_player *player, int res_nb)
{
	if (player->cur_case->ressources[res_nb] > 0)
	{
		player->inventory[res_nb] += 1;
		player->cur_case->ressources[res_nb] -= 1;
		return (res_nb);
	}
	return (-1);
}
