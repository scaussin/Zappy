#include "../includes/serveur.h"

/*
**	Main game checking function. Add functions to this to make checks.
*/

void	check_game_events(t_serveur *serv)
{
	check_world_events(serv); // no events created for now, but lets keep it in case of.
	check_players_events(serv); // game_player_events.c
	check_eggs(serv); // -> for each egg on the ground, check if they hatch or die of hunger.
	check_victory(serv);
}

/*
**	Will check ongoing global events and fire them if it is their time.
**	--> Unused right now.
*/

void	check_world_events(t_serveur *serv)
{
	t_world_event	*event_tmp;

	if (serv->world_hdl.ongoing_events) // any event waiting ?
	{
		event_tmp = serv->world_hdl.ongoing_events;
		while (event_tmp)
		{
			if (timespec_is_over(event_tmp->time) == 1)
			{
				// fire event.
				printf("event to fire : %s\n", event_tmp->type);
				// TODO : call event functions.
				delete_game_event(serv, event_tmp);
				event_tmp = serv->world_hdl.ongoing_events;
			}
			if (event_tmp) // protect against solo event deleted.
				event_tmp = event_tmp->next;
		}
	}
}

void	check_eggs(t_serveur *serv)
{
	t_egg	*egg_tmp;
	char	*gfx_msg;

	if (serv->world_hdl.eggs) // any eggs spawned on the world ?
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp)
		{
			if (egg_tmp->has_hatched == 0)
			{
				if (timespec_is_over(egg_tmp->hatch_time) == 1) // eclosion time, egg!
				{
					egg_tmp->has_hatched = 1;
					// egg increases nb of team slots(limited to define)
					if (serv->settings_hdl.are_teams_growing == B_TRUE)
					{
						if (egg_tmp->team->available_slots < MAX_NB_OF_CLIENTS_PER_TEAM)
							egg_tmp->team->available_slots += 1;
						else // hidden limit for nb of client per team.
						{
							printf(KMAG "[Serveur]: max nb of client reached for team %s\n" KRESET,
								egg_tmp->team->name);
							clear_egg(serv, egg_tmp);
							return ;
						}
					}

					// set the death time the same as a player. 10 * food.
					get_time(&egg_tmp->death_time);
					add_nsec_to_timespec(&egg_tmp->death_time,
						FOOD_LIFE_TIME * 10 * serv->world_hdl.t_unit * 1000000000);

					// gfx egg eclosion "eht #e\n"
					asprintf(&gfx_msg, "eht #%d\n", egg_tmp->egg_nb);
					push_gfx_msg(serv, gfx_msg);
					free(gfx_msg);

					printf(KGRN "[Serveur]: egg #%d hatched! "
								"A player can now connect through the egg.\n" KRESET,
								egg_tmp->egg_nb);
				}
			}
			else // egg already hatched, check death of egg.
				//	Only a player connection of the same team will save an egg from death.
			{
				if (timespec_is_over(egg_tmp->death_time) == 1)
				{
					// egg death takes out slot.
					if (serv->settings_hdl.are_teams_growing == B_TRUE)
					{
						egg_tmp->team->available_slots -= 1;
					}
					// gfx egg death "edi #e\n"
					asprintf(&gfx_msg, "edi #%d\n", egg_tmp->egg_nb);
					push_gfx_msg(serv, gfx_msg);
					free(gfx_msg);

					printf(KGRN "[Serveur]: egg #%d died of hunger. " 
								"Egg removed from server.\n" KRESET, egg_tmp->egg_nb);

					// clear node and restart the check.
					clear_egg(serv, egg_tmp);
					egg_tmp = serv->world_hdl.eggs;
				}
			}
			if (egg_tmp)
				egg_tmp = egg_tmp->next;
		}
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
				// gfx send victory "seg N\n"
				asprintf(&gfx_msg, "seg %s\n", serv->team_hdl.array_teams[i].name);
				push_gfx_msg(serv, gfx_msg);
				free(gfx_msg);
			}
			i++;
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

	// refresh all eggs on the serv.
	refresh_eggs_hatching_time(serv, old_t_unit);
	while (p_client)
	{
		refresh_player_dinner_time(serv, p_client, old_t_unit);
		
		// refresh cmd for each player
		if (p_client->list_cmds && p_client->list_cmds->cmd_started == B_TRUE)
		{
			get_time(&now);
			timespec_life_left = timespec_diff(&now, &p_client->list_cmds->end_time);

			// Time conversion to nanoseconds for precise time remaining.
			nsec_left = convert_timespec_to_nsec(timespec_life_left);
			time_left = (long)roundf((float)(nsec_left / (float)1000000000) * (1 / old_t_unit));

			get_time(&p_client->list_cmds->end_time);
			add_nsec_to_timespec(&p_client->list_cmds->end_time,
				time_left * 1000000000 * serv->world_hdl.t_unit);

			// debug print checks.
			// timespec_life_left = timespec_diff(&now, &p_client->list_cmds->end_time);
			// printf("serveur: real sec left: %ld.%ld\n", timespec_life_left.tv_sec,
			// 		timespec_life_left.tv_nsec);
			// printf("serveur: t unit left: %ld.\n", time_left);
		}
		p_client = p_client->next;
	}
}
