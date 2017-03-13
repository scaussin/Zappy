/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_authentification.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 14:59:26 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 15:29:58 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		send_current_world_state(serv, client);
		printf(KGRN "\nGFX client recognized\n" KRESET);
		return ;
	}
	else
	{
		printf(KRED "\nGFX client already set\n" KRESET);
		client->is_disconnecting = 1;
		return ;
	}
}

/*
**	Checking if the msg sent is a valid team
**	and if there is enough place in it for a new player.
*/

void	client_authenticate_player(t_serveur *serv, t_client_entity *client,
	char *cmd)
{
	t_team_entity	*team;

	if (serv->client_hdl.nb_clients > MAX_CLIENTS_CONNECTED)
	{
		printf(KMAG "\n- Connection refused, too many clients connected.\n"
			KRESET);
		client->is_disconnecting = 1;
		return ;
	}
	if (!(team = get_team_by_name(serv, cmd)))
	{
		printf(KRED "\nGet_team() failed: team name not found: %s\n"
			KRESET, cmd);
		write_buffer(&client->buff_send, "UNKNOWN TEAM\n", 13);
		client->is_disconnecting = 1;
		return ;
	}
	if (team->available_slots > 0)
		client_authenticate_slots_available(serv, client, team);
	else
	{
		printf(KRED "\nNo available slot in team: %s\n" KRESET, team->name);
		fflush(stdout);
		client->is_disconnecting = 1;
	}
}

void	client_authenticate_slots_available(t_serveur *serv,
	t_client_entity *client, t_team_entity *team)
{
	char			*str_to_send;
	t_egg			*egg;

	client->is_in_game = 1;
	client->team = team;
	client->team->cur_nb_players += 1;
	asprintf(&str_to_send, "%d\n%d %d\n", team->available_slots,
			serv->world_hdl.map_x, serv->world_hdl.map_y);
	write_buffer(&client->buff_send, str_to_send, strlen(str_to_send));
	free(str_to_send);
	team->available_slots -= 1;
	team->nb_players_per_lv[client->player.level - 1] += 1;
	if ((egg = egg_available(serv, client)) != NULL)
		assign_player_to_egg(serv, client, egg);
	else
		assign_random_player_position(serv, &(client->player));
	printf(KGRN "\nClient #%d player authentified. Team: [%s]. Position: %dx"
		"%dy\n" KRESET, client->sock, client->team->name, client->player.pos.x,
		client->player.pos.y);
	get_time(&client->delay_time);
	assign_player_time_of_dinner(serv, &(client->player));
	client_authenticate_send_gfx_msg(serv, client);
}

void	assign_player_to_egg(t_serveur *serv, t_client_entity *client,
	t_egg *egg)
{
	char			*str_to_send;

	client->player.pos.x = egg->pos.x;
	client->player.pos.y = egg->pos.y;
	client->player.dir = rand() % 4;
	client->player.cur_case = &(serv->world_hdl.world_board
		[client->player.pos.y][client->player.pos.x]);
	client->player.cur_case->nb_players += 1;
	clear_egg(serv, egg);
	asprintf(&str_to_send, "ebo #%d\n", egg->egg_nb);
	push_gfx_msg(serv, str_to_send);
	free(str_to_send);
	printf(KGRN "\n[Serveur]: Client #%d connected as egg #%d\n" KRESET,
			client->sock, egg->egg_nb);
}
