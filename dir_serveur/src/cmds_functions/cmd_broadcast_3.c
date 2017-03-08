/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 16:12:26 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 16:12:28 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

void	prov_son_bas_droite(int *orient, t_pos taille, t_pos source, t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (a <= 2 * taille.x && b >= -taille.y)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		a = a + taille.x;
		b = b - taille.y;
	}
}

void	prov_son_droite(int *orient, t_pos taille, t_pos source, t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (a <= 2 * taille.x)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		a = a + taille.x;
	}
}
