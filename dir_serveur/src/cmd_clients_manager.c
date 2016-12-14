#include "../includes/serveur.h"

void manage_cmd_clients(t_server_data *server)
{
  int       i;
  t_client  *cur_client;

  i = -1;
  while (++i < server->nb_clients)
  {
    cur_client = &(server->list_clients[i]);
    if (cur_client->len_buff > 0)
      exec_cmd_client(cur_client);
  }
}

void exec_cmd_client(t_client *client)
{
  int   i_loop;
  int   i_buff;
  char  to_process[BUFF_SIZE + 1];

  memset(to_process, 0, BUFF_SIZE);
  i_loop = 0;
  i_buff = client->start_buff;
  while (i_loop < client->len_buff)
  {
    to_process[i_loop] = client->buff[i_buff % BUFF_SIZE];
    i_loop++;
    i_buff++;
  }
  client->start_buff = (i_buff % BUFF_SIZE);
  client->len_buff = 0;
  printf("cmd read : %s\n", to_process);
  printf("*state buffer :\n");
  write(1, client->buff, BUFF_SIZE);
  printf("*\n");
}
