#include "../../includes/serveur.h"

void	cmd_incantation(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)client_cur;
	t_player		*cur_player;
	int				*target_res;
	char			*client_msg;

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
	if (param)
	{
		printf(KMAG "Bad format to cmd [incantation] "
					"from sock %d\n" KRESET, client_cur->sock);
		return ;
	}
	cur_player = &client_cur->player;
	target_res = set_incantation_target_res(cur_player->level);
	if (are_incantation_cdts_ok(serv, cur_player, target_res))
	{
		// Set incanter to start incanting;
		client_cur->player.is_incanting = 1;
		client_cur->player.is_incanter = 1;
		client_msg = strdup("elevation en cours\n");
		write_buffer(&client_cur->buff_send, client_msg, strlen(client_msg));

		// Set incanter delay time.
		get_time(&client_cur->delay_time);
		add_nsec_to_timespec(&client_cur->delay_time,
			INCANTATION_TIME * serv->world_hdl.t_unit * 1000000000);
		get_time(&client_cur->player.incantation_end_time);
		add_nsec_to_timespec(&client_cur->player.incantation_end_time,
			INCANTATION_TIME * serv->world_hdl.t_unit * 1000000000);

		// set every other player on the case.
		set_players_incanting(serv, client_cur);

		// clean the ressources on the case.
		strip_case_ressources(serv, client_cur, target_res);
	}
	else
	{
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		get_time(&client_cur->delay_time);
	}
	free(target_res);
}

/*
**	From the player's level, get the nb of ressources on case required for incantation.
**	For example, lvl 1-2 need one player and one linemate -> ("1100000").
**	The returned int array will be used for comparison, and must be freed with free();
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

/*
**	For each player on the same case as the caster and with same level (including the caster),
**	set it as incanting = 1, and setting the time at which he will get level += 1.
*/

void	set_players_incanting(t_serveur *serv, t_client_entity *cur_client)
{
	t_client_entity		*clients_tmp;
	char				*gfx_msg;
	char				*gfx_msg_cat;
	char				*client_msg;
	int 				client_msg_len;
	t_player			*cur_player;

	clients_tmp = serv->client_hdl.list_clients;
	cur_player = &cur_client->player;
	// "pic X Y L #n #n …\n" - Premier joueur lance l’incantation pour tous les
	// suivants sur la case.
	asprintf(&gfx_msg, "pic %d %d %d #%d",
		cur_player->pos.x, cur_player->pos.y, cur_player->level, cur_client->sock);
	client_msg = strdup("elevation en cours\n");
	client_msg_len = strlen(client_msg);
	while (clients_tmp)
	{
		if (&clients_tmp->player != cur_player
			&& clients_tmp->player.pos.x == cur_player->pos.x
			&& clients_tmp->player.pos.y == cur_player->pos.y
			&& clients_tmp->player.level == cur_player->level)
		{
			// send "elevation en cours" to all concerned clients.
			write_buffer(&clients_tmp->buff_send, client_msg, client_msg_len);
			// cat player's sock value at each loop turn.
			asprintf(&gfx_msg_cat, "%s #%d", gfx_msg, clients_tmp->sock);
			free(gfx_msg);
			gfx_msg = strdup(gfx_msg_cat);
			free(gfx_msg_cat);

			// set client time to the future level up.
			clients_tmp->player.is_incanting = 1;
			// only the leader will send "pie" to the gfx.
			clients_tmp->player.is_incanter = 0;
			get_time(&clients_tmp->delay_time);
			add_nsec_to_timespec(&clients_tmp->delay_time,
				INCANTATION_TIME * serv->world_hdl.t_unit * 1000000000);

			get_time(&clients_tmp->player.incantation_end_time);
			add_nsec_to_timespec(&clients_tmp->player.incantation_end_time,
				INCANTATION_TIME * serv->world_hdl.t_unit * 1000000000);
		}
		clients_tmp = clients_tmp->next;
	}
	asprintf(&gfx_msg_cat, "%s\n", gfx_msg);
	free(gfx_msg);
	push_gfx_msg(serv, gfx_msg_cat);
	free(gfx_msg_cat);
}

/*
**	Strip case of its ressources according to the target res.
**	Right now, strip at incant start, but protected in case it is done at incantation end.
*/

void strip_case_ressources(t_serveur *serv, t_client_entity *client_cur, int *target_res)
{
	int		res_i;
	char	*gfx_msg;

	res_i = 1;  // number of players/food on case does not need to be stripped.
	while (res_i < 7)
	{
		if (client_cur->player.cur_case->ressources[res_i] >= target_res[res_i])
		{
			client_cur->player.cur_case->ressources[res_i] -= target_res[res_i];
		}
		else
		{
			printf(KMAG "[Warning]: not enough ressources on case"
				"to be stripped(incantation cmd)\n" KRESET);
		}
		res_i++;
	}

	// gfx world block ressource update.
	asprintf(&gfx_msg, "bct %d %d %d %d %d %d %d %d %d\n",
		client_cur->player.pos.x,
		client_cur->player.pos.y,
		client_cur->player.cur_case->ressources[FOOD],
		client_cur->player.cur_case->ressources[LINEMATE],
		client_cur->player.cur_case->ressources[DERAUMERE],
		client_cur->player.cur_case->ressources[SIBUR],
		client_cur->player.cur_case->ressources[MENDIANE],
		client_cur->player.cur_case->ressources[PHIRAS],
		client_cur->player.cur_case->ressources[THYSTAME]);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}
