#ifndef SERVEUR_H
# define SERVEUR_H

# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define PORT 4242
# define MAX_CLIENT 10
# define BUFF_SIZE 1024

typedef int                 SOCKET;
typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct sockaddr     SOCKADDR;
typedef struct in_addr      IN_ADDR;

typedef struct              s_client
{
  SOCKET                    sock;
  char                      team[BUFF_SIZE];
  char                      buff[BUFF_SIZE];
}                           t_client;

typedef struct              s_server_data
{
  SOCKET                    sock_endpoint;
  int                       last_sock;
  t_client                  list_clients[MAX_CLIENT];
  int                       nb_clients;
}                           t_server_data;

void                        exit_error(char *error_log);
void                        init_server(t_server_data *server);
void                        new_client_connection(t_server_data *server);
void                        ckeck_all_clients_communication(t_server_data *server, fd_set *read_fs);
int                         read_client(SOCKET c_sock);
void                        disconnect_client(t_server_data *server, int i);
void                        close_connections(t_server_data *server);
void                        new_connection_communication(t_client *client);

#endif
