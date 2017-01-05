#include "../includes/serveur.h"

void	manage_cmd_clients(t_serveur *serv)
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
/*void	lexer(t_client_entity *client)
{

}*/