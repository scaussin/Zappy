#include "../includes/serveur.h"

/*
**	For each client, check if there is something to read on the socket.
**	If there is, put the content into the client's circular buffer.
**
**	Also check if there is something to read on their buffers.
*/

void		check_all_clients_communication(t_serveur *serv)
{
	t_client_entity		*p_client;
	int					ret_read;
	char				*msg;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (FD_ISSET(p_client->sock, serv->network.read_fs)
			&& !(ret_read = read_client(p_client)))
		{
			// check gfx clear
			if (p_client->is_gfx == 1)
				serv->client_hdl.gfx_client = NULL;
			if (p_client->is_in_game == 1)
			{
				// gfx msg : "pdi #n\n"
				asprintf(&msg, "pdi #%d\n",
					p_client->sock);
				push_gfx_msg(serv, msg);
				free(msg);
			}
			disconnect_client(p_client->sock);
			remove_client(serv, p_client);
			return ;
		}
		if (FD_ISSET(p_client->sock, serv->network.write_fs))
			write_client(p_client);
		p_client = p_client->next;
	}
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
			// check gfx clear
			if (p_client->is_gfx == 1)
				serv->client_hdl.gfx_client = NULL;
			if (p_client->is_in_game == 1)
			{
				// gfx msg : "pdi #n\n"
				asprintf(&msg, "pdi #%d\n",
					p_client->sock);
				push_gfx_msg(serv, msg);
				free(msg);
			}
			disconnect_client(p_client->sock);
			remove_client(serv, p_client);
			p_client = serv->client_hdl.list_clients;
		}
		if (p_client) // protection against solo client disconnecting.
			p_client = p_client->next;
	}
}

/*
**	Write on client socket with recv if select() caught modifications.
*/

void		write_client(t_client_entity *client)
{
	char	*buff_tmp;
	int		ret_send;

	buff_tmp = read_buffer(&client->buff_send);
	while (1)
	{
		ret_send = send(client->sock, buff_tmp, client->buff_send.len + client->buff_send.len_overflow, 0);
		if (ret_send == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}
	print_send(client->sock, buff_tmp, client->buff_send.len + client->buff_send.len_overflow);
	if (buff_tmp)
		free(buff_tmp);
	if (ret_send == -1)
		perror("send()");
	if (ret_send > 0)
		update_len_buffer(&client->buff_send, ret_send);
}

/*
**	Read the client socket with recv if select() caught modifications.
*/

int			read_client(t_client_entity *client)
{
	int		ret;
	int		size_read;
	char	*buff_tmp;

	size_read = BUFF_SIZE - client->buff_recv.len;
	if (size_read == 0)
	{
		perror("read buffer full");
		return (0);
	}
	buff_tmp = s_malloc(size_read);
	while (1)
	{
		ret = recv(client->sock, buff_tmp, size_read, 0);
		if (ret == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}	
	if (ret == -1)
	{
		perror("recv()");
	}
	else
	{
		ret = write_buffer(&client->buff_recv, buff_tmp, ret);
		print_receive(client->sock, buff_tmp, ret);
	}
	free(buff_tmp);
	return (ret);
}

/*
**	Write into an actual buffer -> preparing for the select() pass.
*/

int			write_buffer(t_buffer *buff, char *to_write, int size)
{
	int		i;

	if (size)
	{
		if (buff->len + size > BUFF_SIZE || buff->overflow) //buffer overflow
		{
			buff->overflow = realloc(buff->overflow, buff->len_overflow + size);
			memcpy(buff->overflow + buff->len_overflow, to_write, size);
			buff->len_overflow += size;
			printf("[WARNING] : buffer overflow\n");
		}
		else
		{
			i = 0;
			while (i < size)
			{
				buff->buff[(buff->start + buff->len + i) % BUFF_SIZE] = to_write[i];
				i++;
			}
			buff->len += i;
		}
	}
	return (size);
}

/*
**	Read from buffer -> will only get datas if select() caught modifications.
**	Else, it will return NULL.
*/

char		*read_buffer(t_buffer *buff)
{
	char	*ret_buff;
	int		i;

	ret_buff = NULL;
	if (buff->len > 0)
	{
		ret_buff = s_malloc(buff->len + 1);
		i = 0;
		while (i < buff->len)
		{
			ret_buff[i] = buff->buff[(buff->start + i) % BUFF_SIZE];
			i++;
		}
		ret_buff[buff->len] = 0;
	}
	if (buff->len_overflow > 0)
	{
		ret_buff = realloc(ret_buff, buff->len_overflow + buff->len + 1);
		memcpy(ret_buff + buff->len, buff->overflow, buff->len_overflow);
		ret_buff[buff->len_overflow + buff->len] = 0;
	}
	return (ret_buff);
}

/*
**	Update start and len of buffer, and manage buffer overflow.
**	if size == -1 -> erase buffer and buffer_overflow
*/

void	update_len_buffer(t_buffer *buff, int size)
{
	char	*tmp;

	if (size > 0)
	{
		if (buff->len > 0) // update buffer
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
		if (buff->len_overflow > 0 && size > 0) //update buffer overflow
		{
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
	}
	else if (size == -1) // clean all buffer
	{
		buff->len = 0;
		free(buff->overflow);
		buff->overflow = NULL;
		buff->len_overflow = 0;
	}
}


/*
**	Read buffer until first \n and return cmd. Updates 'start' and 'len' variables.
**	Returned pointer must be freed.
*/

char		*get_first_cmd(t_buffer *buffer)
{
	char *buff;
	char *end;
	int len_cmd;

	end = NULL;
	if ((buff = read_buffer(buffer)) && (end = memchr(buff, CHAR_END, buffer->len)))
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
