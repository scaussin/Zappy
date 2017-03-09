/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:54:04 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 17:54:06 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	Strip case of its ressources according to the target res.
**	Right now, strip at incant start, but protected in case it is
**	done at incantation end.
*/

void		strip_case_ressources(t_serveur *serv, t_client_entity *client_cur,
							int *target_res)
{
	int		res_i;
	char	*gfx_msg;

	res_i = 1;
	while (res_i < 7)
	{
		if (client_cur->player.cur_case->ressources[res_i] >= target_res[res_i])
			client_cur->player.cur_case->ressources[res_i] -= target_res[res_i];
		else
		{
			printf(KMAG "[Warning]: not enough ressources on case"
				"to be stripped(incantation cmd)\n" KRESET);
		}
		res_i++;
	}
	asprintf(&gfx_msg, "bct %d %d\n",
		client_cur->player.pos.x,
		client_cur->player.pos.y);
	gfx_cmd_bct(serv, serv->client_hdl.gfx_client, gfx_msg);
	free(gfx_msg);
}

void		put_player_in_incantation(t_serveur *serv,
										t_client_entity *cur_client,
										t_client_entity *clients_tmp)
{
	char				*client_msg;
	int					client_msg_len;

	(void)serv;
	if (cur_client && clients_tmp)
	{
		client_msg = strdup("elevation en cours\n");
		client_msg_len = strlen(client_msg);
		write_buffer(&clients_tmp->buff_send, client_msg, client_msg_len);
		free(client_msg);
		clients_tmp->player.is_incanting = 1;
		clients_tmp->player.incantation_id = cur_client->player.incantation_id;
		clients_tmp->player.is_incanter = 0;
	}
}
