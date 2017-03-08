/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:56:38 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 15:56:41 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.h"

void	prov_son_haut(int *orient, t_pos taille, t_pos source, t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (b <= 2 * taille.y)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		b = b + taille.y;
	}
}

void	prov_son_haut_gauche(int *orient, t_pos taille, t_pos source,
								t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (a >= -taille.x && b <= 2 * taille.y)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		a = a - taille.x;
		b = b + taille.y;
	}
}

void	prov_son_gauche(int *orient, t_pos taille, t_pos source, t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (a >= -taille.x)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		a = a - taille.x;
	}
}

void	prov_son_bas_gauche(int *orient, t_pos taille, t_pos source,
								t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (a >= -taille.x && b >= -taille.y)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		a = a - taille.x;
		b = b - taille.y;
	}
}

void	prov_son_bas(int *orient, t_pos taille, t_pos source, t_pos dest)
{
	int			a;
	int			b;
	double		mindis;
	double		dist;

	a = dest.x;
	b = dest.y;
	mindis = 2 * taille.x * taille.y;
	while (b >= -taille.y)
	{
		if ((dist = distance(source.x, source.y, a, b)) < mindis)
		{
			mindis = dist;
			*orient = orientation(source.x, source.y, a, b);
		}
		b = b - taille.y;
	}
}
