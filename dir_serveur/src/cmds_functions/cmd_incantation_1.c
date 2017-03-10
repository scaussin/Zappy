/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:13:52 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 17:14:31 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	From the player's level, get the nb of ressources on case required for
**	incantation.
**	For example, lvl 1-2 need one player and one linemate -> ("1100000").
**	The returned int array will be used for comparison, and must be freed
**	with free();
*/

int			*set_incantation_target_res(int plevel)
{
	char		*target_res_str;
	int			*target_res;
	int			i;

	i = 0;
	target_res_str = fill_target_str_from_level(plevel);
	target_res = s_malloc(sizeof(int) * 7);
	while (i < 7)
	{
		target_res[i] = target_res_str[i] - 48;
		i++;
	}
	free(target_res_str);
	return (target_res);
}

char		*fill_target_str_from_level(int plevel)
{
	char		*target_res_str;

	if (plevel == 1)
		target_res_str = strdup("1100000");
	else if (plevel == 2)
		target_res_str = strdup("2111000");
	else if (plevel == 3)
		target_res_str = strdup("2201020");
	else if (plevel == 4)
		target_res_str = strdup("4112010");
	else if (plevel == 5)
		target_res_str = strdup("4121300");
	else if (plevel == 6)
		target_res_str = strdup("6123010");
	else if (plevel == 7)
		target_res_str = strdup("6222221");
	else
		target_res_str = NULL;
	return (target_res_str);
}

/*
**	Are all incantation conditions reunited ? If yes, returns 1.
**	Otherwise, returns 0;
*/

int			are_incantation_cdts_ok(t_serveur *serv, t_player *cur_player,
									int *target_res)
{
	int					nb_players_same_lvl;
	t_client_entity		*clients_tmp;

	nb_players_same_lvl = 0;
	clients_tmp = serv->client_hdl.list_clients;
	while (clients_tmp)
	{
		if (clients_tmp->player.pos.x == cur_player->pos.x
			&& clients_tmp->player.pos.y == cur_player->pos.y
			&& clients_tmp->player.level == cur_player->level
			&& clients_tmp->player.is_incanting == B_FALSE
			&& clients_tmp->player.is_incanter == B_FALSE)
			nb_players_same_lvl += 1;
		clients_tmp = clients_tmp->next;
	}
	if (nb_players_same_lvl >= target_res[0]
		&& cur_player->cur_case->ressources[LINEMATE] >= target_res[LINEMATE]
		&& cur_player->cur_case->ressources[DERAUMERE] >= target_res[DERAUMERE]
		&& cur_player->cur_case->ressources[SIBUR] >= target_res[SIBUR]
		&& cur_player->cur_case->ressources[MENDIANE] >= target_res[MENDIANE]
		&& cur_player->cur_case->ressources[PHIRAS] >= target_res[PHIRAS]
		&& cur_player->cur_case->ressources[THYSTAME] >= target_res[THYSTAME])
		return (1);
	return (0);
}

/*
**	For each player on the same case as the caster and with same
**	level (NOT including the caster),
**	set it as incanting = 1, blocking it until the caster launches
**	its incantion endpoint.
*/

void		set_players_incanting(t_serveur *serv, t_client_entity *cur_client)
{
	t_client_entity		*clients_tmp;
	char				*gfx_msg;
	char				*gfx_msg_cat;
	t_player			*cur_player;

	clients_tmp = serv->client_hdl.list_clients;
	cur_player = &cur_client->player;
	asprintf(&gfx_msg, "pic %d %d %d #%d", cur_player->pos.x, cur_player->pos.y,
		cur_player->level, cur_client->sock);
	while (clients_tmp)
	{
		if ((other_incanting_player_cdts(serv, cur_client, clients_tmp)) == 1)
		{
			put_player_in_incantation(serv, cur_client, clients_tmp);
			asprintf(&gfx_msg_cat, "%s #%d", gfx_msg, clients_tmp->sock);
			free(gfx_msg);
			gfx_msg = strdup(gfx_msg_cat);
			free(gfx_msg_cat);
		}
		clients_tmp = clients_tmp->next;
	}
	asprintf(&gfx_msg_cat, "%s\n", gfx_msg);
	free(gfx_msg);
	push_gfx_msg(serv, gfx_msg_cat);
	free(gfx_msg_cat);
}

int			other_incanting_player_cdts(t_serveur *serv,
										t_client_entity *cur_client,
										t_client_entity *clients_tmp)
{
	(void)serv;
	if (clients_tmp && cur_client
			&& &clients_tmp->player != &cur_client->player
			&& clients_tmp->is_gfx == B_FALSE
			&& clients_tmp->player.incantation_id == -1
			&& clients_tmp->player.is_incanting == B_FALSE
			&& clients_tmp->player.is_incanter == B_FALSE
			&& clients_tmp->player.pos.x == cur_client->player.pos.x
			&& clients_tmp->player.pos.y == cur_client->player.pos.y
			&& clients_tmp->player.level == cur_client->player.level)
		return (1);
	else
		return (0);
}
