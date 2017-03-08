/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_connect_nbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:11:43 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 18:11:45 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

/*
**	La commande connect_nbr renvoie le nombre de connections autorisées et non
**	autorisées pour cette famille.
**
**	connaitre le nb de connections	| connect_nbr	| 0/t	|	valeur
**	non utilisée par l’équipe		|
*/

int		on_start_cmd_connect_nbr(t_serveur *serv, t_client_entity *client_cur,
									char *param)
{
	if (param)
	{
		printf(KMAG "Bad format to cmd [connect_nbr]"
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	cmd_connect_nbr(serv, client_cur, param);
	return (0);
}

void	on_end_cmd_connect_nbr(t_serveur *serv, t_client_entity *client_cur,
										char *param)
{
	(void)serv;
	(void)client_cur;
	(void)param;
}

void	cmd_connect_nbr(t_serveur *serv, t_client_entity *client_cur,
						char *param)
{
	char *client_msg;

	(void)serv;
	(void)param;
	asprintf(&client_msg, "%d\n", client_cur->team->available_slots);
	write_buffer(&client_cur->buff_send, client_msg, strlen(client_msg));
	free(client_msg);
}
