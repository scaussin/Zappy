/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   team_hdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 16:48:12 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 19:14:38 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

t_team_entity	*get_team_by_name(t_serveur *serv, char *name)
{
	int				i;
	t_team_entity	*current_team;

	i = 0;
	while (i < serv->team_hdl.nb_teams)
	{
		current_team = serv->team_hdl.array_teams + i;
		if (strncmp(current_team->name, name, strlen(current_team->name)) == 0
			&& strlen(name) - 1 == strlen(current_team->name))
		{
			return (current_team);
		}
		i++;
	}
	return (NULL);
}
