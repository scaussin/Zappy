/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_pin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:34:04 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:34:05 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command pin: asking for the inventory of one player by its sock number.
*/

void	gfx_cmd_pin(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	int					client_nb;
	t_client_entity		*client_tmp;
	char				*gfx_msg;

	if (!regex_match(param, "^pin #[0-9]+\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [pin] from gfx client\n" KRESET);
		return ;
	}
	if (get_player_nb(param, &client_nb, gfx_client) < 0)
		return ;
	client_tmp = serv->client_hdl.list_clients;
	while (client_tmp)
	{
		if (client_tmp->sock == client_nb && !client_tmp->is_gfx
			&& !client_tmp->is_player_dead && !client_tmp->is_disconnecting)
			return (send_client_inv(serv, client_tmp));
		client_tmp = client_tmp->next;
	}
	printf(KMAG "[Serveur]: gfx cmd [pin]: could not find player\n" KRESET);
	asprintf(&gfx_msg, "smg could not find player for cmd [pin]\n");
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}

int		get_player_nb(char *param, int *client_nb,
							t_client_entity *gfx_client)
{
	char				*arg;

	errno = 0;
	arg = strchr(param, '#') + 1;
	*client_nb = strtol(arg, NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		printf(KMAG "Bad format to gfx cmd "
				"from gfx client (bad integer format) : %s\n" KRESET,
				param);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return (-1);
	}
	return (0);
}

void	send_client_inv(t_serveur *serv, t_client_entity *client_tmp)
{
	long				time_left;
	char				*gfx_msg;

	time_left = get_food_as_time(serv, client_tmp);
	if (time_left < 0)
		time_left = 0;
	asprintf(&gfx_msg, "pin #%d %d %d %ld %d %d %d %d %d %d\n",
		client_tmp->sock, client_tmp->player.pos.x, client_tmp->player.pos.y,
		time_left,
		client_tmp->player.inventory[LINEMATE],
		client_tmp->player.inventory[DERAUMERE],
		client_tmp->player.inventory[SIBUR],
		client_tmp->player.inventory[MENDIANE],
		client_tmp->player.inventory[PHIRAS],
		client_tmp->player.inventory[THYSTAME]);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}
