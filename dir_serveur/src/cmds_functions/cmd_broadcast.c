/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 14:59:25 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 14:59:37 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_broadcast(t_serveur *serv, t_client_entity *client_cur,
								char *param)
{
	char				*msg;

	if (!regex_match(param, "^[a-zA-Z0-9 ']+\n$"))
	{
		printf(KMAG "Bad format to cmd [broadcast] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	asprintf(&msg, "pbc #%d %.*s\n", client_cur->sock,
				(int)strlen(param) - 1, param);
	push_gfx_msg(serv, msg);
	free(msg);
	return (0);
}

void	on_end_cmd_broadcast(t_serveur *serv, t_client_entity *client_cur,
								char *param)
{
	cmd_broadcast(serv, client_cur, param);
	write_buffer(&client_cur->buff_send, "ok\n", 3);
}

void	cmd_broadcast(t_serveur *serv, t_client_entity *client_cur,
						char *param)
{
	t_pos					son_source;
	t_pos					son_dest;
	int						provenance;
	char					*msg_client;
	t_client_entity			*list_clients;

	list_clients = serv->client_hdl.list_clients;
	while (list_clients)
	{
		if (list_clients->is_in_game == 1 && list_clients->is_gfx == 0
			&& list_clients->is_disconnecting == 0
			&& &(list_clients->player) != &client_cur->player)
		{
			set_pos_varialbes(client_cur, list_clients, &son_source, &son_dest);
			provenance = provenance_son(serv, son_source, son_dest);
			provenance = provenance_with_dir(provenance,
				list_clients->player.dir);
			asprintf(&msg_client, "message %d,%s", provenance, param);
			write_buffer(&list_clients->buff_send, msg_client,
				strlen(msg_client));
			free(msg_client);
			msg_client = NULL;
		}
		list_clients = list_clients->next;
	}
}

double	provenance_with_dir(int provenance, int dir)
{
	if (provenance == 0)
		return (0);
	provenance = (provenance + 2 * dir) % 8;
	if (provenance == 0)
		return (8);
	return (provenance);
}

void	set_pos_varialbes(t_client_entity *client_cur,
	t_client_entity *list_clients, t_pos *source, t_pos *dest)
{
	source->x = client_cur->player.pos.x;
	source->y = client_cur->player.pos.y;
	dest->x = list_clients->player.pos.x;
	dest->y = list_clients->player.pos.y;
}
