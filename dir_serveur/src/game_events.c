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

void check_eggs(t_serveur *serv)
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

void check_victory(t_serveur *serv)
{
	char	*gfx_msg;
	int		i;

	i = 0;
	while (i < serv->team_hdl.nb_teams)
	{
		if (serv->team_hdl.array_teams[i]
			.nb_players_per_lv[VICTORY_CDT_PLAYER_LV - 1] == 
			VICTORY_CDT_PLAYER_NB)
		{
			printf(KGRN "Victory condition reached for team %s!\n"KRESET,
				serv->team_hdl.array_teams[i].name);
			// TODO: Victory code ?
			sleep(10); //
			// gfx send victory "seg N\n"
			asprintf(&gfx_msg, "seg %s\n", serv->team_hdl.array_teams[i].name);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
		}
		i++;
	}
}