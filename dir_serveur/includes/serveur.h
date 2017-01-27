#ifndef SERVEUR_H
# define SERVEUR_H
# define _GNU_SOURCE

# include <stdlib.h>
# include <regex.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <time.h>
# include <math.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
//<sys/time.h>
# include <arpa/inet.h>
# include <syslog.h>

#include <inttypes.h>

# ifdef __MACH__
#  include <mach/clock.h>
#  include <mach/mach.h>
# endif

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


# define BUFF_SIZE 4096

# define SIZE_CMD_MATCH_TABLE 12 // The number of client available cmds.
# define MAX_LIST_CMD 10
# define END "\n"
# define CHAR_END '\n'
# define LEN_END 1
# define NB_RESSOURCES 7

/*
**	Game defines
*/

# define FOOD_LIFE_TIME 126
# define INCANTATION_TIME 300
# define EGG_HATCH_TIME 600
# define EGG_LAYING_TIME 42

# define MAX_LV 8
# define VICTORY_CDT_PLAYER_NB 6
# define VICTORY_CDT_PLAYER_LV 8

/*
**	Server structures.
*/

typedef int                 SOCKET;
typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct sockaddr     SOCKADDR;
typedef struct in_addr      IN_ADDR;

typedef struct s_list_cmds_entity		t_list_cmds_entity;

typedef enum				e_dir
{
	UP,
	RIGHT,
	DOWN,
	LEFT
}							t_dir;

typedef enum				e_ressources
{
	FOOD,
	LINEMATE,
	DERAUMERE,
	SIBUR,
	MENDIANE,
	PHIRAS,
	THYSTAME
}							t_ressources;

/*
**	Used for team level status. look at t_team_entity.
*/

typedef enum				e_level
{
	LV1,
	LV2,
	LV3,
	LV4,
	LV5,
	LV6,
	LV7,
	LV8
}							t_level;

typedef struct				s_pos
{
	int						x;
	int						y;
}							t_pos;

typedef struct s_serveur			t_serveur;
typedef struct s_client_entity		t_client_entity;
typedef struct s_world_case			t_world_case;

/*
** ************************ Network **************************
*/
typedef struct 				s_network_data
{
	int						port;
	SOCKET					sock_serveur;
	SOCKET					sock_max;
	fd_set					*read_fs;
	fd_set					*write_fs;
	int						max_clients;
}							t_network_data;

/*
** ************************ Team *****************************
*/
typedef struct 				s_team_entity
{
	char					*name;
	int						available_slots;
	int						nb_players_per_lv[MAX_LV];
}							t_team_entity;

typedef struct 				s_team_hdl
{
	int						nb_teams;
	int						nb_teams_slots;
	t_team_entity			*array_teams;
}							t_team_hdl;

/*
** ************************ Client **************************
*/

// Player belonging to the client.
typedef struct				s_player
{
	int						level;
	t_pos					pos;
	t_dir					dir;
	int						nb_see_case;
	t_world_case			*cur_case;
	int						inventory[NB_RESSOURCES];
	struct timespec			next_dinner_time;

	int						is_incanting;
	int						is_incanter;
	struct timespec			incantation_end_time;

	int						is_laying_egg;
	struct timespec			egg_layed_time;
}							t_player;


typedef struct 				s_list_cmds_entity
{
	void					(*func)(struct s_serveur *serv, struct s_client_entity *client_cur, char *param);
	char					*param;
	int						duration_cmd;
	struct timespec			time_end;
	t_list_cmds_entity		*next;
}							t_list_cmds_entity;

typedef struct				s_buffer
{
	char					buff[BUFF_SIZE];
	int						start;
	int						len;
}							t_buffer;

typedef struct 				s_client_entity
{
	// Status flags
	int						is_in_game;
	int						is_player_dead;
	int						is_gfx;
	int						is_disconnecting;

	// Client program variables
	SOCKET					sock;
	t_team_entity			*team;

	t_buffer				buff_recv;
	t_buffer				buff_send;
	int						size_list_cmds;
	t_list_cmds_entity		*list_cmds;
	struct timespec			delay_time;

	// Client Player variables.
	t_player				player;

	// link to next
	struct s_client_entity	*next;
}							t_client_entity;

typedef struct 				s_client_hdl
{
	int						nb_clients;
	t_client_entity			*list_clients;
	t_client_entity			*gfx_client;
}							t_client_hdl;

/*
** ************************ Cmds *****************************
*/

typedef struct 				s_cmd_match
{
	char					*name;
	void					(*func)(struct s_serveur *serv, struct s_client_entity *client_cur, char *param);
	int						duration_cmd;
}							t_cmd_match;

