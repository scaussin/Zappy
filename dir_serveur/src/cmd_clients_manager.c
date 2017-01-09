#include "../includes/serveur.h"

/*
**	buff_recv to t_list_cmds_entity
*/

void	lex_and_parse_cmds(t_client_entity *client, t_cmd_match *cmd_match_table)
{
	char *buff;
	char *cmd;
	//char *last_end;

	if (client->size_list_cmds >= MAX_LIST_CMD)
	{
		client->buff_recv.len = 0;
		printf("[WARNING] : list cmds full on sock: %d\n", client->sock);
		return ;
	}
	buff = read_buffer(&client->buff_recv);
	cmd = strtok(buff, END); // -> Lexer.
	while (cmd)
	{
		check_cmd_match(cmd_match_table, client, cmd); // -> Parser.
		cmd = strtok(NULL, END);
	}
	//last_end = strrchr(buff, END); last update len, start
	if (buff)
		free(buff);
}

void	check_cmd_match(t_cmd_match *cmd_match_table, t_client_entity *client, char *cmd)
{
	int i = 0;

	while (i < SIZE_CMD_MATCH_TABLE)
	{
		if (strcmp(cmd_match_table[i].name, cmd) == 0)
		{
			add_cmd(client, &cmd_match_table[i], cmd);
			return ;
		}
		i++;
	}
	printf("[WARNING] : Unknown command: %s on sock: %d\n", cmd, client->sock);
}

void	add_cmd(t_client_entity *client, t_cmd_match *cmd, char *param)
{
	t_list_cmds_entity	*last;
	t_list_cmds_entity	*new_cmd;

	new_cmd = s_malloc(sizeof(t_list_cmds_entity));
	new_cmd->func = cmd->func;
	new_cmd->time = -1;
	new_cmd->param = strdup(param);
	new_cmd->next = NULL;
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

void	exec_cmd_client(t_serveur *serv)
{
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->list_cmds)
		{
			if (p_client->list_cmds->time == -1)//start cmd
			{
				p_client->list_cmds->func(serv, p_client, p_client->list_cmds->param);
			}
			else if (p_client->list_cmds->time == 0)//end cmd
			{
				if (p_client->list_cmds->param)
					free(p_client->list_cmds->param);
				p_client->list_cmds = p_client->list_cmds->next;
				p_client->size_list_cmds -= 1;
			}
			else// decremente time
			{
				p_client->list_cmds->time -= 1;
			}
		}
		p_client = p_client->next;
	}
}
