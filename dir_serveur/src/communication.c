#include "../includes/serveur.h"

void ckeck_all_clients_communication(t_serveur *serv)
{
	t_client_entity	*p_client;
	int ret_read;

	p_client = serv->client_hdl.list_clients;
	while (p_client)
	{
		if (FD_ISSET(p_client->sock, serv->network.read_fs)
			&& !(ret_read = read_client(p_client)))
		{
			disconnect_client(p_client->sock);
			remove_client(serv, p_client);
			return ;
		}
		if (FD_ISSET(p_client->sock, serv->network.write_fs))
			write_client(p_client);
		p_client = p_client->next;
	}
}

void		write_client(t_client_entity *client)
{
	char	*buff_tmp;
	int		ret_send;

	buff_tmp = read_buffer(&client->buff_send);
	while (1)
	{
		ret_send = send(client->sock, buff_tmp, client->buff_send.len, MSG_DONTWAIT);
		if (ret_send == -1 && (errno == EAGAIN || errno == EINTR))
			continue;
		else
			break ;
	}
	write(1, "send => [", 9);
	write(1, buff_tmp, client->buff_send.len);
	write(1, "]\n", 2);
	if (buff_tmp)
		free(buff_tmp);
	if (ret_send == -1)
		perror("send()");
	if (ret_send > 0)
	{
		client->buff_send.start = (client->buff_send.start + ret_send) % BUFF_SIZE;
		client->buff_send.len -= ret_send;
	}
}

int		read_client(t_client_entity *client)
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
		perror("recv()");
	else
		ret = write_buffer(&client->buff_recv, buff_tmp, ret);
	printf("recv -> ");
	print_buff(client->buff_recv);
	free(buff_tmp);
	return (ret);
}

int		write_buffer(t_buffer *buff, char *to_write, int size)
{
	int		i;

	if (size)
	{
		i = 0;
		if (buff->len + size > BUFF_SIZE)
		{
			perror("buffer full");
			return (0);
		}
		while (i < size)
		{
			buff->buff[(buff->start + buff->len + i) % BUFF_SIZE] = to_write[i];
			i++;
		}
		buff->len += i;
	}
	return (size);
}

char	*read_buffer(t_buffer *buff)
{
	char	*ret_buff;
	int		i;

	ret_buff = NULL;
	if (buff->len > 0)
	{
		ret_buff = s_malloc(buff->len + 1);
		bzero(ret_buff, buff->len + 1);
		i = 0;
		while (i < buff->len)
		{
			ret_buff[i] = buff->buff[(buff->start + i) % BUFF_SIZE];
			i++;
		}
	}
	return (ret_buff);
}

/*int   read_client(t_client_entity *client)
{
	int   ret;
	int   i_loop;
	int   i_buff;
	char  buffer[BUFF_SIZE + 1];

	ret = 0;
	memset(buffer, 0, BUFF_SIZE + 1);
	if ((ret = recv(client->sock, buffer, BUFF_SIZE, 0)) < 0)
	{
		perror("recv()");
		ret = 0;
	}
	else if (!ret)
		return (0);

	buffer[ret] = '\0';
	printf("Recu : %s\n", buffer);

	i_loop = 0;
	i_buff = client->start_buff;
	while (i_loop < ret)
	{
		client->buff[i_buff % BUFF_SIZE] = buffer[i_loop];
		i_loop++;
		i_buff++;
	}
	client->len_buff += ret;
	if (client->len_buff > BUFF_SIZE)
		client->len_buff = BUFF_SIZE;

	return(ret);
}*/
/*
t_team_entity	*get_team(t_serveur *serv, char *buff)
{
	char			*t_name;
	t_team_entity	*team;

	if (!(t_name = get_cmd_trim(buff)))
	{
		return (NULL);
	}

	team = get_team_by_name(serv, t_name);
	free(t_name);
	return (team);
}*/
/*
t_team_entity	*new_client_communication(t_serveur *serv, t_client_entity *client)
{
	char			*buff;
	char			*to_print;
	int				ret;
	t_team_entity	*team;

	buff = (char *)s_malloc(sizeof(char) * BUFF_SIZE);

	// Send Bienvenue
	if (send(client->sock, "BIENVENUE\n", strlen("BIENVENUE\n"), 0) < 0)
		exit_error("send()");
	printf("Send : BIENVENUE*\n");

	// Recv an verify Team Name
	if ((ret = recv(client->sock, buff, BUFF_SIZE - 1, 0)) < 0)
		exit_error("recv()");
	buff[ret] = '\0';					//
	to_print = strdup(buff);			//	FOR DEBUG PRINT
	replace_nl(to_print);				//
	printf("Recv : %s\n", to_print);	//
	if (!(team = get_team(serv, buff)))
	{
		printf("Get_team() failed\n");
		if (send(client->sock, "UNKNOWN TEAM\n",
			strlen("UNKNOWN TEAM\n"), 0) < 0)
			exit_error("send()");
		free(buff);
		free(to_print);
		return (NULL);
	}

	// Format Send buff and send
	memset((void *)buff, 0, BUFF_SIZE);
	sprintf(buff, "%d\n", team->available_slots);
	if (send(client->sock, buff, strlen(buff), 0) < 0)
		exit_error("send()");
	printf("Send : %d*\n", team->available_slots);

	memset((void *)buff, 0, BUFF_SIZE);
	sprintf(buff, "%d %d\n", serv->world_hdl.map_x, serv->world_hdl.map_y);
	if (send(client->sock, buff, strlen(buff), 0) < 0)
		exit_error("send()");
	printf("Send : %d %d*\n", serv->world_hdl.map_x, serv->world_hdl.map_y);

	free(buff);
	free(to_print);
	return (team);
}
*/