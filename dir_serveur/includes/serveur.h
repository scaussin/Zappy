/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 12:35:27 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 12:35:28 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <arpa/inet.h>
# include <syslog.h>
# include <inttypes.h>

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

/*
**	Boolean defines
*/

# define B_TRUE 1
# define B_FALSE 0

/*
**	Serveur communication defines.
*/

# define MAX_CLIENTS_CONNECTED 100

# define BUFF_SIZE 8192

# define SIZE_CMD_MATCH_TABLE 12
# define SIZE_GFX_CMD_MATCH_TABLE 9
# define MAX_LIST_CMD 10
# define END "\n"
# define CHAR_END '\n'
# define LEN_END 1
# define MSG_ARG_MAX_SIZE 10

/*
**	Game defines
*/

# define MAX_GAME_TIME_UNIT 1000
# define MAX_MAP_AREA 10000
# define MAX_NB_SLOT 20
# define MAX_NB_OF_TEAM 10
# define MAX_NB_OF_CLIENTS_PER_TEAM 20

# define NB_RESSOURCES 7

# define FOOD_LIFE_TIME 126
# define EGG_HATCH_TIME 600

/*
**	cmd time defines
*/

# define MOVE_CMDS_TIME 7
# define VOIR_CMD_TIME 7
# define INVENTAIRE_CMD_TIME 1
# define ITEM_CMD_TIME 7
# define EXPULSE_CMD_TIME 7
# define BROADCAST_CMD_TIME 7
# define INCANTATION_CMD_TIME 300
# define FORK_CMD_TIME 42
# define CONNECT_NBR_CMD_TIME 0

/*
**	victory conditions
*/

# define MAX_LV 8
# define VICTORY_CDT_PLAYER_NB 6
# define VICTORY_CDT_PLAYER_LV 8

# include "serveur_types.h"
# include "serveur_other_struct.h"
# include "serveur_struct.h"

/*
** serveur_data.c
*/

void									init_data(t_serveur *serv);
void									init_serveur(t_serveur *serv);
void									fill_team_info(t_serveur *serv);
void									clear_serv(t_serveur *serv,
											t_socket sock);

/*
** tools.c
*/

void									exit_error(char *error_log);
void									*s_malloc(size_t size);
void									replace_nl(char *str, int len);
void									logs(int type, char *log);
int										get_len_cmd(char *str);
char									*get_cmd_trim(char *str);
int										modulo(int a, int b);

void									print_buff(t_buffer buff);
void									print_send(int sock, char *str,
											int len);
void									print_send_gfx(char *str);
void									print_receive(int sock, char *str,
											int len);
char									*str_concat_realloc1(char *str1,
											char *str2);

int										is_equal(double x, double y);

/*
** input_handler.c
*/

void									get_input(t_serveur *serv, int argc,
											char **argv);
void									check_input_format(t_serveur *serv,
											int argc, char **argv);
void									get_input_team_names_format(
										t_serveur *serv, int *i, char **argv);
void									get_input_team_names_format_2(
										t_serveur *serv, int *i, char **argv);
void									parse_filled_input(t_serveur *serv);
void									fill_input(t_serveur *serv, int argc,
											char **argv);
void									fill_input_part_2(t_serveur *serv,
											int *i, char **argv, int team_i);
void									error_in_args(int pos, char *str);
void									print_input_values(t_serveur *serv);
int										regex_match(char *string_to_search,
											char *regex_str);

/*
** terrain_generation.c
*/

void									init_terrain(t_serveur *serv);
void									generate_ressources_name(
											t_world_hdl *world_hdl);
void									allocate_world_board(
											t_world_hdl *world_hdl);
void									set_world_board_cases(
											t_world_hdl *world_hdl);
void									generate_ressources(
											t_serveur *serv,
											t_world_hdl *world_hdl);
void									generate_ressources_flat(
											t_world_hdl *world_hdl,
											int x, int y);
void									generate_ressources_diced(
											t_world_hdl *world_hdl,
											int x, int y);
void									print_world_board(
											t_world_hdl *world_hdl,
											int x, int y);

/*
** serveur_loop.c
*/

void									init_fd(t_serveur *serv);
void									main_loop(t_serveur *serv);
void									manage_clients_input(t_serveur *serv);
int										check_fd(t_serveur *serv);

