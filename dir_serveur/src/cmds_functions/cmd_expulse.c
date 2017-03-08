/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_expulse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 16:37:30 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 16:37:33 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_expulse(t_serveur *serv, t_client_entity *client_cur,
								char *param)
{
	char	*msg;

	if (param)
	{
		printf(KMAG "Bad format to cmd [expulse] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	asprintf(&msg, "pex #%d\n", client_cur->sock);
	push_gfx_msg(serv, msg);
	free(msg);
	return (0);
}

void	on_end_cmd_expulse(t_serveur *serv, t_client_entity *client_cur,
								char *param)
{
	cmd_expulse(serv, client_cur, param);
	if (client_cur->list_cmds->success == 1)
		write_buffer(&client_cur->buff_send, "ok\n", 3);
	else
		write_buffer(&client_cur->buff_send, "ko\n", 3);
}

void	cmd_expulse(t_serveur *serv, t_client_entity *client_cur,
						char *param)
{
	t_player				*cur_player;
	t_client_entity			*list_clients;
	int						nb_client_expulsed;

	(void)param;
	nb_client_expulsed = 0;
	cur_player = &(client_cur->player);
	list_clients = serv->client_hdl.list_clients;
	while (list_clients)
	{
		if ((expulse_cdts(list_clients, cur_player)) == 1)
		{
			expulsion_rule_check(serv, list_clients, cur_player);
			nb_client_expulsed++;
		}
		list_clients = list_clients->next;
	}
	if (nb_client_expulsed == 0)
		client_cur->list_cmds->success = 1;
	else
		client_cur->list_cmds->success = 0;
}

/*
**	Only players fulfilling those conditions will be expulsed.
*/

int		expulse_cdts(t_client_entity *tmp_client, t_player *player)
{
	if (tmp_client->is_in_game == 1
			&& tmp_client->is_gfx == 0
			&& tmp_client->is_disconnecting == 0
			&& tmp_client->player.pos.x == player->pos.x
			&& tmp_client->player.pos.y == player->pos.y
			&& &(tmp_client->player) != player)
		return (1);
	else
		return (0);
}

/*
**	Can we or not interrupt someone incanting ?
**	The case is handled here.
*/

void	expulsion_rule_check(t_serveur *serv, t_client_entity *list_clients,
			t_player *cur_player)
{
	if (!serv->settings_hdl.can_interrupt_incantation)
	{
		if (!list_clients->player.is_incanting
			&& !list_clients->player.is_incanter)
		{
			expulse_client_in_dir(serv, list_clients,
				cur_player->dir);
		}
	}
	else
		expulse_client_in_dir(serv, list_clients, cur_player->dir);
}
