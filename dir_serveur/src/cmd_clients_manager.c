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
	int		i = 0;
	char	*arg_cmd;
	char	*cmd_end;
	int		nb_of_parsed_chars;

	// Check for arguments
	cmd_end = strchr(cmd, '\n');
	arg_cmd = NULL;
	if ((arg_cmd = strchr(cmd, ' ')))
		nb_of_parsed_chars = arg_cmd - cmd;
	else
		nb_of_parsed_chars = strlen(cmd);

	if (nb_of_parsed_chars > 12)
	{
		printf(KMAG "[Serveur]: cmd too long (no cmd takes more than 12 char):"
					"%s on sock: %d\n" KRESET, cmd, client->sock);
		return ;
	}

	// Parsing
	while (i < SIZE_CMD_MATCH_TABLE)
	{
		if (strncmp(cmd_match_table[i].name, cmd, nb_of_parsed_chars) == 0)
		{
			add_cmd(client, &cmd_match_table[i], cmd);
			return ;
		}
		i++;
	}
	printf(KMAG "[Serveur]: Unknown command: %s on sock: %d\n" KRESET, cmd, client->sock);
	return ;
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

/*
**	Changed: for each player, executes the cmd then put the client in "delay state".
**	Next cmd will be executed when delay_time is over.
*/

struct timespec	*exec_cmd_client(t_serveur *serv)
{
	t_client_entity			*p_client;
	struct timespec			*lower_time_end;
	t_list_cmds_entity		*cmd_to_free;

	lower_time_end = NULL;
	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		// execute cmd and delete cmd_entity
		if (p_client->list_cmds && timespec_is_over(p_client->delay_time))
		{
			// reset time for next command -> "delay state" for the client.
			get_time(&p_client->delay_time);
			add_nsec_to_timespec(&p_client->delay_time,
				p_client->list_cmds->duration_cmd * serv->world_hdl.t_unit * 1000000000);

			// function may cancel the timer reset, thats why it must be executed last.
			p_client->list_cmds->func(serv, p_client, p_client->list_cmds->param);

			// cleaning cmd from the cmd list for the player.
			if (p_client->list_cmds->param)
				free(p_client->list_cmds->param);
			cmd_to_free = p_client->list_cmds;
			p_client->list_cmds = p_client->list_cmds->next;
			p_client->size_list_cmds -= 1;
			free(cmd_to_free);
		}
		p_client = p_client->next;
	}
	return (lower_time_end);
}
