/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   egg_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:50:19 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 18:56:59 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Called when a player has finished laying an egg.
**	The spawned egg's status will be checked at each program's loop.
*/

void	add_new_egg(t_serveur *serv, t_client_entity *client)
{
	t_egg	*new_egg;

	new_egg = (t_egg *)s_malloc(sizeof(t_egg));
	serv->world_hdl.nb_of_eggs += 1;
	new_egg->egg_nb = serv->world_hdl.nb_of_eggs;
	new_egg->team = client->team;
	get_time(&new_egg->hatch_time);
	add_nsec_to_timespec(&new_egg->hatch_time,
		EGG_HATCH_TIME * serv->world_hdl.t_unit * 1000000000);
	new_egg->pos.x = client->player.pos.x;
	new_egg->pos.y = client->player.pos.y;
	new_egg->father_nb = client->sock;
	new_egg->has_hatched = 0;
	new_egg->next = NULL;
	printf(KGRN "[Serveur]: player #%d layed egg #%d\n" KRESET, client->sock,
		new_egg->egg_nb);
	add_egg_to_list(serv, new_egg);
}

void	add_egg_to_list(t_serveur *serv, t_egg *new_egg)
{
	t_egg	*egg_tmp;

	if (serv->world_hdl.eggs == NULL)
	{
		serv->world_hdl.eggs = new_egg;
		return ;
	}
	else
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp->next)
		{
			egg_tmp = egg_tmp->next;
		}
		egg_tmp->next = new_egg;
		return ;
	}
}

/*
**	For a given player, will check if there is an egg corresponding to its team.
**	If yes, will return the egg pointer, otherwise, will return NULL;
*/

t_egg	*egg_available(t_serveur *serv, t_client_entity *client)
{
	t_egg	*egg_tmp;

	if (serv->world_hdl.eggs)
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp)
		{
			if (egg_tmp->has_hatched && egg_tmp->team == client->team)
			{
				return (egg_tmp);
			}
			egg_tmp = egg_tmp->next;
		}
	}
	return (NULL);
}

/*
**	Called when the egg must be clear of the world's eggs.
**	Can happen when a player connected to a hatched egg, or when
**	an egg dies of hunger. Destroy the node of the egg in the chained list.
*/

void	clear_egg(t_serveur *serv, t_egg *egg)
{
	t_egg	*egg_tmp;

	if (serv->world_hdl.eggs == egg)
	{
		free(egg);
		serv->world_hdl.eggs = egg->next;
		serv->world_hdl.nb_of_eggs -= 1;
		return ;
	}
	else
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp->next && egg_tmp->next != egg)
			egg_tmp = egg_tmp->next;
		if (egg_tmp->next == egg)
		{
			egg_tmp->next = egg->next;
			free(egg);
			serv->world_hdl.nb_of_eggs -= 1;
		}
		else
			printf(KMAG "[Serveur]: could not find egg #%d to clear."
					KRESET, egg->egg_nb);
	}
}

/*
**	refresh the time of hatch for all the eggs unhatched on the serv.
**	Used when the time unit is changed during runtime.
*/

void	refresh_eggs_hatching_time(t_serveur *serv, float old_t_unit)
{
	t_egg				*egg_tmp;
	long				time_left;
	long				nsec_left;
	struct timespec		now;
	struct timespec		timespec_life_left;

	if (serv->world_hdl.eggs)
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp)
		{
			if (egg_tmp->has_hatched == 0)
			{
				get_time(&now);
				timespec_life_left = timespec_diff(&now, &egg_tmp->hatch_time);
				nsec_left = convert_timespec_to_nsec(timespec_life_left);
				time_left = (long)roundf((float)(nsec_left / (float)1000000000)
					* (1 / old_t_unit));
				get_time(&egg_tmp->hatch_time);
				add_nsec_to_timespec(&egg_tmp->hatch_time,
					time_left * 1000000000 * serv->world_hdl.t_unit);
			}
			egg_tmp = egg_tmp->next;
		}
	}
}
