#include "../../includes/serveur.h"

int		on_start_cmd_incantation(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	if (param)
	{
		printf(KMAG "Bad format to cmd [incantation] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	return (init_incantation(serv, client_cur, param));
}

void	on_end_cmd_incantation(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)	param;
	char	*gfx_msg;
	int		*target_res;

	// if incantation can be interrupted, recheck the conditions at the end,
	// then strip the case of the resources.
	if (serv->settings_hdl.can_interrupt_incantation == B_TRUE)
	{
		target_res = set_incantation_target_res(client_cur->player.level);
		if (are_incantation_cdts_ok(serv, &client_cur->player, target_res))
		{
			finish_incantation(serv, client_cur, 1);
			strip_case_ressources(serv, client_cur, target_res);
			asprintf(&gfx_msg, "pie %d %d %d\n",
				client_cur->player.pos.x, client_cur->player.pos.y, 1);
		}
		else
		{
			finish_incantation(serv, client_cur, 0);
			asprintf(&gfx_msg, "pie %d %d %d\n",
				client_cur->player.pos.x, client_cur->player.pos.y, 0);
		}
	}
	// if not, incantation just finishes, res have already been taken.
	else
	{
		finish_incantation(serv, client_cur, 1);
		asprintf(&gfx_msg, "pie %d %d %d\n",
			client_cur->player.pos.x, client_cur->player.pos.y, 1);
	}
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
	
}

/*
**	Launched in the incantation on_start(). Will check if the conditions are OK
**	for starting incantation. If yes, every concerned player except the incanter
**	will be set at is_incanting = 1, blocking them until the caster launches
**	the incantation on_end() function.
*/

int		init_incantation(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)			param;
	t_player		*cur_player;
	int				*target_res;
	char			*client_msg;
	
	cur_player = &client_cur->player;
	target_res = set_incantation_target_res(cur_player->level);
	if (are_incantation_cdts_ok(serv, cur_player, target_res))
	{
		serv->world_hdl.nb_of_incantations += 1;
		cur_player->incantation_id = serv->world_hdl.nb_of_incantations;
		printf(KGRN "[Server]: Incantation #%d starting ...\n", cur_player->incantation_id);
		// Set incanter to start incanting;
		client_cur->player.is_incanter = 1;
		client_msg = strdup("elevation en cours\n");
		write_buffer(&client_cur->buff_send, client_msg, strlen(client_msg));

		// set every other player on the case.
		set_players_incanting(serv, client_cur);

		// clean the ressources on the case.
		if (serv->settings_hdl.can_interrupt_incantation == B_FALSE)
			strip_case_ressources(serv, client_cur, target_res);
		free(target_res);
		return (0);
	}
	else
	{
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		free(target_res);
		return (-1);
	}
	free(target_res);
	return (-1);
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
**	For each player on the same case as the caster and with same level (NOT including the caster),
**	set it as incanting = 1, blocking it until the caster launches its incantion endpoint.
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
			&& clients_tmp->player.incantation_id == -1
			&& clients_tmp->player.is_incanting == B_FALSE
			&& clients_tmp->player.is_incanter == B_FALSE
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
			clients_tmp->player.incantation_id = cur_player->incantation_id;
			// only the leader will send "pie" to the gfx.
			clients_tmp->player.is_incanter = 0;
		}
		clients_tmp = clients_tmp->next;
	}
	asprintf(&gfx_msg_cat, "%s\n", gfx_msg);
	free(gfx_msg);
	push_gfx_msg(serv, gfx_msg_cat);
	free(gfx_msg_cat);
}

/*
**	For each players on the same incantation as the incanter (including him),
**	makes the level up.
**	Lots of checks are made to avoid joining ongoing incantation.
**	Every incantation have an ID to avoid confusing them.
*/

void	finish_incantation(t_serveur *serv, t_client_entity *cur_client, int result)
{
	t_client_entity		*clients_tmp;
	t_player			*incanter_player;
	char				*msg;

	printf(KGRN "[Server]: Incantation #%d ending.\n", cur_client->player.incantation_id);
	clients_tmp = serv->client_hdl.list_clients;
	incanter_player = &cur_client->player;
	// for the other players -> not the incanter.
	while (clients_tmp)
	{
		if (&clients_tmp->player != incanter_player
			&& clients_tmp->player.pos.x == incanter_player->pos.x
			&& clients_tmp->player.pos.y == incanter_player->pos.y
			&& clients_tmp->player.level == incanter_player->level
			&& clients_tmp->player.is_incanting == 1
			&& clients_tmp->is_player_dead == 0
			&& clients_tmp->player.incantation_id == incanter_player->incantation_id)
		{
			clients_tmp->player.is_incanting = 0;
			clients_tmp->player.incantation_id = -1;

			// updates team and player level.
			if (result == 1)
			{
				clients_tmp->team->nb_players_per_lv[clients_tmp->player.level - 1] -= 1;
				clients_tmp->player.level += 1;
				cur_client->player.nb_see_case = get_nb_case(cur_client->player.level);
				clients_tmp->team->nb_players_per_lv[clients_tmp->player.level - 1] += 1;
			}
			// send client: "niveau actuel : K"
			asprintf(&msg, "niveau actuel : %d\n", clients_tmp->player.level);
			write_buffer(&clients_tmp->buff_send, msg, strlen(msg));
			free(msg);

			// send gfx : "plv #n L\n"
			asprintf(&msg, "plv #%d %d\n",
				clients_tmp->sock, clients_tmp->player.level);
			push_gfx_msg(serv, msg);
			free(msg);
		}
		clients_tmp = clients_tmp->next;
	}
	// Incanter must be lvled up separately and after the others.
	// Because modifiying his datas makes the upper loop impossible.
	// Also, if he died of hunger during incantation, he does not level up,
	// and dies after the incantation is over.
	if (result == 1 && cur_client->player.inventory[FOOD] > 0)
	{
		cur_client->team->nb_players_per_lv[cur_client->player.level - 1] -= 1;
		cur_client->player.level += 1;
		cur_client->player.nb_see_case = get_nb_case(cur_client->player.level);
		cur_client->team->nb_players_per_lv[cur_client->player.level - 1] += 1;

		// send client: "niveau actuel : K"
		asprintf(&msg, "niveau actuel : %d\n", cur_client->player.level);
		write_buffer(&cur_client->buff_send, msg, strlen(msg));
		free(msg);

		// send gfx : "plv #n L\n"
		asprintf(&msg, "plv #%d %d\n",
			cur_client->sock, cur_client->player.level);
		push_gfx_msg(serv, msg);
		free(msg);
	}
	cur_client->player.incantation_id = -1;
	cur_client->player.is_incanter = 0;
	serv->world_hdl.nb_of_incantations -= 1;

	// gfx world block ressource update.
	asprintf(&msg, "bct %d %d\n",
		cur_client->player.pos.x,
		cur_client->player.pos.y);
	gfx_cmd_bct(serv, serv->client_hdl.gfx_client, msg);
	free(msg);	
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
	asprintf(&gfx_msg, "bct %d %d\n",
		client_cur->player.pos.x,
		client_cur->player.pos.y);
	gfx_cmd_bct(serv, serv->client_hdl.gfx_client, gfx_msg);
	free(gfx_msg);
}
