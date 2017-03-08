/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_plv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:44:17 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:44:19 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command plv: asking for the level of one player by its sock number.
*/

void	gfx_cmd_plv(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	int					client_nb;
	t_client_entity		*client_tmp;
	char				*gfx_msg;

	if (!regex_match(param, "^plv #[0-9]+\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [plv] from gfx client\n" KRESET);
		return ;
	}
	if (get_player_nb(param, &client_nb, gfx_client) < 0)
		return ;
	client_tmp = serv->client_hdl.list_clients;
	while (client_tmp)
	{
		if (client_tmp->sock == client_nb && !client_tmp->is_gfx
			&& !client_tmp->is_player_dead && !client_tmp->is_disconnecting)
			return (plv_send_msg(serv, client_tmp));
		client_tmp = client_tmp->next;
	}
	printf(KMAG "[Serveur]: gfx cmd [plv]: could not find player\n" KRESET);
	asprintf(&gfx_msg, "smg could not find player for cmd [plv]\n");
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}

void	plv_send_msg(t_serveur *serv, t_client_entity *client_tmp)
{
	char				*gfx_msg;

	asprintf(&gfx_msg, "plv #%d %d\n",
		client_tmp->sock, client_tmp->player.level);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}
