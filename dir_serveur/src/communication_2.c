/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:32:33 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 17:33:05 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

int			check_size_read(t_client_entity *client)
{
	int		size_read;

	size_read = BUFF_SIZE - client->buff_recv.len;
	if (size_read == 0)
		perror("read buffer full");
	return (size_read);
}

/*
**	Socket caught change, but bytes nb = 0.
*/

void		client_connection_lost(t_serveur *serv,
				t_client_entity *p_client)
{
	char				*msg;

	if (p_client->is_gfx == 1)
		serv->client_hdl.gfx_client = NULL;
	if (p_client->is_in_game == 1)
	{
		asprintf(&msg, "pdi #%d\n", p_client->sock);
		push_gfx_msg(serv, msg);
		free(msg);
	}
	disconnect_client(p_client);
	remove_client(serv, p_client);
}
