#include "../includes/serveur.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  t_server_data server;
  fd_set        read_fs;

  // Todo : parse arg and fill server data

  // Init server
  init_server(&server);

  // Boucle du jeux
  while (42)
  {
    int i;

    // Clear le set de fd ready pour lecture
    FD_ZERO(&read_fs);
    // Surveille l'entrée standard pour exit propre quand enter presse
    FD_SET(STDIN_FILENO, &read_fs);
    // Surveille le endpoint pour new connection
    FD_SET(server.sock_endpoint, &read_fs);
    // Surveille tous les client
    i = -1;
    while (++i < server.nb_clients)
      FD_SET(server.list_clients[i].sock, &read_fs);

    // BOUYAKA SELECT IS HERE !!!!!!!!!!
    if(select(server.last_sock + 1, &read_fs, NULL, NULL, NULL) < 0)
      exit_error("select()");

    // Exit when enter presse
    if (FD_ISSET(STDIN_FILENO, &read_fs))
      break;
    // Connect new client
    else if (FD_ISSET(server.sock_endpoint, &read_fs))
      new_client_connection(&server);
    // Check commands from clients
    else
      ckeck_all_clients_communication(&server, &read_fs);
  }

  // Todo : exec les commandes clients

  // Close les socket clients
  close_connections(&server);
  return (0);
}

void exit_error(char *error_log)
{
  perror(error_log);
  exit(errno);
}

void init_server(t_server_data *server)
{
  SOCKET      sock;
  SOCKADDR_IN sin;

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    exit_error("socket()");

  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(PORT);
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
  printf("New connection\n");
  server->list_clients[server->nb_clients].sock = c_sock;
  new_connection_communication(&(server->list_clients[server->nb_clients]));
  server->nb_clients++;
  server->last_sock = c_sock > server->last_sock ? c_sock : server->last_sock;
}

void ckeck_all_clients_communication(t_server_data *server, fd_set *read_fs)
{
  int i;
  int ret_read;

  // printf("start check_all_client\n");
  i = -1;
  while (++i < server->nb_clients)
  {
    if (FD_ISSET(server->list_clients[i].sock, read_fs))
    {
      ret_read = read_client(server->list_clients[i].sock);
      if (ret_read == 0)
      {
        disconnect_client(server, i);
        return ;
      }
    }
  }
}

int   read_client(SOCKET c_sock)
{
  char  buffer[BUFF_SIZE];
  int   ret;

  // printf("start read client\n");
  ret = 0;
  if ((ret = recv(c_sock, buffer, BUFF_SIZE - 1, 0)) < 0)
  {
    perror("recv()");
    ret = 0;
  }
  buffer[ret] = '\0';
  printf("%s\n", buffer);
  return(ret);
}

void disconnect_client(t_server_data *server, int i)
{
  // printf("start disconnected\n");
  close(server->list_clients[i].sock);
  memmove(server->list_clients + i, server->list_clients + i + 1, (server->nb_clients - i - 1) * sizeof(t_client));
  server->nb_clients--;
  printf("client disconnected\n");
}

void close_connections(t_server_data *server)
{
  int i;

  i = -1;
  while (++i < server->nb_clients)
    close(server->list_clients[i].sock);
  close(server->sock_endpoint);
}

void new_connection_communication(t_client *client)
{
  char  buff_send[BUFF_SIZE];
  char  buff_recv[BUFF_SIZE];
  int   ret;

  strcpy(buff_send, "BIENVENUE\n");
  if (send(client->sock, buff_send, strlen(buff_send), 0) < 0)
    exit_error("send()");
  if ((ret = recv(client->sock, buff_recv, BUFF_SIZE - 1, 0)) < 0)
    exit_error("recv()");
  buff_recv[ret] = '\0';
  strcpy(client->team, buff_recv);
  printf("%s\n", client->team);
}
