#include "../includes/serveur.h"

/*
**	buff_recv to t_list_cmds_entity
*/

void	lex_and_parse_cmds(t_client_entity *client, t_cmd_match *cmd_match_table)
{
	char *cmd;

	if (client->size_list_cmds >= MAX_LIST_CMD)
	{
		client->buff_recv.len = 0;
		printf("[WARNING] : list cmds full on sock: %d\n", client->sock);
		return ;
	}
	while ((cmd = get_first_cmd(&client->buff_recv))) // -> Buffer lexer.
	{
		check_cmd_match(cmd_match_table, client, cmd); // -> Parser.
		free(cmd);
	}
}

/*
**	Parser for cmd. This will parse only n chars from the cmd, depending on
**	if there is a space in the cmd.
**	(example : for "prend 2", we only want to parse "prend").
*/

void	check_cmd_match(t_cmd_match *cmd_match_table, t_client_entity *client, char *cmd)
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
	// Parsing
	while (i < SIZE_CMD_MATCH_TABLE)
	{
		if (compare_cmd(cmd_match_table[i].name, cmd) == 0)
		{
			add_cmd(client, &cmd_match_table[i], param);
			return ;
		}
		i++;
	}
	printf(KMAG "[Serveur]: Unknown command: %s on sock: %d\n" KRESET, cmd, client->sock);
	return ;
}

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

	// Add cmd to chained list of cmds.
	if (!client->list_cmds)
		client->list_cmds = new_cmd;
	else
	{

		last = client->list_cmds;
		while (last && last->next)
		{
			last = last->next;
		}
		last->next = new_cmd;
	}
	client->size_list_cmds += 1;
}

/*
**	For each player, executes the start point then the endpoint of the current cmd.
**	If the start point fails (mainly player input error), the end point is canceled.
*/

struct timespec	*exec_cmd_client(t_serveur *serv)
{
	t_client_entity			*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{	
		// execute start point of first cmd
		if (p_client->list_cmds && p_client->list_cmds->cmd_started == B_FALSE)
		{
			if (p_client->player.is_incanting == B_FALSE) // incanting player == blocked.
			{
				if (p_client->list_cmds->on_start(serv, p_client, p_client->list_cmds->param) != -1)
				{
					p_client->list_cmds->cmd_started = B_TRUE;

					// set end time of cmd for launching endpoint.
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
		// execute end point of started first cmd.
		else if (p_client->list_cmds && p_client->list_cmds->cmd_started == B_TRUE)
		{
			// incanting player cant exec on_end. but incanter can.
			if (p_client->player.is_incanting == B_FALSE)
			{
				// cmd started and not incanting, now look for end point time;
				if (timespec_is_over(p_client->list_cmds->end_time))
				{
					// exec cmd.
					p_client->list_cmds->on_end(serv, p_client, p_client->list_cmds->param);
					clean_clients_first_cmd(p_client);
				}
			}
		}
		p_client = p_client->next;
	}
	return (NULL);
}

/*
**	Helper function: cleans the first node from the client's cmd chained list.
*/

void	clean_clients_first_cmd(t_client_entity *p_client)
{
	t_list_cmds_entity		*cmd_to_free; // stock ptr to free.

	if (p_client->list_cmds->param)
		free(p_client->list_cmds->param);
	cmd_to_free = p_client->list_cmds;
	p_client->list_cmds = p_client->list_cmds->next;
	p_client->size_list_cmds -= 1;
	free(cmd_to_free);
}
