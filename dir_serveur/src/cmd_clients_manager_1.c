/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_clients_manager_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:01:45 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 17:04:32 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

int		compare_cmd(char *s1, char *s2)
{
	int		i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

void	add_cmd(t_client_entity *client, t_cmd_match *cmd, char *param)
{
	t_list_cmds_entity	*last;
	t_list_cmds_entity	*new_cmd;

	new_cmd = s_malloc(sizeof(t_list_cmds_entity));
	new_cmd->on_start = cmd->on_start;
	new_cmd->on_end = cmd->on_end;
	new_cmd->duration_cmd = cmd->duration_cmd;
	new_cmd->cmd_started = 0;
	new_cmd->success = 0;
	if (param)
		new_cmd->param = strdup(param);
	else
		new_cmd->param = NULL;
	new_cmd->next = NULL;
	if (!client->list_cmds)
		client->list_cmds = new_cmd;
	else
	{
		last = client->list_cmds;
		while (last && last->next)
			last = last->next;
		last->next = new_cmd;
	}
	client->size_list_cmds += 1;
}
