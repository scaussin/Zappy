#include "../includes/serveur.h"

/*
**	buff_recv to t_list_cmds_entity
*/

void	lex_and_parse_cmds(t_serveur *serv, t_client_entity *client, t_cmd_match *cmd_match_table)
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
		check_cmd_match(serv, cmd_match_table, client, cmd); // -> Parser.
		free(cmd);
	}
}

void	check_cmd_match(t_serveur *serv, t_cmd_match *cmd_match_table, t_client_entity *client, char *cmd)
{
	int i = 0;

	while (i < SIZE_CMD_MATCH_TABLE)
	{
		if (strcmp(cmd_match_table[i].name, cmd) == 0)
		{
			add_cmd(serv, client, &cmd_match_table[i], cmd);
			return ;
		}
		i++;
	}
	return ;
	printf("[WARNING] : Unknown command: %s on sock: %d\n", cmd, client->sock);
}

void	add_cmd(t_serveur *serv, t_client_entity *client, t_cmd_match *cmd, char *param)
{
	t_list_cmds_entity	*last;
	t_list_cmds_entity	*new_cmd;

	new_cmd = s_malloc(sizeof(t_list_cmds_entity));
	new_cmd->func = cmd->func;
	new_cmd->clock_end = clock() + (CLOCKS_PER_SEC * cmd->time * serv->world_hdl.t_unit);
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
	clock_t t;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->list_cmds)
		{
			t = clock();
			if (p_client->list_cmds->clock_end <= t)//start cmd
			{
				printf(KBLU "Lag: %f seconds" KRESET, ((float)(t - p_client->list_cmds->clock_end) / CLOCKS_PER_SEC);
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
