#include "../includes/serveur.h"

/*
**	Main game checking function. Add functions to this to make checks.
*/

void	check_game_events(t_serveur *serv)
{
	check_players_life(serv);
	// TODO: check_eggs(); -> for each egg on the ground, check if they hatch or die of hunger.
	// TODO: check_game_over(); -> a team won, or all players died.
}

/*
**	For each player, we check if they must die of hunger.
*/

void	check_players_life(t_serveur *serv)
{
	t_client_entity		*cur_client;
	char				*gfx_msg;

	cur_client = serv->client_hdl.list_clients;
	while (cur_client)
	{
		if (!cur_client->is_gfx 
			&& cur_client->is_in_game 
			&& !cur_client->is_player_dead
			&& !cur_client->is_disconnecting)
		{
			if (cur_client->player.inventory[FOOD] == 0) // Works. At zero, the player dies at ~12.6 sec for 126 of food life time and t_unit 100.
			{
				cur_client->is_player_dead = 1;
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
		cur_client = cur_client->next;
	}
}

int is_equal(double x, double y)
{
  const double epsilon = 1e-5;
  return fabs(x - y) <= epsilon * fabs(x);
}