typedef struct 				s_cmd_hdl
{
	int						nb_cmds;
	t_cmd_match				*cmd_match_table; // array of commands and their linked function.
}							t_cmd_hdl;

/*
** ************************ World struct ***************************** => Game board
*/
typedef	struct				s_event_client
{
	t_client_entity			*client;
	struct s_event_client	*next;
}							t_event_client;

typedef struct				s_egg
{
	int						egg_nb;
	t_team_entity			*team;
	t_pos					pos;

	struct timespec			hatch_time;
	int						has_hatched;
	struct timespec			death_time;

	struct s_egg			*next;
}							t_egg;

typedef struct				s_world_event
{
	char					*type;
	int 					trigger_delay;
	struct timespec			time;
	t_event_client			*concerned_clients;

	struct s_world_event	*next;
}							t_world_event;

typedef struct				s_world_case
{
	// A case is associated to a player.cur_case pointer.
	int						ressources[7];
	int						nb_players;
}							t_world_case;

typedef struct 				s_world_hdl
{
	int						map_x;
	int						map_y;
	double					t_unit;
	char					*name_ressources[NB_RESSOURCES];
	t_world_case			**world_board; // ==> access with	world_board[y_pos][x_pos]

	t_world_event			*ongoing_events;

	int						nb_of_eggs;
	t_egg					*eggs;
}							t_world_hdl;

/*
** ******************** Serveur game settings ******************
**	Could be useful for in-game rule change.
*/

typedef struct				s_game_settings
{
//	int						are_teams_infinite; // does not seem possible for now.
}							t_game_settings;

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

	t_game_settings			settings_hdl;
}							t_serveur;

/*
** serveur_data.c
*/
void						init_data(t_serveur *serv);
void						init_serveur(t_serveur *serv);
void						fill_team_info(t_serveur *serv);

/*
** tools.c
*/
void						exit_error(char *error_log);
void						*s_malloc(size_t size);
void						replace_nl(char * str, int len);
void						logs(int type, char *log);
int							get_len_cmd(char *str);
char						*get_cmd_trim(char *str);
int							modulo(int a, int b);

void						print_buff(t_buffer buff);
void						print_send(int sock, char *str, int len);
void						print_send_gfx(char *str);
void						print_receive(int sock, char *str, int len);
char						*str_concat_realloc1(char *str1, char *str2);

int							is_equal(double x, double y);

/*
** input_handler.c
*/
void						get_input(t_serveur *serv, int argc, char **argv);
void						check_input_format(t_serveur *serv, int argc, char **argv);
void						parse_input(int argc, char **argv);
void						fill_input(t_serveur *serv, int argc, char **argv);
void						error_in_args(int pos, char *str);
int							regex_match(char *string_to_search, char *regex_str);

/*
** terrain_generation.c
*/
void						init_terrain(t_serveur *serv);
void						generate_ressources_name(t_world_hdl *world_hdl);
void						allocate_world_board(t_world_hdl *world_hdl);
void						set_world_board_cases(t_world_hdl *world_hdl);
void						generate_ressources(t_world_hdl *world_hdl);
void						generate_ressources_flat(t_world_hdl *world_hdl, int x, int y);
void						generate_ressources_diced(t_world_hdl *world_hdl, int x, int y);
void						print_world_board(t_world_hdl *world_hdl);

/*
** serveur_loop.c
*/
void						init_fd(t_serveur *serv);
void						main_loop(t_serveur *serv);
void						manage_clients_input(t_serveur *serv);

/*
** connection.c
*/
SOCKET						accept_connection(t_serveur *serv);
void						new_client_connection(t_serveur *serveur);
void						disconnect_client(SOCKET c_sock);
void						close_all_connections(t_serveur *serv);

/*
** communication.c
*/
void						check_all_clients_communication(t_serveur *serv);
void						disconnect_flagged_clients(t_serveur *serv);
int							read_client(t_client_entity *client);
t_team_entity				*get_team(t_serveur *serv, char *buff);
t_team_entity				*new_client_communication(t_serveur * serv, t_client_entity *client);
int							write_buffer(t_buffer *buff, char *to_write, int size);
char						*read_buffer(t_buffer *buff);
void						write_client(t_client_entity *client);
char						*get_first_cmd(t_buffer *buffer);

/*
** communication_gfx.c
*/

void						push_gfx_msg(t_serveur *serv, char *msg);
void						send_current_world_state(t_serveur *serv, t_client_entity *gfx_client);

/*
** client_hdl.c
*/
t_client_entity				*create_client(SOCKET sock);
void						set_client_player_datas(t_client_entity *new_client);
void						add_client(t_serveur *serv, t_client_entity *client);
void						remove_client(t_serveur *serv, t_client_entity *client);



