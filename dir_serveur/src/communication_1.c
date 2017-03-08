/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:16:38 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 17:33:07 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void		clean_all_buffer(t_buffer *buff)
{
	buff->len = 0;
	free(buff->overflow);
	buff->overflow = NULL;
	buff->len_overflow = 0;
}

/*
**	Update start and len of buffer, and manage buffer overflow.
**	if size == -1 -> erase buffer and buffer_overflow
*/

void		update_len_buffer(t_buffer *buff, int size)
{
	if (size > 0)
	{
		if (buff->len > 0)
		{
			if (buff->len < size)
			{
				size -= buff->len;
				buff->len = 0;
			}
			else
			{
				buff->len -= size;
				buff->start = (buff->start + size) % BUFF_SIZE;
			}
		}
		if (buff->len_overflow > 0 && size > 0)
			update_buffer_overflow(buff, size);
	}
	else if (size == -1)
		clean_all_buffer(buff);
}

void		update_buffer_overflow(t_buffer *buff, int size)
{
	char	*tmp;

	if (buff->len_overflow > size)
	{
		tmp = buff->overflow;
		buff->overflow = malloc(buff->len_overflow - size);
		memcpy(buff->overflow, tmp + size, buff->len_overflow - size);
		buff->len_overflow -= size;
		free(tmp);
	}
	else
	{
		free(buff->overflow);
		buff->overflow = NULL;
		buff->len_overflow = 0;
	}
}

/*
**	Read buffer until first \n and return cmd. Updates 'start' and 'len'
**	variables.
**	Returned pointer must be freed.
*/

char		*get_first_cmd(t_buffer *buffer)
{
	char	*buff;
	char	*end;
	int		len_cmd;

	end = NULL;
	if ((buff = read_buffer(buffer)) &&
		(end = memchr(buff, CHAR_END, buffer->len)))
	{
		end[LEN_END] = 0;
		len_cmd = (end - buff) + LEN_END;
		buffer->start = (buffer->start + len_cmd) % BUFF_SIZE;
		buffer->len -= len_cmd;
		return (buff);
	}
	if (end == NULL && buffer->len == BUFF_SIZE)
		update_len_buffer(buffer, -1);
	return (NULL);
}

/*
**	Will check if a client needs to be disconnected and removed.
**	If yes, we start the loop again to not miss any other
**	players disconnecting.
*/

void		disconnect_flagged_clients(t_serveur *serv)
{
	t_client_entity		*p_client;
	char				*msg;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (p_client->is_disconnecting == 1)
		{
			if (p_client->is_gfx == 1)
				serv->client_hdl.gfx_client = NULL;
			if (p_client->is_in_game == 1)
			{
				asprintf(&msg, "pdi #%d\n", p_client->sock);
				push_gfx_msg(serv, msg);
				free(msg);
			}
			disconnect_client(p_client->sock);
			remove_client(serv, p_client);
			p_client = serv->client_hdl.list_clients;
		}
		if (p_client)
			p_client = p_client->next;
	}
}
