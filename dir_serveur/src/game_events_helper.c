#include "../includes/serveur.h"

/*
**	***********************************************************************************	**
**	This file contains helper functions for the games events, allowing the creation 	**
**	of global events that can be used at any moment and will be check in the main loop.	**
**																						**
**	***********************************************************************************	**
*/

/*
**	This helper function will be used in cmds, when a cmd generates another action later.
**	Incantation for example, will be started by a player then be applied 300 t unit later.
*/

void	init_game_event(t_world_event *world_event, char *type_str, int trigger_delay)
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
		{
			cur_event = cur_event->next;
		}
		cur_event->next = world_event;
		return ;
	}
}

/*
**	The event will have an impact when fired on clients linked through this function.
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
		{
			cur_client = cur_client->next;
		}
		cur_client->next = new_client;
		return ;
	}
}

/*
**	To clean event and its concerned clients after it has been fired.
*/

void	delete_game_event(t_serveur *serv, t_world_event *target_event)
{
	t_world_event		*cur_event;
	//t_world_event		*event_to_free;
	t_event_client		*client_tmp;
	t_event_client		*client_to_free;

	if (serv->world_hdl.ongoing_events) // any event waiting ?
	{
		cur_event = serv->world_hdl.ongoing_events;
		while (cur_event)
		{
			if (cur_event->next && cur_event->next == target_event)
			{
				//event_to_free = cur_event->next;
				// free inner chained list of concerned players.
				if (target_event->type)
					free (target_event->type);
				if (target_event->concerned_clients)
				{
					client_tmp = target_event->concerned_clients;
					while (client_tmp)
					{
						client_to_free = client_tmp;
						client_tmp = client_tmp->next;
						free(client_to_free);
					}
				}
			}
		}
	}

}