/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication_gfx_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:01:21 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 18:01:40 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	exec_gfx_cmd(t_serveur *serv, t_client_entity *gfx_client, char *cmd,
	int nb_of_parsed_chars)
{
	int		i;

	i = 0;
	while (i < SIZE_GFX_CMD_MATCH_TABLE)
	{
		if (strncmp(serv->cmd_hdl.gfx_cmd_match_table[i].name,
			cmd, nb_of_parsed_chars) == 0)
		{
			serv->cmd_hdl.gfx_cmd_match_table[i].on_end(serv, gfx_client,
				cmd);
			return ;
		}
		i++;
	}
}
