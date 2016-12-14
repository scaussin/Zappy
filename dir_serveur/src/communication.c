#include "../includes/serveur.h"

void ckeck_all_clients_communication(t_server_data *server, fd_set *read_fs)
{
  int i;
  int ret_read;

  i = -1;
  while (++i < server->nb_clients)
  {
    if (FD_ISSET(server->list_clients[i].sock, read_fs)
      && !(ret_read = read_client(&(server->list_clients[i]))))
    {
      disconnect_client(server, i);
      return ;
    }
  }
}

int   read_client(t_client *client)
{
  int   ret;
  int   i_loop;
  int   i_buff;
  char  buffer[BUFF_SIZE + 1];

  ret = 0;
  memset(buffer, 0, BUFF_SIZE + 1);
  if ((ret = recv(client->sock, buffer, BUFF_SIZE, 0)) < 0)
  {
    perror("recv()");
    ret = 0;
  }
  else if (!ret)
    return (0);

  buffer[ret] = '\0';
  printf("Recu : %s\n", buffer);
  
  i_loop = 0;
  i_buff = client->start_buff;
  while (i_loop < ret)
  {
    client->buff[i_buff % BUFF_SIZE] = buffer[i_loop];
    i_loop++;
    i_buff++;
  }
  client->len_buff += ret;
  if (client->len_buff > BUFF_SIZE)
    client->len_buff = BUFF_SIZE;

  return(ret);
}

void new_connection_communication(t_client *client)
{
  //char  buff_send[BUFF_SIZE];
  char  buff_recv[BUFF_SIZE];
  int   ret;

  /*memset(buff_send, 0, BUFF_SIZE);
  strcpy(buff_send, "BIENVENUE\n");*/
  if (send(client->sock, "BIENVENUE\n", strlen("BIENVENUE\n"), 0) < 0)
    exit_error("send()");
  printf("Send : BIENVENUE\n");

  if ((ret = recv(client->sock, buff_recv, BUFF_SIZE - 1, 0)) < 0)
    exit_error("recv()");
  buff_recv[ret] = '\0';
  strcpy(client->team, buff_recv);
  printf("Recu : %s\n", buff_recv);

  /*memset(buff_send, 0, BUFF_SIZE);
  strcpy(buff_send, "10\n");*/
  if (send(client->sock, "10\n", strlen("10\n"), 0) < 0)
    exit_error("send()");
  printf("Send : 10\n");

  /*memset(buff_send, 0, BUFF_SIZE);
  strcpy(buff_send, "20 20\n");*/
  if (send(client->sock, "20 20\n", strlen("20 20\n"), 0) < 0)
    exit_error("send()");
  printf("Send : 20 20\n");
}
