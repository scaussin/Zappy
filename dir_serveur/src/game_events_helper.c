/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_events_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 19:11:04 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 19:11:05 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	init_game_event(t_world_event *world_event, char *type_str,
						int trigger_delay)
{
	world_event = (t_world_event *)s_malloc(sizeof(t_world_event));
	world_event->type = strdup(type_str);
	world_event->trigger_delay = trigger_delay;
	world_event->concerned_clients = NULL;
	world_event->next = NULL;
}

/*
**	Every event added here will be checked at each loop turn in game_events.c
*/

void	add_new_event(t_serveur *serv, t_world_event *world_event)
{
	t_world_event	*cur_event;

	if (serv->world_hdl.ongoing_events == NULL)
	{
		serv->world_hdl.ongoing_events = world_event;
		return ;
	}
	else
	{
		cur_event = serv->world_hdl.ongoing_events;
		while (cur_event->next)
			cur_event = cur_event->next;
		cur_event->next = world_event;
		return ;
	}
}

/*
**	The event will have an impact when fired on clients linked through
**	this function.
*/

void	add_client_to_event(t_world_event *world_event, t_client_entity *client)
{
	t_event_client		*cur_client;
	t_event_client		*new_client;

	new_client = (t_event_client *)s_malloc(sizeof(t_event_client));
	new_client->client = client;
	new_client->next = NULL;
	if (world_event->concerned_clients == NULL)
	{
		world_event->concerned_clients = new_client;
		return ;
	}
	else
	{
		cur_client = world_event->concerned_clients;
		while (cur_client->next)
			cur_client = cur_client->next;
		cur_client->next = new_client;
		return ;
	}
}
