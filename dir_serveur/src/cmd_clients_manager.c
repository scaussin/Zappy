#include "../includes/serveur.h"

void manage_cmd_clients(t_serveur *serv)
{
	t_client_entity	*p_client;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->len_buff > 0)
			get_cmd_client(serv, p_client);
		p_client = p_client->next;
	}
}

void get_cmd_client(t_serveur *serv, t_client_entity *client)
{
	int		i_loop;
	int		i_buff;
	char	*to_process;
	char	*to_print;

	to_process = (char *)s_malloc((BUFF_SIZE + 1) * sizeof(char));
	memset(to_process, 0, BUFF_SIZE + 1);
	i_loop = 0;
	i_buff = client->start_buff;
	while (i_loop < client->len_buff)
	{
		to_process[i_loop] = client->buff[i_buff % BUFF_SIZE];
		i_loop++;
		i_buff++;
	}
	client->start_buff = (i_buff % BUFF_SIZE);
	client->len_buff = 0;

	to_print = strdup(to_process);			//
	replace_nl(to_print);					//	Debug Print
	printf("cmd read : %s\n", to_print);	//
	/*printf("*state buffer :\n");
	write(1, client->buff, BUFF_SIZE);
	printf("*\n");*/

	if (strcmp(to_process, "VOIR\n") == 0)
	{
		printf("%sCommande VOIR requested%s\n", KYEL, KRESET);
		cmd_see(serv, client);
	}
	else
	{
		printf("%sCommande inconnue%s\n", KYEL, KRESET);
	}
}
