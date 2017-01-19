#include "../includes/serveur.h"

/*
**	Link command with their name and time to the corresponding functions.
*/

void		init_cmd_match_table(t_serveur *serv)
{
	// Allocating number of possible commands.
	serv->cmd_hdl.cmd_match_table = s_malloc(sizeof(t_cmd_match)
		* SIZE_CMD_MATCH_TABLE);

	serv->cmd_hdl.cmd_match_table[0].name = "avance\n";
	serv->cmd_hdl.cmd_match_table[0].func = cmd_avance;
	serv->cmd_hdl.cmd_match_table[0].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[1].name = "droite\n";
	serv->cmd_hdl.cmd_match_table[1].func = cmd_droite;
	serv->cmd_hdl.cmd_match_table[1].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[2].name = "gauche\n";
	serv->cmd_hdl.cmd_match_table[2].func = cmd_gauche;
	serv->cmd_hdl.cmd_match_table[2].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[3].name = "voir\n";
	serv->cmd_hdl.cmd_match_table[3].func = cmd_voir;
	serv->cmd_hdl.cmd_match_table[3].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[4].name = "inventaire\n";
	serv->cmd_hdl.cmd_match_table[4].func = cmd_inventaire;
	serv->cmd_hdl.cmd_match_table[4].duration_cmd = 1;

	serv->cmd_hdl.cmd_match_table[5].name = "prend"; // no \n cause of args.
	serv->cmd_hdl.cmd_match_table[5].func = cmd_prend;
	serv->cmd_hdl.cmd_match_table[5].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[6].name = "pose"; // no \n cause of args.
	serv->cmd_hdl.cmd_match_table[6].func = cmd_pose;
	serv->cmd_hdl.cmd_match_table[6].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[7].name = "expulse\n";
	serv->cmd_hdl.cmd_match_table[7].func = cmd_expulse;
	serv->cmd_hdl.cmd_match_table[7].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[8].name = "broadcast"; // no \n cause of args.
	serv->cmd_hdl.cmd_match_table[8].func = cmd_broadcast;
	serv->cmd_hdl.cmd_match_table[8].duration_cmd = 7;

	serv->cmd_hdl.cmd_match_table[9].name = "incantation\n";
	serv->cmd_hdl.cmd_match_table[9].func = cmd_incantation;
	serv->cmd_hdl.cmd_match_table[9].duration_cmd = 300;

	serv->cmd_hdl.cmd_match_table[10].name = "fork\n";
	serv->cmd_hdl.cmd_match_table[10].func = cmd_fork;
	serv->cmd_hdl.cmd_match_table[10].duration_cmd = 42;

	serv->cmd_hdl.cmd_match_table[11].name = "connect_nbr\n";
	serv->cmd_hdl.cmd_match_table[11].func = cmd_connect_nbr;
	serv->cmd_hdl.cmd_match_table[11].duration_cmd = 0;
}