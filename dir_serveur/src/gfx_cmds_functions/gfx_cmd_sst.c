/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_cmd_sst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:53:25 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:53:27 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	gfx command sst: asking for changing the world time unit.
*/

void	gfx_cmd_sst(t_serveur *serv, t_client_entity *gfx_client, char *param)
{
	char	*gfx_msg;
	int		new_t;
	float	old_t_unit;

	if (!regex_match(param, "^sst [0-9]+\n$"))
	{
		printf(KMAG "Bad format to gfx cmd [sst] from gfx client\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return ;
	}
	if ((get_time_val(param, &new_t, gfx_client)) < 0)
		return ;
	if ((sst_check_max(new_t, gfx_client)) < 0)
		return ;
	old_t_unit = serv->world_hdl.t_unit;
	serv->world_hdl.t_unit = (float)(1.0 / new_t);
	refresh_times(serv, old_t_unit);
	asprintf(&gfx_msg, "sgt %f\n", serv->world_hdl.t_unit);
	push_gfx_msg(serv, gfx_msg);
	free(gfx_msg);
}

int		get_time_val(char *param, int *val, t_client_entity *gfx_client)
{
	char				*arg;

	errno = 0;
	arg = strchr(param, ' ') + 1;
	*val = strtol(arg, NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		printf(KMAG "Bad format to cmd [sst] "
				"from gfx client (bad integer format)\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return (-1);
	}
	return (0);
}

int		sst_check_max(int new_t, t_client_entity *gfx_client)
{
	if (new_t > MAX_GAME_TIME_UNIT || new_t <= 0)
	{
		printf(KMAG "Bad format to cmd [sst] "
				"from gfx client (max value = 1000)\n" KRESET);
		write_buffer(&gfx_client->buff_send, "sbp\n", 4);
		return (-1);
	}
	return (0);
}
