#include "../includes/serveur.h"

void	main_loop(t_serveur *serv)
{
	int		i;
	fd_set	read_fs;

	while (42)
	{
		// Clear le set de fd ready pour lecture
		FD_ZERO(&read_fs);
		// Surveille l'entrée standard pour exit propre quand enter presse
		FD_SET(STDIN_FILENO, &read_fs);
		// Surveille le endpoint pour new connection
		FD_SET(serv->network.sock_endpoint, &read_fs);
		// Surveille tous les client
		i = -1;
		while (++i < serv.nb_clients)
			FD_SET(serv.list_clients[i].sock, &read_fs);

		// BOUYAKA SELECT IS HERE !!!!!!!!!!
		if(select(serv->network.sock_max + 1, &read_fs, NULL, NULL, NULL) < 0)
			exit_error("select()");

		// Exit when enter presse
		if (FD_ISSET(STDIN_FILENO, &read_fs))
			return;
		// Connect new client
		else if (FD_ISSET(serv->network.sock_endpoint, &read_fs))
			new_client_connection(&serv);
		// Check commands from clients
		else
			ckeck_all_clients_communication(&serv, &read_fs);

		// Todo : exec les commandes clients
		manage_cmd_clients(&serv);
	}
}
