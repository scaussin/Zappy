/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inventaire.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:56:45 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 17:56:47 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_inventaire(t_serveur *serv, t_client_entity *client_cur,
									char *param)
{
	(void)serv;
	if (param)
	{
		printf(KMAG "Bad format to cmd [inventaire] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	return (0);
}

void	on_end_cmd_inventaire(t_serveur *serv, t_client_entity *client_cur,
								char *param)
{
	cmd_inventaire(serv, client_cur, param);
}

void	cmd_inventaire(struct s_serveur *serv,
			struct s_client_entity *client_cur, char *param)
{
	t_player			*cur_player;
	char				*msg;
	long				time_left;

	(void)serv;
	(void)param;
	cur_player = &(client_cur->player);
	time_left = get_food_as_time(serv, client_cur);
	asprintf(&msg, "{nourriture %ld, linemate %d, deraumere %d,"
					" sibur %d, mendiane %d, phiras %d, thystame %d}\n",
		time_left,
		cur_player->inventory[LINEMATE],
		cur_player->inventory[DERAUMERE],
		cur_player->inventory[SIBUR],
		cur_player->inventory[MENDIANE],
		cur_player->inventory[PHIRAS],
		cur_player->inventory[THYSTAME]);
	write_buffer(&client_cur->buff_send, msg, strlen(msg));
	free(msg);
}
