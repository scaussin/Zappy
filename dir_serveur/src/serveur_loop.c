
#include "../includes/serveur.h"

void	init_fd(t_serveur *serv)
{
	t_client_entity	*p_client;

	// Clear le set de fd ready pour lecture
	FD_ZERO(serv->network.read_fs);
	FD_ZERO(serv->network.write_fs);

	// Surveille l'entrée standard pour exit propre quand enter presse
	FD_SET(STDIN_FILENO, serv->network.read_fs);

	// Surveille le endpoint pour new connection
	FD_SET(serv->network.sock_serveur, serv->network.read_fs);

	// Si un ou plusieurs clients sont marques pour deconnection.
	disconnect_flagged_clients(serv);

	// Surveille tous les client
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
		// Before executing clients cmds, we shall check for
		// game modifiying events.
		check_game_events(serv);

		init_fd(serv);

		ret_select =  select(serv->network.sock_max + 1, serv->network.read_fs, 
			serv->network.write_fs, NULL, set_timeout_select(&timeout, lower_time_end));
		if (ret_select < 0) //wake up select : error
			exit_error("select()");
		else if (ret_select > 0) //wake up select : event read or write
		{
			// Exit when enter presse
			if (FD_ISSET(STDIN_FILENO, serv->network.read_fs))
				return;
			// Connect new client
			if (FD_ISSET(serv->network.sock_serveur, serv->network.read_fs))
				new_client_connection(serv);
			// Check commands from clients and fill all clients buffers
			check_all_clients_communication(serv);
			// Treat datas from buffers previously filled.
			manage_clients_input(serv);
		}
		//printf(KYEL "." KRESET);
		//fflush(stdout);
		lower_time_end = exec_cmd_client(serv);
	}
}

/*
**	Redirecting input checking toward corresponding function.
**	If the client hasnt given its team name, he is not in game,
**	and his input check is different.
*/

void manage_clients_input(t_serveur *serv)
{
	t_client_entity		*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->buff_recv.len > 0) // is there something to read ?
		{
			if (!p_client->is_in_game && !p_client->is_gfx && !p_client->is_player_dead)
			{
				// check if received TEAM NAME or GRAPHIC for client setting.
				client_authentification(serv, p_client);
				// note : if client is invalid, he will be marked for disconnection
				// and get disconnected at next server turn.
			}
			else if (p_client->is_in_game && !p_client->is_player_dead && !p_client->is_gfx)
			{
				// client is in game and not gfx and not dead, everything he sends is cmds.
				lex_and_parse_cmds(p_client, serv->cmd_hdl.cmd_match_table);
			}
			else if (p_client->is_player_dead)
			{
				// we must wait for the client to receive "mort\n" before
				// disconnecting him.
				p_client->is_disconnecting = 1;
			}
			else if (p_client->is_gfx)
			{
				// it may be gfx commands.
				lex_and_parse_gfx_cmds(serv, p_client);
			}
		}
		p_client = p_client->next;
	}
}
