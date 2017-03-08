/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_clients_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:01:11 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 17:03:58 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	buff_recv to t_list_cmds_entity
*/

void			lex_and_parse_cmds(t_client_entity *client,
	t_cmd_match *cmd_match_table)
{
	char *cmd;

	if (client->size_list_cmds >= MAX_LIST_CMD)
	{
		client->buff_recv.len = 0;
		printf("[WARNING] : list cmds full on sock: %d\n", client->sock);
		return ;
	}
	while ((cmd = get_first_cmd(&client->buff_recv)))
	{
		check_cmd_match(cmd_match_table, client, cmd);
		free(cmd);
	}
}

/*
**	Parser for cmd. This will parse only n chars from the cmd, depending on
**	if there is a space in the cmd.
**	(example : for "prend 2", we only want to parse "prend").
*/

void			check_cmd_match(t_cmd_match *cmd_match_table,
	t_client_entity *client, char *cmd)
{
	int		i;
	char	*param;

	i = 0;
	param = NULL;
	if ((param = strchr(cmd, ' ')))
	{
		*param = '\0';
		param = param + 1;
	}
	while (i < SIZE_CMD_MATCH_TABLE)
	{
		if (compare_cmd(cmd_match_table[i].name, cmd) == 0)
		{
			add_cmd(client, &cmd_match_table[i], param);
			return ;
		}
		i++;
	}
	printf(KMAG "[Serveur]: Unknown command: %s on sock: %d\n" KRESET, cmd,
		client->sock);
	return ;
}

/*
**	For each player, executes the start point then the endpoint of the current
**	cmd.
**	If the start point fails for various reasons, the end point is canceled.
*/

struct timespec	*exec_cmd_client(t_serveur *serv)
{
	t_client_entity			*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->list_cmds && p_client->list_cmds->cmd_started == B_FALSE)
			exec_cmd_client_start(serv, p_client);
		else if (p_client->list_cmds &&
			p_client->list_cmds->cmd_started == B_TRUE)
		{
			if (p_client->player.is_incanting == B_FALSE)
			{
				if (timespec_is_over(p_client->list_cmds->end_time))
				{
					p_client->list_cmds->on_end(serv, p_client,
						p_client->list_cmds->param);
					clean_clients_first_cmd(p_client);
				}
			}
		}
		p_client = p_client->next;
	}
	return (NULL);
}

void			exec_cmd_client_start(t_serveur *serv,
	t_client_entity *p_client)
{
	if (p_client->player.is_incanting == B_FALSE)
	{
		if (p_client->list_cmds->on_start(serv, p_client,
			p_client->list_cmds->param) != -1)
		{
			p_client->list_cmds->cmd_started = B_TRUE;
			get_time(&p_client->list_cmds->end_time);
			add_nsec_to_timespec(&p_client->list_cmds->end_time,
				p_client->list_cmds->duration_cmd
				* serv->world_hdl.t_unit * 1000000000);
		}
		else
		{
			clean_clients_first_cmd(p_client);
		}
	}
}

/*
**	Helper function: cleans the first node from the client's cmd chained list.
*/

void			clean_clients_first_cmd(t_client_entity *p_client)
{
	t_list_cmds_entity		*cmd_to_free;

	if (p_client->list_cmds->param)
		free(p_client->list_cmds->param);
	cmd_to_free = p_client->list_cmds;
	p_client->list_cmds = p_client->list_cmds->next;
	p_client->size_list_cmds -= 1;
	free(cmd_to_free);
}
