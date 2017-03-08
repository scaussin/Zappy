/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:32:33 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 17:33:05 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

int			check_size_read(t_client_entity *client)
{
	int		size_read;

	size_read = BUFF_SIZE - client->buff_recv.len;
	if (size_read == 0)
		perror("read buffer full");
	return (size_read);
}
