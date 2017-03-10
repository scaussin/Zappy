/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_player_events.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 19:14:49 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 19:14:51 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Check events that must be check on a "per player" basis.
**	Reuniting them here allows to make only one run through the players.
*/

void	check_players_events(t_serveur *serv)
{
	t_client_entity		*cur_client;

	cur_client = serv->client_hdl.list_clients;
	while (cur_client)
	{
		if (!cur_client->is_gfx
			&& cur_client->is_in_game
			&& !cur_client->is_player_dead
			&& !cur_client->is_disconnecting)
		{
			check_player_life(serv, cur_client);
		}
		cur_client = cur_client->next;
	}
}

/*
**	For current player, we check if he must die of hunger.
**	At zero, the player dies at ~12.6 sec for 126 of food life time
**	and t_unit 100.
**	Exception : incanter cant die, or else the incantation will be
**	lost in suspens.
**	He will die at the end of his incantation and NOT get his level up.
*/

void	check_player_life(t_serveur *serv, t_client_entity *cur_client)
{
	char				*gfx_msg;

	if (cur_client->player.is_incanter
		== 0 && cur_client->player.inventory[FOOD] <= 0)
	{
		cur_client->is_player_dead = 1;
		cur_client->team->nb_players_per_lv[cur_client->player.level - 1] -= 1;
		write_buffer(&cur_client->buff_send, "mort\n", 5);
		printf(KMAG "player %d died: death by hunger\n" KRESET,
			cur_client->sock);
		asprintf(&gfx_msg, "pdi #%d\n", cur_client->sock);
		push_gfx_msg(serv, gfx_msg);
		free(gfx_msg);
	}
	else if (timespec_is_over(cur_client->player.next_dinner_time) == 1)
	{
		cur_client->player.inventory[FOOD] -= 1;
		assign_player_time_of_dinner(serv, &cur_client->player);
	}
}

/*
**	Refresh each player dinner time. Called when the server t unit is
**	modified during runtime.
*/

void	refresh_player_dinner_time(t_serveur *serv, t_client_entity *client,
									float old_t_unit)
{
	long				time_left;
	long				nsec_left;
	struct timespec		now;
	struct timespec		timespec_life_left;

	if (client->player.inventory[FOOD] != 0)
	{
		get_time(&now);
		timespec_life_left = timespec_diff(&now,
			&client->player.next_dinner_time);
		nsec_left = convert_timespec_to_nsec(timespec_life_left);
		time_left = (long)roundf((float)(nsec_left /
			(float)1000000000) * (1 / old_t_unit));
		get_time(&client->player.next_dinner_time);
		add_nsec_to_timespec(&client->player.next_dinner_time,
		time_left * 1000000000 * serv->world_hdl.t_unit);
	}
}