/*
**	client_authentification.c
*/

void						client_authentification(t_serveur *serv, t_client_entity *client);
void						client_authenticate_gfx(t_serveur *serv, t_client_entity *client);
void						client_authenticate_player(t_serveur *serv, t_client_entity *client, char *cmd);

/*
**	Player game datas handling
*/

void						assign_random_player_position(t_serveur *serv, t_player *player);
void						assign_player_time_of_dinner(t_serveur *serv, t_player *player);

/*
** team_hdl.c
*/

t_team_entity				*get_team_by_name(t_serveur *serv, char *name);

/*
** game_events.c && game_events_helper.c
*/

void						check_game_events(t_serveur *serv);
void							check_world_events(t_serveur *serv);
void							check_players_events(t_serveur *serv);
void							check_eggs(t_serveur *serv);
void								check_player_life(t_serveur *serv, t_client_entity *cur_client);
void								check_player_incantation_end(t_serveur	*serv, t_client_entity	*cur_client);
void								check_player_laying_egg_end(t_serveur	*serv, t_client_entity	*cur_client);
/*
** cmd_clients_manager.c
*/

void						init_cmd_match_table(t_serveur *serv); // init command dictionnary.

void						lex_and_parse_cmds(t_client_entity *client, t_cmd_match *cmd_match_table);
void						check_cmd_match(t_cmd_match *cmd_match_table, t_client_entity *client, char *cmd);
void						add_cmd(t_client_entity *client, t_cmd_match *cmd, char *param);

void						init_game_event(t_world_event *world_event, char *type_str, int trigger_delay);
void						add_new_event(t_serveur *serv, t_world_event *world_event);
void						add_client_to_event(t_world_event *world_event, t_client_entity *client);
void						delete_game_event(t_serveur *serv, t_world_event *target_event);

// client command execution.
struct timespec				*exec_cmd_client(t_serveur *serv);
int							timespec_is_over(struct timespec time_end);
struct timespec				*min_timespec(struct timespec *a, struct timespec *b);

/*
**	clock.c
*/

void						get_time(struct timespec *ts);
struct timespec				timespec_diff(struct timespec *start, struct timespec *stop);
struct timeval				*set_timeout_select(struct timeval *timeout, struct timespec *lower_time_end);
void						add_nsec_to_timespec(struct timespec *time, long nanosec);

/*
** src/cmds_functions/
*/

void						cmd_avance(t_serveur *serv, t_client_entity *client_cur, char *param);
void						cmd_droite(struct s_serveur *serv, struct s_client_entity *client_cur, char *param);
void						cmd_gauche(struct s_serveur *serv, struct s_client_entity *client_cur, char *param);
void						cmd_voir(struct s_serveur *serv, struct s_client_entity *client_cur, char *param);
t_pos						*get_see_case_coordinates(t_serveur *serv, t_player *player);
int								get_nb_case(int level);
void							fill_tab(t_pos *abs_pos, t_pos *rel_pos, t_player *player, t_serveur *serv);
void						cmd_inventaire(t_serveur *serv, t_client_entity *client_cur, char *param);
int							parse_ressource_index(char *param);
void						cmd_prend(t_serveur *serv, t_client_entity *client_cur, char *param);
int								try_to_take_res(t_player *client, int res_nb);
void						cmd_pose(t_serveur *serv, t_client_entity *client_cur, char *param);
int								try_to_drop_ressource(t_player *player, int res_nb);
void						cmd_expulse(t_serveur *serv, t_client_entity *client_cur, char *param);
void							expulse_client_in_dir(t_serveur *serv, t_client_entity *client, int dir);
void						cmd_broadcast(t_serveur *serv, t_client_entity *client_cur, char *param);
void						cmd_incantation(t_serveur *serv, t_client_entity *client_cur, char *param);
int								*set_incantation_target_res(int plevel);
int								are_incantation_cdts_ok(t_serveur *serv, t_player *cur_player, int *target_res);
void							set_players_incanting(t_serveur *serv, t_client_entity *cur_client);
void							strip_case_ressources(t_serveur *serv, t_client_entity *client_cur, int *target_res);
void						cmd_fork(t_serveur *serv, t_client_entity *client_cur, char *param);
void						cmd_connect_nbr(t_serveur *serv, t_client_entity *client_cur, char *param);

/*
**	egg_handling.c
*/

void	add_new_egg(t_serveur *serv, t_client_entity *client);
t_egg	*egg_available(t_serveur *serv, t_client_entity *client);
void	clear_egg(t_serveur *serv, t_egg *egg);


#endif
