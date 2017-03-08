/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:12:37 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 15:12:41 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

double	distance(int sourcex, int sourcey, int i, int j)
{
	double a;

	a = (sourcex - i) * (sourcex - i) + (sourcey - j) * (sourcey - j);
	return (sqrt(a));
}

int		orientation(int sourcex, int sourcey, int i, int j)
{
	if (abs(sourcex - i) > abs(sourcey - j))
	{
		if (sourcex > i)
			return (7);
		else
			return (3);
	}
	else if (abs(sourcey - j) > abs(sourcex - i))
	{
		if (sourcey > j)
			return (1);
		else
			return (5);
	}
	else if (abs(sourcey - j) == abs(sourcex - i))
		return (orientation_check_3(i, j, sourcex, sourcey));
	return (0);
}

int		orientation_check_3(int i, int j, int sourcex, int sourcey)
{
	if (sourcey >= j && sourcex >= i)
		return (8);
	else if (sourcey >= j && sourcex <= i)
		return (2);
	else if (sourcey <= j && sourcex <= i)
		return (4);
	else if (sourcey <= j && sourcex >= i)
		return (6);
	return (0);
}

int		provenance_son(t_serveur *serv, t_pos source, t_pos dest)
{
	int			orient;
	t_pos		taille;

	taille.x = serv->world_hdl.map_x;
	taille.y = serv->world_hdl.map_y;
	orient = 0;
	if (source.x == dest.x && source.y == dest.y)
		return (0);
	prov_son_haut_droite(&orient, taille, source, dest);
	prov_son_haut(&orient, taille, source, dest);
	prov_son_haut_gauche(&orient, taille, source, dest);
	prov_son_gauche(&orient, taille, source, dest);
	prov_son_bas_gauche(&orient, taille, source, dest);
	prov_son_bas(&orient, taille, source, dest);
	prov_son_bas_droite(&orient, taille, source, dest);
	prov_son_droite(&orient, taille, source, dest);
	return (orient);
}

void	prov_son_haut_droite(int *orient, t_pos taille, t_pos source,
								t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (a <= 2 * taille.x && b <= 2 * taille.y)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		a = a + taille.x;
		b = b + taille.y;
	}
}