/*
** connection.c
*/

t_socket								accept_connection(t_serveur *serv);
void									new_client_connection(
											t_serveur *serveur);
void									disconnect_client(
											t_client_entity *client);
void									close_all_connections(t_serveur *serv);

/*
** communication.c
*/

void									check_all_clients_communication(
											t_serveur *serv);
void									client_connection_lost(t_serveur *serv,
											t_client_entity *p_client);
void									write_client(t_client_entity *client);
int										read_client(t_client_entity *client);
int										write_buffer(t_buffer *buff,
											char *to_write, int size);
char									*read_buffer(t_buffer *buff);
void									clean_all_buffer(t_buffer *buff);
void									update_len_buffer(t_buffer *buff,
											int size);
void									update_buffer_overflow(t_buffer *buff,
											int size);
char									*get_first_cmd(t_buffer *buffer);
void									disconnect_flagged_clients(
											t_serveur *serv);
int										check_size_read(
											t_client_entity *client);

t_team_entity							*get_team(t_serveur *serv, char *buff);
t_team_entity							*new_client_communication(
											t_serveur *serv,
											t_client_entity *client);

/*
** communication_gfx.c
*/

void									push_gfx_msg(t_serveur *serv,
											char *msg);
void									send_current_world_state(
											t_serveur *serv,
											t_client_entity *gfx_client);
void									send_spawned_eggs(t_serveur *serv);
void									exec_gfx_cmd(t_serveur *serv,
											t_client_entity *gfx_client,
											char *cmd,
											int nb_of_parsed_chars);

/*
** client_hdl.c
*/

t_client_entity							*create_client(t_socket sock);
void									set_client_player_datas(
											t_client_entity *new_client);
void									add_client(t_serveur *serv,
											t_client_entity *client);
void									remove_client(t_serveur *serv,
											t_client_entity *client);
void									remove_team_slot(t_serveur *serv,
											t_client_entity *client);

/*
**	client_authentification.c
*/

void									client_authentification(
											t_serveur *serv,
											t_client_entity *client);
void									client_authenticate_gfx(
											t_serveur *serv,
											t_client_entity *client);
void									client_authenticate_player(
											t_serveur *serv,
											t_client_entity *client,
											char *cmd);
void									client_authenticate_slots_available(
											t_serveur *serv,
											t_client_entity *client,
											t_team_entity	*team);
void									assign_player_to_egg(
											t_serveur *serv,
											t_client_entity *client,
											t_egg *egg);
void									client_authenticate_send_gfx_msg(
											t_serveur *serv,
											t_client_entity *client);

/*
**	Player game datas handling
*/

void									assign_random_player_position(
											t_serveur *serv,
											t_player *player);
void									assign_player_time_of_dinner(
											t_serveur *serv,
											t_player *player);
long									get_food_as_time(t_serveur *serv,
											t_client_entity *client);

/*
** team_hdl.c
*/

t_team_entity							*get_team_by_name(
											t_serveur *serv, char *name);

/*
** game_events.c && game_events_helper.c
*/

void									check_game_events(
											t_serveur *serv);
void									check_world_events(
											t_serveur *serv);
void									check_players_events(
											t_serveur *serv);
void									check_eggs(t_serveur *serv);
void									check_egg_eclosion(t_serveur *serv,
											t_egg *tmp);
void									set_hatched_egg_datas(t_serveur *serv,
											t_egg *egg_tmp);
void									check_egg_death(t_serveur *serv,
											t_egg *tmp);
void									check_player_life(t_serveur *serv,
											t_client_entity *cur_client);
void									refresh_player_dinner_time(
											t_serveur *serv,
											t_client_entity *client,
											float old_t_unit);

void									check_victory(t_serveur *serv);
void									refresh_times(t_serveur *serv,
											float old_t_unit);

/*
** cmd_clients_manager.c
*/

void									lex_and_parse_cmds(
											t_client_entity *client,
											t_cmd_match *cmd_match_table);
void									check_cmd_match(
											t_cmd_match *cmd_match_table,
											t_client_entity *client,
											char *cmd);
void									add_cmd(t_client_entity *client,
											t_cmd_match *cmd, char *param);
void									init_game_event(
											t_world_event *world_event,
											char *type_str, int trigger_delay);
void									add_new_event(t_serveur *serv,
											t_world_event *world_event);
