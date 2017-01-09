#include "../includes/serveur.h"

/*
**	Link command with their name to the corresponding functions.
**	TODO: Expand with all the commands.
*/

void		init_cmd_match_table(t_serveur *serv)
{
	// Allocating number of possible commands.
	serv->cmd_match_table = s_malloc(sizeof(t_cmd_match)
		* SIZE_CMD_MATCH_TABLE);

	// Setting those actual commands.
	serv->cmd_match_table[0].name = "avance";
	serv->cmd_match_table[0].func = cmd_avance;
	serv->cmd_match_table[1].name = "droite";
	serv->cmd_match_table[1].func = cmd_droite;
}