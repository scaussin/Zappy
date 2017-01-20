#include "../includes/serveur.h"

t_client_entity	*create_client(SOCKET sock)
{
	t_client_entity	*new_client;

	new_client = (t_client_entity *)s_malloc(sizeof(t_client_entity));
	memset((void *)new_client, 0, sizeof(t_client_entity));
	new_client->sock = sock;

	// set client program status variables.
	new_client->is_in_game = 0;
	new_client->is_gfx = 0;
	new_client->is_player_dead = 0;

	// set player game starting datas
	set_client_player_datas(new_client);
	return (new_client);
}

void			set_client_player_datas(t_client_entity *new_client)
{
	int res_i;

	res_i = 0;
	new_client->player.level = 0;
	new_client->player.pos.x = 5;
	new_client->player.pos.y = 5;
	new_client->player.dir = UP;
	new_client->player.cur_case = NULL;
	while (res_i < 7)
	{
		new_client->player.inventory[res_i] = 0;
		res_i++;
	}
	// Set life and death time.
	new_client->player.inventory[FOOD] = 10;
	// Dinner time will be set to a value when the player is authenticated.
	new_client->player.next_dinner_time.tv_sec = 0;
	new_client->player.next_dinner_time.tv_nsec = 0;
}

void			add_client(t_serveur *serv, t_client_entity *client)
{
	client->next = serv->client_hdl.list_clients;
	serv->client_hdl.list_clients = client;
}

void			remove_client(t_serveur *serv, t_client_entity *client)
{
	t_client_entity	*current;
	t_client_entity	*prev;

	if (client->team)
	{
		client->team->available_slots += 1;
	}
	if (serv->client_hdl.list_clients == client)
	{
		serv->client_hdl.list_clients = client->next;
		free(client);
		return ;
	}
	prev = serv->client_hdl.list_clients;
	current = serv->client_hdl.list_clients->next;
	while (current && current != client)
	{
		prev = current;
		current = current->next;
	}
	if (!current || current != client)
	{
		perror("remove_client(): client not found");
		return ;
	}
	prev->next = client->next;
	free(client);
}