void									add_client_to_event(
											t_world_event *world_event,
											t_client_entity *client);
int										compare_cmd(char *s1, char *s2);
void									clean_clients_first_cmd(
											t_client_entity *p_client);
void									exec_cmd_client_start(
											t_serveur *serv,
											t_client_entity *p_client);

struct timespec							*exec_cmd_client(t_serveur *serv);

/*
**	Gfx client cmds
*/

void									lex_and_parse_gfx_cmds(t_serveur *serv,
											t_client_entity *gfx_client);
void									parse_gfx_cmd(t_serveur *serv,
											t_client_entity *gfx_client,
											char *cmd);

/*
**	clock.c: tools for handling time on the server.
*/

void									get_time(struct timespec *ts);
struct timespec							timespec_diff(struct timespec *start,
											struct timespec *stop);
struct timeval							*set_timeout_select(
											struct timeval *timeout,
											struct timespec *lower_time_end);
void									add_nsec_to_timespec(
											struct timespec *time,
											long nanosec);
long									convert_timespec_to_nsec(
											struct timespec time);
int										timespec_is_over(
											struct timespec time_end);
struct timespec							*min_timespec(
											struct timespec *a,
											struct timespec *b);

/*
** src/cmds_functions/
*/

int										on_start_cmd_avance(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_avance(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_avance(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

int										on_start_cmd_droite(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_droite(struct s_serveur *serv,
											struct s_client_entity *client_cur,
											char *param);
void									on_end_cmd_droite(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

int										on_start_cmd_gauche(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_gauche(struct s_serveur *serv,
											struct s_client_entity *client_cur,
											char *param);
void									on_end_cmd_gauche(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

int										on_start_cmd_voir(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_voir(struct s_serveur *serv,
											struct s_client_entity *client_cur,
											char *param);
t_pos									*get_see_case_coordinates(
											t_serveur *serv, t_player *player);
int										get_nb_case(int level);
void									fill_tab(t_pos *abs_pos,
											t_pos *rel_pos, t_player *player,
											t_serveur *serv);
char									*init_see_str(void);
char									*get_players(char *see_str, t_pos *pos,
											int num, t_world_case **world);
char									*get_ressources(t_serveur *serv,
											int x, int y, char *see_str);
void									cmd_voir(struct s_serveur *serv,
											struct s_client_entity *client_cur,
											char *param);

int										on_start_cmd_inventaire(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_inventaire(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_inventaire(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

int										parse_ressource_index(char *param);
int										dual_strncmp(char *param, char *val1,
											char *val2, int n);
int										on_start_cmd_prend(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_prend(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_prend(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

int										try_to_take_res(t_player *client,
											int res_nb);

int										on_start_cmd_pose(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_pose(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_pose(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
int										try_to_drop_ressource(t_player *player,
											int res_nb);

int										on_start_cmd_expulse(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_expulse(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_expulse(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									expulse_client_in_dir(t_serveur *serv,
											t_client_entity *client,
											int dir);
int										expulse_cdts(
											t_client_entity *tmp_client,
											t_player *player);
void									expulsion_rule_check(t_serveur *serv,
											t_client_entity *list_clients,
											t_player *cur_player);
void									expulse_apply(t_serveur *serv,
											t_client_entity *client,
											int dir);

int										on_start_cmd_broadcast(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_broadcast(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									set_pos_varialbes(
											t_client_entity *client_cur,
											t_client_entity *list_clients,
											t_pos *source, t_pos *dest);
void									on_end_cmd_broadcast(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
double									distance(int sourcex, int sourcey,
											int i, int j);
int										orientation(int sourcex, int sourcey,
											int i, int j);
int										orientation_check_3(int i, int j,
											int sourcex, int sourcey);
int										provenance_son(t_serveur *serv,
											t_pos source, t_pos dest);
void									prov_son_haut_droite(int *orient,
											t_pos taille, t_pos source,
											t_pos dest);
void									prov_son_haut(int *orient, t_pos taille,
											t_pos source, t_pos dest);
void									prov_son_haut_gauche(int *orient,
											t_pos taille, t_pos source,
											t_pos dest);
void									prov_son_gauche(int *orient,
											t_pos taille, t_pos source,
											t_pos dest);
void									prov_son_bas_gauche(int *orient,
											t_pos taille, t_pos source,
											t_pos dest);
void									prov_son_bas(int *orient, t_pos taille,
											t_pos source, t_pos dest);
void									prov_son_bas_droite(int *orient,
											t_pos taille, t_pos source,
											t_pos dest);
void									prov_son_droite(int *orient,
											t_pos taille, t_pos source,
											t_pos dest);

double									provenance_with_dir(int provenance,
											int dir);

int										on_start_cmd_incantation(
											t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_incantation(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									end_incantation_interruptable(
											t_serveur *serv,
											t_client_entity *client_cur);
int										init_incantation(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									start_incantating(t_serveur *serv,
											t_client_entity *client_cur);
void									finish_incantation(t_serveur *serv,
											t_client_entity *cur_client,
											int result);
void									incanter_finish_incantation(
											t_serveur *serv,
											t_client_entity *cur_client,
											int result);

void									level_up_clients_player(
											t_client_entity *client);
void									send_level_messages(t_serveur *serv,
											t_client_entity *client);
int										other_players_incantation_level_up_cdts
											(t_serveur *serv,
											t_client_entity *cur_client,
											t_client_entity *clients_tmp);
void									other_players_finish_incantation(
											t_serveur *serv,
											t_client_entity *clients_tmp,
											int result);
int										*set_incantation_target_res(
											int plevel);
char									*fill_target_str_from_level(
											int plevel);
int										are_incantation_cdts_ok(
											t_serveur *serv,
											t_player *cur_player,
											int *target_res);
void									set_players_incanting(t_serveur *serv,
										t_client_entity *cur_client);
int										other_incanting_player_cdts(
											t_serveur *serv,
											t_client_entity *cur_client,
											t_client_entity *clients_tmp);
void									put_player_in_incantation(
											t_serveur *serv,
											t_client_entity *cur_client,
											t_client_entity *clients_tmp);
void									strip_case_ressources(t_serveur *serv,
											t_client_entity *client_cur,
											int *target_res);

int										on_start_cmd_fork(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_fork(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

int										on_start_cmd_connect_nbr(
											t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									cmd_connect_nbr(t_serveur *serv,
											t_client_entity *client_cur,
											char *param);
void									on_end_cmd_connect_nbr(
											t_serveur *serv,
											t_client_entity *client_cur,
											char *param);

/*
**	src/gfx_cmds_functions/
*/

void									gfx_cmd_msz(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									gfx_cmd_bct(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									get_param_values(char *param,
											int *x, int *y);
void									send_gfx_bct_msg(t_serveur *serv,
											int x, int y);
void									gfx_cmd_mct(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									send_one_case_content(
											t_serveur *serv, int x, int y);
void									gfx_cmd_tna(
											t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									gfx_cmd_ppo(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									ppo_send_msg(t_serveur *serv,
											t_client_entity *client_tmp);
void									gfx_cmd_plv(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									plv_send_msg(t_serveur *serv,
											t_client_entity *client_tmp);
void									gfx_cmd_pin(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
int										get_player_nb(char *param,
											int *client_nb,
											t_client_entity *gfx_client);
void									send_client_inv(t_serveur *serv,
											t_client_entity *client_tmp);
void									gfx_cmd_sgt(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
void									gfx_cmd_sst(t_serveur *serv,
											t_client_entity *gfx_client,
											char *param);
int										get_time_val(char *param, int *val,
											t_client_entity *gfx_client);
int										sst_check_max(int new_t,
											t_client_entity *gfx_client);

/*
**	egg_handling.c
*/

void									add_new_egg(t_serveur *serv,
											t_client_entity *client);
t_egg									*egg_available(t_serveur *serv,
											t_client_entity *client);
void									clear_egg(t_serveur *serv,
											t_egg *egg);
void									refresh_eggs_hatching_time(
											t_serveur *serv, float old_t_unit);
void									add_egg_to_list(t_serveur *serv,
											t_egg *new_egg);

/*
**	cmd_clients_init.c
*/

void									init_cmd_match_table(t_serveur *serv);
void									init_cmd_match_table_2(t_serveur *serv);
void									init_cmd_match_table_3(t_serveur *serv);
void									init_gfx_cmd_match_table(
											t_serveur *serv);
void									init_gfx_cmd_match_table_2(
											t_serveur *serv);

#endif
