
#include "../includes/serveur.h"

void	set_read_fs(t_serveur *serv)
{
	t_client_entity	*p_client;

	// Clear le set de fd ready pour lecture
	FD_ZERO(serv->network.read_fs);
	FD_ZERO(serv->network.write_fs);

	// Surveille l'entrée standard pour exit propre quand enter presse
	FD_SET(STDIN_FILENO, serv->network.read_fs);

	// Surveille le endpoint pour new connection
	FD_SET(serv->network.sock_serveur, serv->network.read_fs);

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
** Central function for server.
*/

void	main_loop(t_serveur *serv)
{
	printf(KCYN "- Server awaiting datas... -\n" KRESET);
	while (42)
	{
		set_read_fs(serv);

		// BOUYAKA SELECT IS HERE !!!!!!!!!!
		if (select(serv->network.sock_max + 1,
				serv->network.read_fs, serv->network.write_fs, NULL, NULL) < 0)
			exit_error("select()");

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
		exec_cmd_client(serv); // time
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
				client_recognition(serv, p_client);
			}
			else if (p_client->is_in_game && !p_client->is_gfx)
			{
				// client is in game, everything he sends is cmds.
				lex_and_parse_cmds(p_client, serv->cmd_match_table);
			}
		}
		p_client = p_client->next;
	}

}