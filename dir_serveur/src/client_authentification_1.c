/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_authentification_1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/10 16:23:54 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/10 16:23:56 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	client_authenticate_send_gfx_msg(t_serveur *serv,
	t_client_entity *client)
{
	char	*str_to_send;

	asprintf(&str_to_send, "pnw #%d %d %d %d %d %s\n",
		client->sock, client->player.pos.x, client->player.pos.y,
		client->player.dir + 1, client->player.level, client->team->name);
	push_gfx_msg(serv, str_to_send);
	free(str_to_send);
}
