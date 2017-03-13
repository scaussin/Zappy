/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_struct.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 15:05:07 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 15:05:16 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_STRUCT_H
# define SERVEUR_STRUCT_H

# include "serveur.h"

/*
** -----------------------	Main client struct.
*/

typedef struct							s_client_entity
{
	int									is_in_game;
	int									is_player_dead;
	int									is_gfx;
	int									is_disconnecting;

	t_socket							sock;
	t_team_entity						*team;

	t_buffer							buff_recv;
	t_buffer							buff_send;
	int									size_list_cmds;
	t_list_cmds_entity					*list_cmds;
	struct timespec						delay_time;

	t_player							player;

	struct s_client_entity				*next;
}										t_client_entity;

/*
** ******************** Serveur Main Struct ******************
*/

typedef struct							s_serveur
{
	t_network_data						network;
	t_team_hdl							team_hdl;
	t_client_hdl						client_hdl;
	t_cmd_hdl							cmd_hdl;
	t_world_hdl							world_hdl;

	int									victory_reached;
	unsigned int						loop_nb;

	char								*print_buff;

	t_game_settings						settings_hdl;
}										t_serveur;

#endif
