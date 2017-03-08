/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 16:53:54 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 16:53:57 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	Fork will put the player in egg laying status. When the egg is
**	layed(after 42/t),
**	the player can continue his life. The egg will lay after 600/t,
**	see game_player_events.c for what happen when the player finished
**	laying the egg.
*/

int		on_start_cmd_fork(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	char *gfx_msg;

	if (param)
	{
		printf(KMAG "Bad format to cmd [fork] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	printf(KGRN "[Serveur]: player #%d laying egg ... \n" KRESET,
			client_cur->sock);
	asprintf(&gfx_msg, "pfk #%d\n", client_cur->sock);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
	return (0);
}

void	on_end_cmd_fork(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	char	*gfx_msg;

	(void)param;
	add_new_egg(serv, client_cur);
	write_buffer(&client_cur->buff_send, "ok\n", 3);
	asprintf(&gfx_msg, "enw #%d #%d %d %d\n",
		serv->world_hdl.nb_of_eggs,
		client_cur->sock,
		client_cur->player.pos.x,
		client_cur->player.pos.y);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}
