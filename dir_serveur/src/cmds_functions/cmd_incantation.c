#include "../../includes/serveur.h"

void	cmd_incantation(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)client_cur;
	(void)param;
	t_player		*cur_player;
	int				*target_res;
	t_world_event	*incantation_event; // will be called after 300 delay time.
	

	/*
		L’objectif de tous est de s’élever dans la hiérarchie Trantorienne. Ce rituel qui permet
		d’accroître ses capacités physiques et mentales doit être effectué selon un rite particulier.
		Il faut en effet réunir sur la même unité de terrain :
		• une combinaison de pierres,
		• un certain nombre de joueurs de même niveau.
		Un joueur débute l’incantation et l’élévation est alors en cours. Il n’est pas nécessaire
		que les joueurs soient de la même équipe. Seul leur niveau importe. Tous les joueurs du
		groupe réalisant l’incantation atteignent le niveau supérieur.
	*/
	cur_player = &client_cur->player;
	target_res = set_incantation_target_res(cur_player->level);
	// prepare incantation node for chained list.
	incantation_event = NULL;
	init_game_event(incantation_event, "incantation_end", 300);
	if (are_incantation_cdts_ok(serv, cur_player, target_res))
	{

	}
}

/*
**	From the player's level, get the nb of ressources on case required for incantation.
**	For example, lvl 1-2 need one player on case and one linemate -> ("1100000").
**	The returned int array will be used for comparison.
*/

int			*set_incantation_target_res(int plevel)
{
	char		*target_res_str;
	int			*target_res;
	int			i;

	i = 0;
	// target string set according to level;
	if (plevel == 1)
			target_res_str = strdup("1100000"); // I'll use zero case for nb of players.
	else if (plevel == 2)
			target_res_str = strdup("2111000");
	else if (plevel == 3)
			target_res_str = strdup("2201020");
	else if (plevel == 4)
			target_res_str = strdup("4112010");
	else if (plevel == 5)
			target_res_str = strdup("4121300");
	else if (plevel == 6)
			target_res_str = strdup("6123010");
	else if (plevel == 7)
			target_res_str = strdup("6222221");

	target_res = s_malloc(sizeof(int) * 7);
	// Then fill array of ints with string.
	while (i < 7)
	{
		target_res[i] = target_res_str[i] - 48;
		i++;
	}
	free(target_res_str);
	return (target_res);
}

/*
**	Are all incantation conditions reunited ? If yes, returns 1.
**	Otherwise, returns 0;
*/

int			are_incantation_cdts_ok(t_serveur *serv, t_player *cur_player, int *target_res)
{
	int					nb_players_same_lvl;
	t_client_entity		*clients_tmp;

	nb_players_same_lvl = 0;
	clients_tmp = serv->client_hdl.list_clients;
	while (clients_tmp)
	{
		if (clients_tmp->player.pos.x == cur_player->pos.x
			&& clients_tmp->player.pos.y == cur_player->pos.y
			&& clients_tmp->player.level == cur_player->level)
		{
			nb_players_same_lvl += 1;
		}
		clients_tmp = clients_tmp->next;
	}
	if (nb_players_same_lvl >= target_res[0]
		&& cur_player->cur_case->ressources[LINEMATE] >= target_res[LINEMATE]
		&& cur_player->cur_case->ressources[DERAUMERE] >= target_res[DERAUMERE]
		&& cur_player->cur_case->ressources[SIBUR] >= target_res[SIBUR]
		&& cur_player->cur_case->ressources[MENDIANE] >= target_res[MENDIANE]
		&& cur_player->cur_case->ressources[PHIRAS] >= target_res[PHIRAS]
		&& cur_player->cur_case->ressources[THYSTAME] >= target_res[THYSTAME])
	{
		return (1);
	}
	return (0);
}


