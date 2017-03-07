/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_voir_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpuccine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 18:49:46 by rpuccine          #+#    #+#             */
/*   Updated: 2017/03/02 18:49:51 by rpuccine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

char	*get_players(char *see_str, t_pos *pos, int num, t_world_case **world)
{
	int		i;
	int		x;
	int		y;
	int		nb_players;
	int		multi_ressources;

	y = pos[num].y;
	x = pos[num].x;
	nb_players = (!num ? world[y][x].nb_players - 1 : world[y][x].nb_players);
	i = -1;
	multi_ressources = 0;
	while (++i < nb_players)
	{
		if (multi_ressources)
			see_str = str_concat_realloc1(see_str, " ");
		else
			multi_ressources = 1;
		see_str = str_concat_realloc1(see_str, "joueur");
	}
	return (see_str);
}

char	*get_ressources(t_serveur *serv, int x, int y, char *see_str)
{
	t_world_case	**world;
	int				i;
	int				j;
	int				multi_ressources;

	world = serv->world_hdl.world_board;
	multi_ressources = 0;
	i = -1;
	while (++i < NB_RESSOURCES)
	{
		j = -1;
		while (++j < world[y][x].ressources[i])
		{
			if (multi_ressources)
				see_str = str_concat_realloc1(see_str, " ");
			else
				multi_ressources = 1;
			see_str = str_concat_realloc1(see_str,
				serv->world_hdl.name_ressources[i]);
		}
	}
	return (see_str);
}

char	*init_see_str(void)
{
	char	*see_str;

	see_str = (char *)s_malloc(sizeof(char) * 2);
	see_str[0] = '{';
	see_str[1] = '\0';
	return (see_str);
}

void	cmd_voir(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	t_world_case	**world;
	t_player		*player;
	t_pos			*tab_pos;
	int				i;
	char			*see_str;

	(void)param;
	player = &(client_cur->player);
	world = serv->world_hdl.world_board;
	tab_pos = get_see_case_coordinates(serv, player);
	see_str = init_see_str();
	i = -1;
	while (++i < player->nb_see_case)
	{
		see_str = get_players(see_str, tab_pos, i, world);
		see_str = get_ressources(serv, tab_pos[i].x, tab_pos[i].y, see_str);
		if (i != player->nb_see_case - 1)
			see_str = str_concat_realloc1(see_str, ", ");
		else
			see_str = str_concat_realloc1(see_str, "}\n");
	}
	write_buffer(&client_cur->buff_send, see_str, strlen(see_str));
	free(see_str);
	free(tab_pos);
}
