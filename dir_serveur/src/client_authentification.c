#include "../includes/serveur.h"

/*
**	Check the reception of the client.
**	If received TEAM NAME, client goes into the game.
**	If received GRAPHIC, client is the gfx.
*/

void	client_authentification(t_serveur *serv, t_client_entity *client)
{
	char			*cmd;

	if ((cmd = get_first_cmd(&client->buff_recv)))
	{
		if (strncmp(cmd, "GRAPHIC\n", 8) == 0)
		{
			client_authenticate_gfx(serv, client);
		}
		else
		{
			client_authenticate_player(serv, client, cmd);
		}
		free(cmd);
	}
	return ;
}

/*
**	Setting the gfx. There can only be one gfx.
*/

void	client_authenticate_gfx(t_serveur *serv, t_client_entity *client)
{
	if (!serv->client_hdl.gfx_client)
	{
		serv->client_hdl.gfx_client = client;
		client->is_gfx = 1;
		send_current_world_state(serv, client); // see communication_gfx.c
		printf(KGRN "GFX client recognized\n" KRESET);
		return ;
	}
	else
	{
		printf(KRED "GFX client already set\n" KRESET);
		client->is_disconnecting = 1; // flagged, will be disconnected before next fd_set.
		return ;
	}
}

/*
**	Checking if the msg sent is a valid team
**	and if there is enough place in it for a new player. 
*/

void	client_authenticate_player(t_serveur *serv, t_client_entity *client, char *cmd)
{
	t_team_entity	*team;
	char			*str_to_send;
	t_egg			*egg;

	// does team exist ?
	if (!(team = get_team_by_name(serv, cmd)))
	{
		printf(KRED "Get_team() failed: team name not found: %s\n" KRESET, cmd);
		write_buffer(&client->buff_send, "UNKNOWN TEAM\n", 13);
		client->is_disconnecting = 1; // flagged, will be disconnected before next fd_set.
		return ;
	}

	// Is a slot available for client ?
	if (team->available_slots > 0)
	{
		// sending first datas; <nb-client>\n<x><y>\n
		client->is_in_game = 1;
		client->team = team;
		asprintf(&str_to_send, "%d\n%d %d\n", team->available_slots,
				serv->world_hdl.map_x, serv->world_hdl.map_y);
		write_buffer(&client->buff_send, str_to_send, strlen(str_to_send));
		free(str_to_send);
		// one slot now taken in team.
		team->available_slots -= 1;
		team->nb_players_per_lv[client->player.level - 1] += 1;
		if ((egg = egg_available(serv, client)) != NULL)
		{
			client->player.pos.x = egg->pos.x;
			client->player.pos.y = egg->pos.y;
			client->player.dir = rand() % 4;

			client->player.cur_case = &(serv->world_hdl
				.world_board[client->player.pos.y][client->player.pos.x]);
			client->player.cur_case->nb_players += 1;

			clear_egg(serv, egg);
			// gfx egg connection "ebo #e\n"
			asprintf(&str_to_send, "ebo #%d\n", egg->egg_nb);
			push_gfx_msg(serv, str_to_send);
			free(str_to_send);

			printf(KGRN "[Serveur]: Client #%d connected as egg #%d\n" KRESET,
					client->sock, egg->egg_nb);
		}
		else
		{
			assign_random_player_position(serv, &(client->player));
		}
		printf(KGRN "Client #%d player authentified. Team: [%s]. Position: %dx %dy\n" KRESET,
			client->sock, client->team->name, client->player.pos.x, client->player.pos.y);

		// assign time of dinner = now + FOOD_LIFE_TIME;
		get_time(&client->delay_time);
		assign_player_time_of_dinner(serv, &(client->player));

		// "pnw #n X Y O L N\n"
		asprintf(&str_to_send, "pnw #%d %d %d %d %d %s\n",
			client->sock,
			client->player.pos.x,
			client->player.pos.y,
			client->player.dir + 1, // +1 cause dir enum start at 0, and gfx protocol wants 1;
			client->player.level,
			client->team->name);
		push_gfx_msg(serv, str_to_send);
		free(str_to_send);
	}
	else
	{
		printf(KRED "No available slot in team: %s\n" KRESET, team->name);
		fflush(stdout);
		client->is_disconnecting = 1;
	}
	return ;
}
