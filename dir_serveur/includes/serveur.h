#ifndef SERVEUR_H
# define SERVEUR_H

# include <stdlib.h>
# include <regex.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/*
**	color in text;
*/
# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"
# define KRESET "\x1B[0m"

# define PORT 4242
# define MAX_CLIENT 10
# define BUFF_SIZE 10

typedef int                 SOCKET;
typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct sockaddr     SOCKADDR;
typedef struct in_addr      IN_ADDR;

/*
** ************************ Network **************************
*/
typedef struct 				s_network_data
{
	int						port;
	SOCKET					sock_endpoint;
	SOCKET					sock_max;
}							t_network_data;

/*
** ************************ Team *****************************
*/
typedef struct 				s_team_entity
{
	char					*name;
	int						available_slots;
}							t_team_entity;

typedef struct 				s_team_hdl
{
	int						nb_teams;
	t_team_entity			*array_teams;
}							t_team_hdl;

/*
** ************************ Client **************************
*/
typedef struct 				s_client_entity
{
	SOCKET					sock;
	t_team_entity			*team;
	char					buff[BUFF_SIZE];
	int						start_buff;
	int						len_buff;
	int						nb_pending_cmds;
	t_list_cmds_entity		*list_pending_cmds;
}							t_client_entity;

typedef struct				s_list_clients_entity
{
	t_client_entity			*client;
	t_list_clients_entity	*next;
}							t_list_clients_entity;

typedef struct 				s_client_hdl
{
	int						nb_clients;
	t_list_clients_entity	*list_clients;
}							t_client_hdl;

/*
** ************************ Cmds *****************************
*/
typedef struct 				s_cmd_entity
{
	char					*name;
	int						available_slots;
}							t_cmd_entity;

typedef struct 				s_list_cmds_entity
{
	t_cmd_entity			*cmd;
	t_list_cmds_entity		*next
}							t_list_cmds_entity;

typedef struct 				s_cmd_hdl
{
	int						nb_cmds;
	t_client_entity			*array_cmds;
}							t_cmd_hdl;

/*
** ******************** Serveur Main Struct ******************
*/
typedef struct				s_serveur
{
	t_network_data 			network;
	t_team_hdl				team_hdl;
	t_client_hdl			client_hdl;
	t_cmd_hdl				cmd_hdl;
	t_world_hdl				world_hdl;
}							t_serveur;

//***********************OLD TOUT POURRRS**********************************************

// recupéré dans input_handling.c
typedef struct				s_serv_settings
{
	int						port;
	int						map_width;
	int						map_height;
	int						nb_of_teams;
	char					**team_names;
	int						nb_of_clients;
	int						t;
}							t_serv_settings;

typedef struct				s_client
{
  SOCKET					sock;
  char						team[BUFF_SIZE];
  char						buff[BUFF_SIZE];
  int						start_buff;
  int						len_buff;
}							t_client;

typedef struct				s_server_data
{
  SOCKET					sock_endpoint;
  int						last_sock;
  t_client					list_clients[MAX_CLIENT];
  int						nb_clients;
  t_serv_settings			serv_settings;
}							t_server_data;

/*
** input_handler.c
*/
void					              get_input(t_serv_settings *serv_settings, int argc, char **argv);
void					              check_input_format(t_serv_settings *serv_settings, int argc, char **argv);
void					              parse_input(int argc, char **argv);
void					              fill_input(t_serv_settings *serv_settings, int argc, char **argv);
void					              error_in_args(int pos, char *str);
int						              regex_match(char *string_to_search, char *regex_str);

/*
** tools.c
*/
void                        exit_error(char *error_log);

/*
** connection.c
*/
void                        init_server(t_server_data *server);
void                        new_client_connection(t_server_data *server);
void                        disconnect_client(t_server_data *server, int i);
void                        close_all_connections(t_server_data *server);

/*
** communication.c
*/
void                        ckeck_all_clients_communication(t_server_data *server, fd_set *read_fs);
void                        new_connection_communication(t_client *client);
int                         read_client(t_client *client);

/*
** cmd_clients_manager.c
*/
void                        manage_cmd_clients(t_server_data *server);
void                        exec_cmd_client(t_client *client);

#endif
