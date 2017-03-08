/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_msz.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:27:46 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:27:48 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command msz: asking for the world size.
*/

void	gfx_cmd_msz(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	char			*msg;

	if (!regex_match(param, "^msz\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [msz] "
					"from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	asprintf(&msg, "msz %d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	write_buffer(&gfx_client->buff_send, msg, strlen(msg));
	free(msg);
}
