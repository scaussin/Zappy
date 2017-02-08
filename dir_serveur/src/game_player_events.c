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
			// check_player_incantation_end(serv, cur_client);
			// check_player_laying_egg_end(serv, cur_client);
		}
		cur_client = cur_client->next;
	}
}

/*
**	For current player, we check if he must die of hunger.
**	At zero, the player dies at ~12.6 sec for 126 of food life time
**	and t_unit 100.
*/

void	check_player_life(t_serveur *serv, t_client_entity *cur_client)
{
	char				*gfx_msg;
	
	if (cur_client->player.inventory[FOOD] == 0) // Works. 
	{
		cur_client->is_player_dead = 1;
		cur_client->team->nb_players_per_lv[cur_client->player.level - 1] -= 1;

		// give back the slot.
		cur_client->team->available_slots += 1;

		// we do not disconnect him right away cause we want to send it "mort\n"
		write_buffer(&cur_client->buff_send, "mort\n", 5);
		printf(KMAG "player %d died: death by hunger\n" KRESET, cur_client->sock);
		// "pdi #n\n"
		asprintf(&gfx_msg, "pdi #%d\n", cur_client->sock);
		push_gfx_msg(serv, gfx_msg);
		free(gfx_msg);
	}
	else if (timespec_is_over(cur_client->player.next_dinner_time) == 1) // updates food in inventory.
	{
		cur_client->player.inventory[FOOD] -= 1;
		assign_player_time_of_dinner(serv, &cur_client->player);
	}
}

/*
**	Refresh each player dinner time. Called when the server t unit is modified during runtime.
*/

void	refresh_player_dinner_time(t_serveur *serv, t_client_entity *client, float old_t_unit)
{
	long				time_left;
	long				nsec_left;
	struct timespec		now;
	struct timespec		timespec_life_left;

	if (client->player.inventory[FOOD] != 0)
	{
		// we want the status of the current food.
		get_time(&now);
		timespec_life_left = timespec_diff(&now, &client->player.next_dinner_time);

		// Time conversion to nanoseconds for precise time remaining.
		nsec_left = convert_timespec_to_nsec(timespec_life_left);
		time_left = (long)roundf((float)(nsec_left / (float)1000000000) * (1 / old_t_unit));
		
		// reset dinner time.
		get_time(&client->player.next_dinner_time);
		add_nsec_to_timespec(&client->player.next_dinner_time,
		time_left * 1000000000 * serv->world_hdl.t_unit);
	}
}

// Now executed in cmd incantation endpoint on_end_cmd_fork();
// void	check_player_incantation_end(t_serveur	*serv, t_client_entity	*cur_client)
// {
// 	char	*client_msg;
// 	char	*gfx_msg;

// 	if (cur_client->player.is_incanting == 1)
// 	{
// 		if (timespec_is_over(cur_client->player.incantation_end_time) == 1)
// 		{
// 			cur_client->player.is_incanting = 0;
// 			// updates team and player level.
// 			cur_client->team->nb_players_per_lv[cur_client->player.level - 1] -= 1;
// 			cur_client->player.level += 1;
// 			cur_client->team->nb_players_per_lv[cur_client->player.level - 1] += 1;
			
// 			// send client: "niveau actuel : K"
// 			asprintf(&client_msg, "niveau actuel : %d\n", cur_client->player.level);
// 			write_buffer(&cur_client->buff_send, client_msg, strlen(client_msg));
// 			free(client_msg);

// 			// send gfx: "pie X Y R\n" for incantation leader only
// 			if (cur_client->player.is_incanter)
// 			{
// 				cur_client->player.is_incanter = 0;
// 				asprintf(&gfx_msg, "pie %d %d %d\n",
// 					cur_client->player.pos.x, cur_client->player.pos.y, 1);
// 				push_gfx_msg(serv, gfx_msg);
// 				free(gfx_msg);
// 			}

// 			// send gfx : "plv #n L\n"
// 			asprintf(&gfx_msg, "plv #%d %d\n",
// 				cur_client->sock, cur_client->player.level);
// 			push_gfx_msg(serv, gfx_msg);
// 			free(gfx_msg);

// 			// gfx world block ressource update.
// 			asprintf(&gfx_msg, "bct %d %d %d %d %d %d %d %d %d\n",
// 				cur_client->player.pos.x,
// 				cur_client->player.pos.y,
// 				cur_client->player.cur_case->ressources[FOOD],
// 				cur_client->player.cur_case->ressources[LINEMATE],
// 				cur_client->player.cur_case->ressources[DERAUMERE],
// 				cur_client->player.cur_case->ressources[SIBUR],
// 				cur_client->player.cur_case->ressources[MENDIANE],
// 				cur_client->player.cur_case->ressources[PHIRAS],
// 				cur_client->player.cur_case->ressources[THYSTAME]);
// 			push_gfx_msg(serv, gfx_msg);
// 			free(gfx_msg);
// 		}
// 	}
// }


// // Now executed in cmd fork endpoint on_end_cmd_fork();
// void	check_player_laying_egg_end(t_serveur	*serv, t_client_entity	*cur_client)
// {
// 	//char	*client_msg;
// 	char	*gfx_msg;

// 	if (cur_client->player.is_laying_egg == 1)
// 	{
// 		if (timespec_is_over(cur_client->player.egg_layed_time) == 1)
// 		{

// 			cur_client->player.is_laying_egg = 0;
// 			// the egg will be created and added to the event check.
// 			add_new_egg(serv, cur_client);
// 			// send gfx: "enw #e #n X Y\n"
// 			asprintf(&gfx_msg, "enw #%d\n", cur_client->sock);
// 			push_gfx_msg(serv, gfx_msg);
// 			free(gfx_msg);

// 		}
// 	}
// }