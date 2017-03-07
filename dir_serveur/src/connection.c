#include "../includes/serveur.h"

/*
**	All connections are accepted by the program. They will be checked 
**	and closed if needed in the authentification process.
*/

SOCKET		accept_connection(t_serveur *serv)
{
	SOCKET			c_sock;
	SOCKADDR_IN		c_sin;
	socklen_t		c_sin_size;

	if (serv->client_hdl.nb_clients > MAX_CLIENTS_CONNECTED)
	{
		printf(KMAG "- Connection refused, too many clients connected.\n" KRESET);
		return (-1);
	}
	c_sin_size = sizeof(c_sin);
	if ((c_sock = accept(serv->network.sock_serveur,
		(SOCKADDR *)&c_sin, &c_sin_size)) < 0)
	{
		perror("accept()");
	}
	return (c_sock);
}

/*
**	This function will create a client on the server's global struct
**	and send it the "BIENVENUE\n" message.
**	At this point, we dont know if it is a client player, or gfx,
**	Until the said player responds and get through authentification.
*/

void		new_client_connection(t_serveur *serv)
{
	SOCKET				c_sock;
	t_client_entity		*client;

	printf(KGRN "\n- New client connection process started -\n" KRESET);
	if ((c_sock = accept_connection(serv)) < 0)
		return ;
	client = create_client(c_sock);
	write_buffer(&client->buff_send, "BIENVENUE\n", 10);
	serv->network.sock_max = c_sock > serv->network.sock_max ? c_sock : serv->network.sock_max;
	printf("sock_max: %d\n", serv->network.sock_max);
	add_client(serv, client);
	printf(KGRN "New client connected, sock : %d\n" KRESET, client->sock);
}

void		disconnect_client(SOCKET c_sock)
{
	char	*buff;

	buff = (char *)s_malloc(sizeof(char) * BUFF_SIZE);
	memset((void *)buff, 0, BUFF_SIZE);
	sprintf(buff, "Client disconnected, sock : %d", c_sock);
	printf("%s%s%s\n", KRED, buff, KRESET);
	close(c_sock);
	free(buff);
}

void		close_all_connections(t_serveur *serv)
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
