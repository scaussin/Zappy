
#include "../includes/serveur.h"
void	set_read_fs(t_serveur *serv)
{
	t_client_entity	*p_client;

	// Clear le set de fd ready pour lecture
	FD_ZERO(serv->network.read_fs);

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

void	main_loop(t_serveur *serv)
{
	while (42)
	{
		set_read_fs(serv);

		// BOUYAKA SELECT IS HERE !!!!!!!!!!
		if (select(serv->network.sock_max + 1, serv->network.read_fs, serv->network.write_fs, NULL, NULL) < 0)
			exit_error("select()");

		// Exit when enter presse
		if (FD_ISSET(STDIN_FILENO, serv->network.read_fs))
			return;
		// Connect new client
		else if (FD_ISSET(serv->network.sock_serveur, serv->network.read_fs))
			new_client_connection(serv);
		// Check commands from clients
		else
			ckeck_all_clients_communication(serv, serv->network.read_fs);

			//wite

		// Todo : exec les commandes clients
		manage_cmd_clients(serv);
	}
}
