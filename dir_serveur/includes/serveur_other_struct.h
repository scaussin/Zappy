/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_other_struct.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 17:11:24 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 17:11:26 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_OTHER_STRUCT_H
# define SERVEUR_OTHER_STRUCT_H

/*
**	Server structures.
*/

/*
** ***************** Serveur game settings ********************	**
*/

typedef struct							s_game_settings
{
	int									is_pickup_instant;
	int									can_interrupt_incantation;
	int									are_teams_growing;
}										t_game_settings;

/*
** ************* Game enums and helpers structs ***************	**
*/

typedef enum							e_dir
{
	UP,
	RIGHT,
	DOWN,
	LEFT
}										t_dir;

typedef enum							e_ressources
{
	FOOD,
	LINEMATE,
	DERAUMERE,
	SIBUR,
	MENDIANE,
	PHIRAS,
	THYSTAME
}										t_ressources;

typedef struct							s_pos
{
	int									x;
	int									y;
}										t_pos;

/*
** ************************ Network **************************
*/

typedef struct							s_network_data
{
	int									port;
	t_socket							sock_serveur;
	t_socket							sock_max;
	fd_set								*read_fs;
	fd_set								*write_fs;
	int									max_clients;
}										t_network_data;

/*
** ************************ Team *****************************
*/

typedef struct							s_team_entity
{
	char								*name;
	int									available_slots;
	int									nb_players_per_lv[MAX_LV];
}										t_team_entity;

typedef struct							s_team_hdl
{
	int									nb_teams;
	int									nb_teams_slots;
	t_team_entity						*array_teams;
}										t_team_hdl;

typedef struct							s_world_case
{
	int									ressources[7];
	int									nb_players;
}										t_world_case;

/*
** ******************* Client's Player ***********************
** player struct is inside client_entity.
*/

typedef struct							s_player
{
	int									level;
	t_pos								pos;
	t_dir								dir;
	int									nb_see_case;
	t_world_case						*cur_case;
	int									inventory[NB_RESSOURCES];
	struct timespec						next_dinner_time;

	int									is_incanting;
	int									is_incanter;
	int									incantation_id;
}										t_player;

/*
** ************************ Client **************************
*/

typedef struct							s_list_cmds_entity
{
	int									duration_cmd;
	int									(*on_start)(t_serveur *serv,
						struct s_client_entity *client_cur, char *param);
	int									cmd_started;
	int									success;
	struct timespec						end_time;
	void								(*on_end)(t_serveur *serv,
						struct s_client_entity *client_cur, char *param);
	char								*param;

	struct s_list_cmds_entity			*next;
}										t_list_cmds_entity;

typedef struct							s_buffer
{
	char								buff[BUFF_SIZE];
	int									start;
	int									len;
	char								*overflow;
	int									len_overflow;
}										t_buffer;

/*
** -----------------------	Client container struct.
*/

typedef struct							s_client_hdl
{
	int									nb_clients;
	t_client_entity						*list_clients;
	t_client_entity						*gfx_client;
}										t_client_hdl;

/*
** ************************ Cmds *****************************
*/

typedef struct							s_cmd_match
{
	char								*name;
	int									duration_cmd;
	int									(*on_start)(t_serveur *serv,
											struct s_client_entity *client_cur,
											char *param);
	void								(*on_end)(t_serveur *serv,
											struct s_client_entity *client_cur,
											char *param);
}										t_cmd_match;

typedef struct							s_cmd_hdl
{
	int									nb_cmds;
	t_cmd_match							*cmd_match_table;
	t_cmd_match							*gfx_cmd_match_table;
}										t_cmd_hdl;

/*
** ************************ World struct *****************************
*/

typedef	struct							s_event_client
{
	t_client_entity						*client;
	struct s_event_client				*next;
}										t_event_client;

typedef struct							s_egg
{
	int									egg_nb;
	int									father_nb;
	t_team_entity						*team;
	t_pos								pos;

	struct timespec						hatch_time;
	int									has_hatched;
	struct timespec						death_time;

	struct s_egg						*next;
}										t_egg;

typedef struct							s_world_event
{
	char								*type;
	int									trigger_delay;
	struct timespec						time;
	t_event_client						*concerned_clients;

	struct s_world_event				*next;
}										t_world_event;

/*
** -----------------------	World main container struct.
*/

typedef struct							s_world_hdl
{
	int									map_x;
	int									map_y;
	double								t_unit;
	char								*name_ressources[NB_RESSOURCES];
	t_world_case						**world_board;

	t_world_event						*ongoing_events;

	int									nb_of_incantations;
	int									nb_of_eggs;
	t_egg								*eggs;
}										t_world_hdl;

#endif
