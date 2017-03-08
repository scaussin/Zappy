/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:17:38 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 17:17:39 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	For each players on the same incantation as the incanter (including him),
**	makes the level up.
**	Lots of checks are made to avoid joining ongoing incantation.
**	Every incantation have an ID to avoid confusing them.
*/

void		finish_incantation(t_serveur *serv, t_client_entity *cur_client,
							int result)
{
	t_client_entity		*clients_tmp;
	char				*msg;

	printf(KGRN "[Server]: Incantation #%d ending.\n",
			cur_client->player.incantation_id);
	clients_tmp = serv->client_hdl.list_clients;
	while (clients_tmp)
	{
		if ((other_players_incantation_level_up_cdts(serv,
			cur_client, clients_tmp)) == 1)
			other_players_finish_incantation(serv, clients_tmp, result);
		clients_tmp = clients_tmp->next;
	}
	if (result == 1 && cur_client->player.inventory[FOOD] > 0)
	{
		level_up_clients_player(cur_client);
		send_level_messages(serv, cur_client);
	}
	cur_client->player.incantation_id = -1;
	cur_client->player.is_incanter = 0;
	serv->world_hdl.nb_of_incantations -= 1;
	asprintf(&msg, "bct %d %d\n", cur_client->player.pos.x,
		cur_client->player.pos.y);
	gfx_cmd_bct(serv, serv->client_hdl.gfx_client, msg);
	free(msg);
}

void		level_up_clients_player(t_client_entity *client)
{
	client->team->nb_players_per_lv[client->player.level - 1] -= 1;
	client->player.level += 1;
	client->player.nb_see_case = get_nb_case(client->player.level);
	client->team->nb_players_per_lv[client->player.level - 1] += 1;
}

void		send_level_messages(t_serveur *serv, t_client_entity *client)
{
	char				*msg;

	asprintf(&msg, "niveau actuel : %d\n", client->player.level);
	write_buffer(&client->buff_send, msg, strlen(msg));
	free(msg);
	asprintf(&msg, "plv #%d %d\n",
		client->sock, client->player.level);
	push_gfx_msg(serv, msg);
	free(msg);
}

int			other_players_incantation_level_up_cdts(t_serveur *serv,
			t_client_entity *cur_client, t_client_entity *clients_tmp)
{
	(void)serv;
	if (&clients_tmp->player != &cur_client->player
			&& clients_tmp->player.pos.x == cur_client->player.pos.x
			&& clients_tmp->player.pos.y == cur_client->player.pos.y
			&& clients_tmp->player.level == cur_client->player.level
			&& clients_tmp->player.is_incanting == 1
			&& clients_tmp->is_player_dead == 0
			&& clients_tmp->player.incantation_id
			== cur_client->player.incantation_id)
		return (1);
	else
		return (0);
}

void		other_players_finish_incantation(t_serveur *serv,
					t_client_entity *clients_tmp, int result)
{
	clients_tmp->player.is_incanting = 0;
	clients_tmp->player.incantation_id = -1;
	if (result == 1)
		level_up_clients_player(clients_tmp);
	send_level_messages(serv, clients_tmp);
}
