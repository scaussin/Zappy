#include "../includes/serveur.h"

void new_client_connection(t_server_data *server)
{
  SOCKET      c_sock;
  SOCKADDR_IN c_sin;
  socklen_t      c_sin_size;

  c_sin_size = sizeof(c_sin);
  if ((c_sock = accept(serv->network.sock_endpoint, (SOCKADDR *)&c_sin, &c_sin_size)) < 0)
  {
    perror("accept()");
    return ;
  }
  printf(KGRN "New connection\n" KRESET);
  server->list_clients[server->nb_clients].sock = c_sock;
  new_connection_communication(&(server->list_clients[server->nb_clients]));
  server->nb_clients++;
  server->last_sock = c_sock > server->last_sock ? c_sock : server->last_sock;
}

void disconnect_client(t_server_data *server, int i)
{
  printf(KRED "client disconnected\n" KRESET);
  close(server->list_clients[i].sock);
  memmove(server->list_clients + i, server->list_clients + i + 1, (server->nb_clients - i - 1) * sizeof(t_client));
  server->nb_clients--;
}

void close_all_connections(t_server_data *server)
{
  int i;

  i = -1;
  while (++i < server->nb_clients)
    close(server->list_clients[i].sock);
  close(serv->network.sock_endpoint);
  printf(KGRN "Server disconnected\n" KRESET);
}

//     DOING 
void	create_client(t_serveur *serv, SOCKET sock)
{
	t_client_entity	*new_client;

	new_client = (t_client_entity *)malloc(sizeof(t_client_entity));
	new_client->sock = sock;
	new_client->list_pending_cmds = NULL
}
