#include "../includes/serveur.h"

void init_server(t_server_data *server)
{
  SOCKET      sock;
  SOCKADDR_IN sin;

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    exit_error("socket()");
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(server->serv_settings.port);
  sin.sin_family = AF_INET;
  if(bind(sock,(SOCKADDR *)&sin, sizeof sin) < 0)
    exit_error("bind()");
  if(listen(sock, MAX_CLIENT) < 0)
    exit_error("listen()");
  server->sock_endpoint = sock;
  server->last_sock = server->sock_endpoint;
  server->nb_clients = 0;
}

void new_client_connection(t_server_data *server)
{
  SOCKET      c_sock;
  SOCKADDR_IN c_sin;
  socklen_t      c_sin_size;

  c_sin_size = sizeof(c_sin);
  if ((c_sock = accept(server->sock_endpoint, (SOCKADDR *)&c_sin, &c_sin_size)) < 0)
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
  close(server->sock_endpoint);
  printf(KGRN "Server disconnected\n" KRESET);
}
