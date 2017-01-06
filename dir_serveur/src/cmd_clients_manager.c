#include "../includes/serveur.h"

void	manage_cmd_clients(t_serveur *serv, t_lexer *lexer_tab)
{
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->buff_recv.len > 0)
			exec_cmd_client(p_client);
		p_client = p_client->next;
	}
}

void	exec_cmd_client(t_client_entity *client)
{
	char	*buff;

	buff = read_buffer(&client->buff_recv);
	printf("exec cmd : %s\n", buff);
	client->buff_recv.len = 0;
	free(buff);
}

//buff_recv to t_list_cmds_entity
void	lexer(t_client_entity *client, t_lexer *lexer_tab)
{
	char *buff;
	char *cmd;

	buff = read_buffer(&client->buff_recv);	

	cmd = strtok(buff, END);
	while (cmd != NULL)
	{
		match_lexer();
		cmd = strtok(NULL, END);
	}
	//strrchr()
	if (buff)
		free(buff);
}

void	match_lexer(t_lexer *lexer_tab, t_client_entity *client, char *cmd)
{
	int i = 0;

	while (i < SIZE_LEXER_TAB)
	{
		if (strcmp(lexer_tab[i].name, cmd) == 0)
		{
			add_cmd(client->list_cmds);
		}
		i++;
	}
}

void	add_cmd(t_client_entity *client, t_lexer *cmd, char *param)
{
	t_list_cmds_entity *lst_tmp;

	if (!client->list_cmds)
	{
		client->list_cmds = s_malloc(sizeof(t_list_cmds_entity));
		client->list_cmds->func = cmd->func;
		client->list_cmds->param = strdup(param);
		client->list_cmds->next = NULL;
	}

	lst_tmp = client->list_cmds;
	while (lst_tmp && lst_tmp->next)
	{
		lst_tmp = lst_tmp->next;
	}
	lst_tmp->next
	client->size_list_cmds += 1;
}

void	cmd_avance(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	write_buffer(&client_cur->buff_send, "OK\n", 3);
}

void	cmd_droite(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	write_buffer(&client_cur->buff_send, "OK\n", 3);
}