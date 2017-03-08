/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_tna.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:55:17 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:55:18 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command tna: asking for every team's name.
*/

void	gfx_cmd_tna(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	int		x;
	char	*msg;

	x = 0;
	if (!regex_match(param, "^tna\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [tna] from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	while (x < serv->team_hdl.nb_teams)
	{
		asprintf(&msg, "tna %s\n", serv->team_hdl.array_teams[x].name);
		push_gfx_msg(serv, msg);
		free(msg);
		x++;
	}
}
