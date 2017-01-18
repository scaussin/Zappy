
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
		if (p_client->buff_send.len > 0)
			FD_SET(p_client->sock, serv->network.write_fs);
		p_client = p_client->next;
	}
}

/*
	struct timeval
	{
		long	tv_sec;		// secondes
		long	tv_usec;	// microsecondes -> 1 000 000 us == 1s
	};
	struct timespec
	{
		long	tv_sec;		// secondes
		long	tv_nsec;	// nanosecondes -> 1 000 000 000 ns == 1s
	};
*/

struct timespec timespec_diff(struct timespec *start, struct timespec *stop)
{
	struct timespec result;

	if ((stop->tv_nsec - start->tv_nsec) < 0) {
		result.tv_sec = stop->tv_sec - start->tv_sec - 1;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	} else {
		result.tv_sec = stop->tv_sec - start->tv_sec;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
	return result;
}

struct timeval	*set_timeout_select(struct timeval *timeout, struct timespec *lower_time_end)
{
	struct timespec now;

	if (!lower_time_end) // no cmd in progress : timeout is useless
	{
		//printf(KYEL "timeout select: NULL (sleeping)\n" KRESET);
		return (NULL);
	}
	get_time(&now);
	if (min_timespec(&now, lower_time_end) == &now)
	{
		now = timespec_diff(&now, lower_time_end);
		//printf(KYEL "diff: %lds %ldns\n" KRESET, now.tv_sec, now.tv_nsec);
		timeout->tv_usec = (now.tv_nsec / 1000);
		timeout->tv_sec = now.tv_sec;// + ((now.tv_nsec * 1000) / 1000000);
	}
	else
	{
		timeout->tv_sec = 0;
		timeout->tv_usec = 0;
	}
	//printf(KYEL "timeout select: %06lds %06ldus\n" KRESET, timeout->tv_sec, (long)timeout->tv_usec);
	return (timeout);	
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
			else if (FD_ISSET(serv->network.sock_serveur, serv->network.read_fs))
				new_client_connection(serv);
			// Check commands from clients and fill all clients buffers
			else
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
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->buff_recv.len > 0) // is there something to read ?
		{
			if (!p_client->is_in_game && !p_client->is_gfx)
			{
				// check if received TEAM NAME or GRAPHIC for client setting.
				client_authentification(serv, p_client);
				// note : if client is invalid, he will be marked for disconnection
				// and get disconnected at next server turn.
			}
			else if (p_client->is_in_game && !p_client->is_gfx)
			{
				// client is in game and not gfx, everything he sends is cmds.
				lex_and_parse_cmds(p_client, serv->cmd_hdl.cmd_match_table);
			}
		}
		p_client = p_client->next;
	}

}
