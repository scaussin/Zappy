/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_voir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpuccine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 18:49:18 by rpuccine          #+#    #+#             */
/*   Updated: 2017/03/02 18:49:27 by rpuccine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

int		on_start_cmd_voir(t_serveur *serv, t_client_entity *client_cur,
	char *param)
{
	(void)serv;
	if (param)
	{
		printf(KMAG "Bad format to cmd [voir] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	return (0);
}

void	on_end_cmd_voir(t_serveur *serv, t_client_entity *client_cur,
	char *param)
{
	cmd_voir(serv, client_cur, param);
}

t_pos	*get_see_case_coordinates(t_serveur *serv, t_player *player)
{
	t_pos	*tab_pos;
	t_pos	rel_pos;
	int		i_line;
	int		i_case;
	int		i_tab;

	tab_pos = (t_pos *)s_malloc(player->nb_see_case * sizeof(t_pos));
	i_line = 0;
	i_tab = 0;
	while (i_line <= player->level)
	{
		i_case = 0;
		rel_pos.x = i_line * -1;
		rel_pos.y = i_line;
		while (i_case < 2 * i_line + 1)
		{
			fill_tab(tab_pos + i_tab, &rel_pos, player, serv);
			i_case++;
			i_tab++;
			rel_pos.x++;
		}
		i_line++;
	}
	return (tab_pos);
}

int		get_nb_case(int level)
{
	int	total;
	int	i;

	total = 0;
	i = -1;
	while (++i <= level)
		total += 2 * i + 1;
	return (total);
}

void	fill_tab(t_pos *abs_pos, t_pos *rel_pos, t_player *player,
	t_serveur *serv)
{
	int	world_width;
	int	world_height;

	world_width = serv->world_hdl.map_x;
	world_height = serv->world_hdl.map_y;
	if (player->dir == UP)
	{
		abs_pos->x = modulo(player->pos.x + rel_pos->x, world_width);
		abs_pos->y = modulo(player->pos.y + rel_pos->y, world_height);
	}
	else if (player->dir == RIGHT)
	{
		abs_pos->x = modulo(player->pos.x + rel_pos->y, world_width);
		abs_pos->y = modulo(player->pos.y - rel_pos->x, world_height);
	}
	else if (player->dir == DOWN)
	{
		abs_pos->x = modulo(player->pos.x - rel_pos->x, world_width);
		abs_pos->y = modulo(player->pos.y - rel_pos->y, world_height);
	}
	else if (player->dir == LEFT)
	{
		abs_pos->x = modulo(player->pos.x - rel_pos->y, world_width);
		abs_pos->y = modulo(player->pos.y + rel_pos->x, world_height);
	}
}
