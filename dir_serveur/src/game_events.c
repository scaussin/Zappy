/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 19:03:48 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 19:03:49 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Main game checking function. Add functions to this to make checks.
*/

void	check_game_events(t_serveur *serv)
{
	check_players_events(serv);
	check_eggs(serv);
	check_victory(serv);
}

/*
**	Will check ongoing global events and fire them if it is their time.
**	--> Unused right now.
*/

void	check_world_events(t_serveur *serv)
{
	t_world_event	*event_tmp;

	if (serv->world_hdl.ongoing_events)
	{
		event_tmp = serv->world_hdl.ongoing_events;
		while (event_tmp)
		{
			if (timespec_is_over(event_tmp->time) == 1)
			{
				printf("event to fire : %s\n", event_tmp->type);
				event_tmp = serv->world_hdl.ongoing_events;
			}
			if (event_tmp)
				event_tmp = event_tmp->next;
		}
	}
}

void	check_eggs(t_serveur *serv)
{
	t_egg	*egg_tmp;

	if (serv->world_hdl.eggs)
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp)
		{
			if (egg_tmp->has_hatched == 0)
			{
				check_egg_eclosion(serv, egg_tmp);
			}
			else
			{
				check_egg_death(serv, egg_tmp);
			}
			if (egg_tmp)
				egg_tmp = egg_tmp->next;
		}
	}
}

/*
**	Refresh the time of every ongoing action to be coherent with
**	server's time unit. Used when the time unit is modified during
**	runtime.
*/

void	refresh_times(t_serveur *serv, float old_t_unit)
{
	t_client_entity			*p_client;
	struct timespec			timespec_life_left;
	struct timespec			now;
	long					time_left;
	long					nsec_left;

	p_client = serv->client_hdl.list_clients;
	refresh_eggs_hatching_time(serv, old_t_unit);
	while (p_client)
	{
		refresh_player_dinner_time(serv, p_client, old_t_unit);
		if (p_client->list_cmds && p_client->list_cmds->cmd_started == B_TRUE)
		{
			get_time(&now);
			timespec_life_left = timespec_diff(&now,
				&p_client->list_cmds->end_time);
			nsec_left = convert_timespec_to_nsec(timespec_life_left);
			time_left = (long)roundf((float)(nsec_left / (float)1000000000)
				* (1 / old_t_unit));
			get_time(&p_client->list_cmds->end_time);
			add_nsec_to_timespec(&p_client->list_cmds->end_time,
				time_left * 1000000000 * serv->world_hdl.t_unit);
		}
		p_client = p_client->next;
	}
}
