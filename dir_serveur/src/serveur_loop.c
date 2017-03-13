/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 19:08:42 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 19:12:54 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	init_fd(t_serveur *serv)
{
	t_client_entity	*p_client;

	FD_ZERO(serv->network.read_fs);
	FD_ZERO(serv->network.write_fs);
	FD_SET(STDIN_FILENO, serv->network.read_fs);
	FD_SET(serv->network.sock_serveur, serv->network.read_fs);
	disconnect_flagged_clients(serv);
	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		FD_SET(p_client->sock, serv->network.read_fs);
		if (p_client->buff_send.len > 0 || p_client->buff_send.len_overflow > 0)
			FD_SET(p_client->sock, serv->network.write_fs);
		p_client = p_client->next;
	}
}

/*
** Central function for server.
*/

void	main_loop(t_serveur *serv)
{
	int				ret_select;
	struct timeval	timeout;
	struct timespec	*lower_time_end;

	lower_time_end = NULL;
	printf(KCYN "- Server awaiting datas... -\n" KRESET);
	while (42)
	{
		serv->loop_nb += 1;
		check_game_events(serv);
		init_fd(serv);
		ret_select = select(serv->network.sock_max + 1, serv->network.read_fs,
			serv->network.write_fs, NULL, set_timeout_select(&timeout,
				lower_time_end));
		if (ret_select < 0)
			exit_error("select()");
		else if (ret_select > 0 && check_fd(serv) != 0)
			return ;
		lower_time_end = exec_cmd_client(serv);
	}
}

int		check_fd(t_serveur *serv)
{
	if (FD_ISSET(STDIN_FILENO, serv->network.read_fs))
		return (1);
	if (FD_ISSET(serv->network.sock_serveur, serv->network.read_fs))
		new_client_connection(serv);
	check_all_clients_communication(serv);
	manage_clients_input(serv);
	if ((serv->loop_nb % 500) == 0)
		printf("\n");
	return (0);
}

/*
**	Redirecting input checking toward corresponding function.
**	If the client hasnt given its team name, he is not in game,
**	and his input check is different.
*/

void	manage_clients_input(t_serveur *serv)
{
	t_client_entity		*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->is_player_dead)
			p_client->is_disconnecting = 1;
		if (p_client->buff_recv.len > 0)
		{
			if (!p_client->is_in_game && !p_client->is_gfx &&
				!p_client->is_player_dead)
				client_authentification(serv, p_client);
			else if (!serv->victory_reached && p_client->is_in_game &&
				!p_client->is_player_dead && !p_client->is_gfx)
				lex_and_parse_cmds(p_client, serv->cmd_hdl.cmd_match_table);
			else if (p_client->is_gfx)
				lex_and_parse_gfx_cmds(serv, p_client);
		}
		p_client = p_client->next;
	}
}
