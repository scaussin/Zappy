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
	while ((cmd = get_first_cmd(&client->buff_recv)))
	{
		check_cmd_match(cmd_match_table, client, cmd); // -> Parser.
		free(cmd);
	}
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
	return ;
	printf("[WARNING] : Unknown command: %s on sock: %d\n", cmd, client->sock);
}

void	add_cmd(t_client_entity *client, t_cmd_match *cmd, char *param)
{
	t_list_cmds_entity	*last;
	t_list_cmds_entity	*new_cmd;

	new_cmd = s_malloc(sizeof(t_list_cmds_entity));
	new_cmd->func = cmd->func;
	new_cmd->duration_cmd = cmd->duration_cmd;
	new_cmd->time_end.tv_sec = 0;
	new_cmd->time_end.tv_nsec = 0;
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

int		timespec_is_over(struct timespec time_end)
{
	struct timespec	now;

	clock_gettime(CLOCK_REALTIME, &now);
	if (now.tv_sec >= time_end.tv_sec && now.tv_nsec >= time_end.tv_nsec)
		return (1);
	return (0);
}

void	exec_cmd_client(t_serveur *serv)
{
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		while (p_client->list_cmds)
		{
			if (p_client->list_cmds->time_end.tv_sec == 0) // start
			{
				clock_gettime(CLOCK_REALTIME, &p_client->list_cmds->time_end);
				//printf(KYEL "now: %lu %lu\n" KRESET, p_client->list_cmds->time_end.tv_sec, p_client->list_cmds->time_end.tv_nsec);
				p_client->list_cmds->time_end.tv_nsec += (serv->world_hdl.t_unit - (int)serv->world_hdl.t_unit) * 1000000000;
				if (p_client->list_cmds->time_end.tv_nsec > 1000000000)
				{
					p_client->list_cmds->time_end.tv_nsec -= 1000000000;
					p_client->list_cmds->time_end.tv_sec += 1;	
				}
				p_client->list_cmds->time_end.tv_sec += p_client->list_cmds->duration_cmd * serv->world_hdl.t_unit;
				//printf(KYEL "end: %lu %lu\n" KRESET, p_client->list_cmds->time_end.tv_sec, p_client->list_cmds->time_end.tv_nsec);
			}
			if (timespec_is_over(p_client->list_cmds->time_end) == 1)//if time commande is terminated
			{
				//execute cmd and delete cmd_entity
				struct timespec	now;
				clock_gettime(CLOCK_REALTIME, &now);
				printf(KRED "lag %lus %luns\n" KRESET, now.tv_sec - p_client->list_cmds->time_end.tv_sec, now.tv_nsec - p_client->list_cmds->time_end.tv_nsec);
				p_client->list_cmds->func(serv, p_client, p_client->list_cmds->param);
				if (p_client->list_cmds->param)
					free(p_client->list_cmds->param);
				p_client->list_cmds = p_client->list_cmds->next;
			}
			else //time not terminated
				break ;
		}
		p_client = p_client->next;
	}
}
