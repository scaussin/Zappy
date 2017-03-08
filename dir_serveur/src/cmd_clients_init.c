/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_clients_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 16:02:03 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 16:06:23 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Link command with their name and time to the corresponding functions.
*/

void	init_cmd_match_table(t_serveur *serv)
{
	serv->cmd_hdl.cmd_match_table = s_malloc(sizeof(t_cmd_match)
		* SIZE_CMD_MATCH_TABLE);
	serv->cmd_hdl.cmd_match_table[0].name = "avance\n";
	serv->cmd_hdl.cmd_match_table[0].duration_cmd = MOVE_CMDS_TIME;
	serv->cmd_hdl.cmd_match_table[0].on_start = on_start_cmd_avance;
	serv->cmd_hdl.cmd_match_table[0].on_end = on_end_cmd_avance;
	serv->cmd_hdl.cmd_match_table[1].name = "droite\n";
	serv->cmd_hdl.cmd_match_table[1].duration_cmd = MOVE_CMDS_TIME;
	serv->cmd_hdl.cmd_match_table[1].on_start = on_start_cmd_droite;
	serv->cmd_hdl.cmd_match_table[1].on_end = on_end_cmd_droite;
	init_cmd_match_table_2(serv);
	init_cmd_match_table_3(serv);
	init_gfx_cmd_match_table(serv);
}

void	init_cmd_match_table_2(t_serveur *serv)
{
	serv->cmd_hdl.cmd_match_table[2].name = "gauche\n";
	serv->cmd_hdl.cmd_match_table[2].duration_cmd = MOVE_CMDS_TIME;
	serv->cmd_hdl.cmd_match_table[2].on_start = on_start_cmd_gauche;
	serv->cmd_hdl.cmd_match_table[2].on_end = on_end_cmd_gauche;
	serv->cmd_hdl.cmd_match_table[3].name = "voir\n";
	serv->cmd_hdl.cmd_match_table[3].duration_cmd = VOIR_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[3].on_start = on_start_cmd_voir;
	serv->cmd_hdl.cmd_match_table[3].on_end = on_end_cmd_voir;
	serv->cmd_hdl.cmd_match_table[4].name = "inventaire\n";
	serv->cmd_hdl.cmd_match_table[4].duration_cmd = INVENTAIRE_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[4].on_start = on_start_cmd_inventaire;
	serv->cmd_hdl.cmd_match_table[4].on_end = on_end_cmd_inventaire;
	serv->cmd_hdl.cmd_match_table[5].name = "prend";
	serv->cmd_hdl.cmd_match_table[5].duration_cmd = ITEM_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[5].on_start = on_start_cmd_prend;
	serv->cmd_hdl.cmd_match_table[5].on_end = on_end_cmd_prend;
}

void	init_cmd_match_table_3(t_serveur *serv)
{
	serv->cmd_hdl.cmd_match_table[6].name = "pose";
	serv->cmd_hdl.cmd_match_table[6].duration_cmd = ITEM_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[6].on_start = on_start_cmd_pose;
	serv->cmd_hdl.cmd_match_table[6].on_end = on_end_cmd_pose;
	serv->cmd_hdl.cmd_match_table[7].name = "expulse\n";
	serv->cmd_hdl.cmd_match_table[7].duration_cmd = EXPULSE_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[7].on_start = on_start_cmd_expulse;
	serv->cmd_hdl.cmd_match_table[7].on_end = on_end_cmd_expulse;
	serv->cmd_hdl.cmd_match_table[8].name = "broadcast";
	serv->cmd_hdl.cmd_match_table[8].duration_cmd = BROADCAST_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[8].on_start = on_start_cmd_broadcast;
	serv->cmd_hdl.cmd_match_table[8].on_end = on_end_cmd_broadcast;
	serv->cmd_hdl.cmd_match_table[9].name = "incantation\n";
	serv->cmd_hdl.cmd_match_table[9].duration_cmd = INCANTATION_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[9].on_start = on_start_cmd_incantation;
	serv->cmd_hdl.cmd_match_table[9].on_end = on_end_cmd_incantation;
	serv->cmd_hdl.cmd_match_table[10].name = "fork\n";
	serv->cmd_hdl.cmd_match_table[10].duration_cmd = FORK_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[10].on_start = on_start_cmd_fork;
	serv->cmd_hdl.cmd_match_table[10].on_end = on_end_cmd_fork;
	serv->cmd_hdl.cmd_match_table[11].name = "connect_nbr\n";
	serv->cmd_hdl.cmd_match_table[11].duration_cmd = CONNECT_NBR_CMD_TIME;
	serv->cmd_hdl.cmd_match_table[11].on_start = on_start_cmd_connect_nbr;
	serv->cmd_hdl.cmd_match_table[11].on_end = on_end_cmd_connect_nbr;
}

void	init_gfx_cmd_match_table(t_serveur *serv)
{
	serv->cmd_hdl.gfx_cmd_match_table = s_malloc(sizeof(t_cmd_match)
		* SIZE_CMD_MATCH_TABLE);
	serv->cmd_hdl.gfx_cmd_match_table[0].name = "msz\n";
	serv->cmd_hdl.gfx_cmd_match_table[0].on_end = gfx_cmd_msz;
	serv->cmd_hdl.gfx_cmd_match_table[0].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[1].name = "bct\n";
	serv->cmd_hdl.gfx_cmd_match_table[1].on_end = gfx_cmd_bct;
	serv->cmd_hdl.gfx_cmd_match_table[1].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[2].name = "mct\n";
	serv->cmd_hdl.gfx_cmd_match_table[2].on_end = gfx_cmd_mct;
	serv->cmd_hdl.gfx_cmd_match_table[2].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[3].name = "tna\n";
	serv->cmd_hdl.gfx_cmd_match_table[3].on_end = gfx_cmd_tna;
	serv->cmd_hdl.gfx_cmd_match_table[3].duration_cmd = 0;
	init_gfx_cmd_match_table_2(serv);
}

void	init_gfx_cmd_match_table_2(t_serveur *serv)
{
	serv->cmd_hdl.gfx_cmd_match_table[4].name = "ppo\n";
	serv->cmd_hdl.gfx_cmd_match_table[4].on_end = gfx_cmd_ppo;
	serv->cmd_hdl.gfx_cmd_match_table[4].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[5].name = "plv\n";
	serv->cmd_hdl.gfx_cmd_match_table[5].on_end = gfx_cmd_plv;
	serv->cmd_hdl.gfx_cmd_match_table[5].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[6].name = "pin\n";
	serv->cmd_hdl.gfx_cmd_match_table[6].on_end = gfx_cmd_pin;
	serv->cmd_hdl.gfx_cmd_match_table[6].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[7].name = "sgt\n";
	serv->cmd_hdl.gfx_cmd_match_table[7].on_end = gfx_cmd_sgt;
	serv->cmd_hdl.gfx_cmd_match_table[7].duration_cmd = 0;
	serv->cmd_hdl.gfx_cmd_match_table[8].name = "sst\n";
	serv->cmd_hdl.gfx_cmd_match_table[8].on_end = gfx_cmd_sst;
	serv->cmd_hdl.gfx_cmd_match_table[8].duration_cmd = 0;
}
