/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_hdl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 14:35:02 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 15:36:19 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

t_client_entity	*create_client(t_socket sock)
{
	t_client_entity	*new_client;

	new_client = (t_client_entity *)s_malloc(sizeof(t_client_entity));
	memset((void *)new_client, 0, sizeof(t_client_entity));
	new_client->sock = sock;
	new_client->is_in_game = 0;
	new_client->is_gfx = 0;
	new_client->is_player_dead = 0;
	new_client->buff_recv.len = 0;
	new_client->buff_recv.overflow = NULL;
	new_client->buff_recv.len_overflow = 0;
	new_client->buff_send.len = 0;
	new_client->buff_send.overflow = NULL;
	new_client->buff_send.len_overflow = 0;
	set_client_player_datas(new_client);
	return (new_client);
}

void			set_client_player_datas(t_client_entity *new_client)
{
	int	res_i;

	res_i = 0;
	new_client->player.level = 1;
	new_client->player.pos.x = 5;
	new_client->player.pos.y = 5;
	new_client->player.dir = UP;
	new_client->player.nb_see_case = 4;
	new_client->player.cur_case = NULL;
	new_client->player.is_incanting = 0;
	new_client->player.is_incanter = 0;
	new_client->player.incantation_id = -1;
	while (res_i < 7)
	{
		new_client->player.inventory[res_i] = 0;
		res_i++;
	}
	get_time(&new_client->delay_time);
	new_client->player.inventory[FOOD] = 10;
	new_client->player.next_dinner_time.tv_sec = 0;
	new_client->player.next_dinner_time.tv_nsec = 0;
}

void			add_client(t_serveur *serv, t_client_entity *client)
{
	client->next = serv->client_hdl.list_clients;
	serv->client_hdl.list_clients = client;
	serv->client_hdl.nb_clients += 1;
}

/*
**	Remove client node from server's client list.
*/

void			remove_client(t_serveur *serv, t_client_entity *client)
{
	t_client_entity	*current;
	t_client_entity	*prev;

	if (client->team)
		client->team->available_slots += 1;
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
		return (perror("remove_client(): client not found"));
	prev->next = client->next;
	free(client);
	serv->client_hdl.nb_clients -= 1;
}
