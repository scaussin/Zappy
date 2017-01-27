#include "../includes/serveur.h"

SOCKET	accept_connection(t_serveur *serv)
{
	SOCKET			c_sock;
	SOCKADDR_IN		c_sin;
	socklen_t		c_sin_size;
	int				i;
	int 			n_available_slots;

	i = 0;
	n_available_slots = 0;
	while (i < serv->team_hdl.nb_teams)
	{
		n_available_slots += serv->team_hdl.array_teams[i].available_slots;
		i++;
	}
	if  (n_available_slots == 0)
	{
		printf(KRED "No available slot\n" KRESET);
		return (-1);
	}
	printf("Slots available: %d\n", n_available_slots);
	c_sin_size = sizeof(c_sin);
	if ((c_sock = accept(serv->network.sock_serveur,
		(SOCKADDR *)&c_sin, &c_sin_size)) < 0)
	{
		perror("accept()");
	}
	return (c_sock);
}

void new_client_connection(t_serveur *serv)
{
	SOCKET				c_sock;
	t_client_entity		*client;

	// Debug Print
	printf(KGRN "\n- New client connection process started -\n" KRESET);

	// Accept connection
	if ((c_sock = accept_connection(serv)) < 0)
		return ;

	// Create Client and send "BIENVENU\n"
	client = create_client(c_sock);

	// Sending BIENVENUE\n message.
	write_buffer(&client->buff_send, "BIENVENUE\n", 10);

	// Set network Info
	serv->network.sock_max = c_sock > serv->network.sock_max ? c_sock : serv->network.sock_max;

	printf("sock_max: %d\n", serv->network.sock_max);
	// Add client to the list
	add_client(serv, client);

	// Debug Print
	printf(KGRN "New client connected, sock : %d\n" KRESET, client->sock);
}

void disconnect_client(SOCKET c_sock)
{
	char	*buff;

	buff = (char *)s_malloc(sizeof(char) * BUFF_SIZE);
	// Debug Print
	memset((void *)buff, 0, BUFF_SIZE);
	sprintf(buff, "Client disconnected, sock : %d", c_sock);
	printf("%s%s%s\n", KRED, buff, KRESET);

	close(c_sock);
	free(buff);
}

void close_all_connections(t_serveur *serv)
{
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	push_gfx_msg(serv, "smg SHUTDOWN\n");
	while (p_client)
	{
		close(p_client->sock);
		p_client = p_client->next;
	}
	close(serv->network.sock_serveur);
	printf(KGRN "Server disconnected\n" KRESET);
}
