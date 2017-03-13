/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terrain_generation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/06 12:24:32 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/03 19:16:50 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Allocating each case of the terrain and putting items in it.
*/

void	init_terrain(t_serveur *serv)
{
	printf(KCYN "- Generating terrain:\n" KRESET);
	printf("World x width: %d\nWorld y height: %d\n",
		serv->world_hdl.map_x, serv->world_hdl.map_y);
	if (serv->world_hdl.map_x < 1 || serv->world_hdl.map_y < 1)
	{
		printf(KRED "Error: Terrain width or height too small (%dx %dy)\n"
			KRESET, serv->world_hdl.map_x, serv->world_hdl.map_y);
		exit(-1);
	}
	allocate_world_board(&serv->world_hdl);
	set_world_board_cases(&serv->world_hdl);
	generate_ressources(serv, &serv->world_hdl);
	generate_ressources_name(&serv->world_hdl);
	printf(KGRN "Terrain generated successfully.\n" KRESET);
}

void	generate_ressources_name(t_world_hdl *world_hdl)
{
	world_hdl->name_ressources[0] = "nourriture";
	world_hdl->name_ressources[1] = "linemate";
	world_hdl->name_ressources[2] = "deraumere";
	world_hdl->name_ressources[3] = "sibur";
	world_hdl->name_ressources[4] = "mendiane";
	world_hdl->name_ressources[5] = "phiras";
	world_hdl->name_ressources[6] = "thystame";
}

void	allocate_world_board(t_world_hdl *world_hdl)
{
	int y;

	y = 0;
	world_hdl->world_board =
		(t_world_case **)s_malloc(sizeof(t_world_case *) * world_hdl->map_y);
	while (y < world_hdl->map_y)
	{
		world_hdl->world_board[y] =
			(t_world_case *)s_malloc(sizeof(t_world_case) * world_hdl->map_x);
		y++;
	}
}

/*
**	Init each world board cases ressources and players on it to zero.
*/

void	set_world_board_cases(t_world_hdl *world_hdl)
{
	int		y;
	int		x;

	y = 0;
	x = 0;
	while (y < world_hdl->map_y)
	{
		while (x < world_hdl->map_x)
		{
			world_hdl->world_board[y][x].ressources[FOOD] = 0;
			world_hdl->world_board[y][x].ressources[LINEMATE] = 0;
			world_hdl->world_board[y][x].ressources[DERAUMERE] = 0;
			world_hdl->world_board[y][x].ressources[SIBUR] = 0;
			world_hdl->world_board[y][x].ressources[MENDIANE] = 0;
			world_hdl->world_board[y][x].ressources[PHIRAS] = 0;
			world_hdl->world_board[y][x].ressources[THYSTAME] = 0;
			world_hdl->world_board[y][x].nb_players = 0;
			x++;
		}
		y++;
		x = 0;
	}
}

/*
**	This is where the ressources are actually put into the cases.
**	TODO: Add logical patterns (bioms?) to ressources generation.
*/

void	generate_ressources(t_serveur *serv, t_world_hdl *world_hdl)
{
	int		y;
	int		x;

	(void)serv;
	y = 0;
	x = 0;
	srand(time(NULL));
	while (y < world_hdl->map_y)
	{
		while (x < world_hdl->map_x)
		{
			generate_ressources_diced(world_hdl, x, y);
			x++;
		}
		y++;
		x = 0;
	}
}
