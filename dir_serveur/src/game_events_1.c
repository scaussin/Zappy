/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_events_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 19:05:27 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 19:05:28 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	check_egg_eclosion(t_serveur *serv, t_egg *egg_tmp)
{
	int		food_life_time;

	food_life_time = FOOD_LIFE_TIME;
	if (timespec_is_over(egg_tmp->hatch_time) == 1)
	{
		egg_tmp->has_hatched = 1;
		if (serv->settings_hdl.are_teams_growing == B_TRUE)
		{
			if (egg_tmp->team->available_slots < MAX_NB_OF_CLIENTS_PER_TEAM)
				egg_tmp->team->available_slots += 1;
			else
			{
				printf(KMAG "[Serveur]: max client nb reached for team %s\n"
					KRESET, egg_tmp->team->name);
				clear_egg(serv, egg_tmp);
				return ;
			}
		}
		set_hatched_egg_datas(serv, egg_tmp);
		printf(KGRN "[Serveur]: egg #%d hatched!\n" KRESET, egg_tmp->egg_nb);
	}
}

void	set_hatched_egg_datas(t_serveur *serv, t_egg *egg_tmp)
{
	char	*gfx_msg;

	get_time(&egg_tmp->death_time);
	add_nsec_to_timespec(&egg_tmp->death_time,
		FOOD_LIFE_TIME * 10 * serv->world_hdl.t_unit * 1000000000);
	asprintf(&gfx_msg, "eht #%d\n", egg_tmp->egg_nb);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}

void	check_egg_death(t_serveur *serv, t_egg *egg_tmp)
{
	char	*gfx_msg;

	if (timespec_is_over(egg_tmp->death_time) == 1)
	{
		if (serv->settings_hdl.are_teams_growing == B_TRUE)
			egg_tmp->team->available_slots -= 1;
		asprintf(&gfx_msg, "edi #%d\n", egg_tmp->egg_nb);
		push_gfx_msg(serv, gfx_msg);
		free(gfx_msg);
		printf(KGRN "[Serveur]: egg #%d died of hunger. "
					"Egg removed from server.\n" KRESET, egg_tmp->egg_nb);
		clear_egg(serv, egg_tmp);
		egg_tmp = serv->world_hdl.eggs;
	}
}

/*
**	For each team, check if the nb of player by level is the targeted one.
**	The team is updated at three moments:
**	- a player connects([LV1] += 1)
**	- a player gets a lv([NEW LV] += 1 ; [OLD LV] -= 1)
**	- a player dies ([LV] -= 1)
*/

void	check_victory(t_serveur *serv)
{
	char	*gfx_msg;
	int		i;

	i = 0;
	if (serv->victory_reached == 0)
	{
		while (i < serv->team_hdl.nb_teams)
		{
			if (serv->team_hdl.array_teams[i]
				.nb_players_per_lv[VICTORY_CDT_PLAYER_LV - 1] >=
				VICTORY_CDT_PLAYER_NB)
			{
				serv->victory_reached = 1;
				printf(KGRN "Victory condition reached for team %s!\n"KRESET,
					serv->team_hdl.array_teams[i].name);
				asprintf(&gfx_msg, "seg %s\n",
					serv->team_hdl.array_teams[i].name);
				push_gfx_msg(serv, gfx_msg);
				free(gfx_msg);
			}
			i++;
		}
	}
}
