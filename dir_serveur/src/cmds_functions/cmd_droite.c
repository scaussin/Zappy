/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_droite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 16:32:09 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 16:32:16 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_droite(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	char		*msg;

	if (param)
	{
		printf(KMAG "Bad format to cmd [droite]"
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	cmd_droite(serv, client_cur, param);
	asprintf(&msg, "ppo #%d %d %d %d\n",
		client_cur->sock,
		client_cur->player.pos.x,
		client_cur->player.pos.y,
		client_cur->player.dir + 1);
	push_gfx_msg(serv, msg);
	free(msg);
	return (0);
}

void	on_end_cmd_droite(t_serveur *serv, t_client_entity *client_cur,
							char *param)
{
	(void)serv;
	(void)param;
	write_buffer(&client_cur->buff_send, "ok\n", 3);
}

void	cmd_droite(struct s_serveur *serv, struct s_client_entity *client_cur,
					char *param)
{
	(void)serv;
	(void)param;
	client_cur->player.dir += 1;
	if (client_cur->player.dir > 3)
		client_cur->player.dir = 0;
}
