/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terrain_generation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/06 12:24:32 by aleung-c          #+#    #+#             */
/*   Updated: 2017/01/06 12:24:39 by aleung-c         ###   ########.fr       */
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
		printf(KRED "Error: Terrain width or height too small (%dx %dy)\n" KRESET,
			serv->world_hdl.map_x, serv->world_hdl.map_y);
		exit (-1);
	}
	
	allocate_world_board(&serv->world_hdl);

	set_world_board_cases(&serv->world_hdl);

	generate_ressources(&serv->world_hdl);

	//print_world_board(&serv->world_hdl); 

	printf(KGRN "Terrain generated successfully.\n" KRESET);

}

void	allocate_world_board(t_world_hdl *world_hdl)
{
	int y;

	y = 0;
	world_hdl->world_board =
		(t_world_case **)malloc(sizeof(t_world_case *) * world_hdl->map_y);
	while (y < world_hdl->map_y)
	{
		world_hdl->world_board[y] =
			(t_world_case *)malloc(sizeof(t_world_case) * world_hdl->map_x);
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
			world_hdl->world_board[y][x].players = NULL;
			world_hdl->world_board[y][x].ressources.food = 0;
			world_hdl->world_board[y][x].ressources.linemate = 0;
			world_hdl->world_board[y][x].ressources.deraumere = 0;
			world_hdl->world_board[y][x].ressources.sibur = 0;
			world_hdl->world_board[y][x].ressources.mendiane = 0;
			world_hdl->world_board[y][x].ressources.phiras = 0;
			world_hdl->world_board[y][x].ressources.thystame = 0;
			x++;
		}
		y++;
		x = 0;
	}
}

/*
**	This is where the ressources are actually put into the cases.
**	For now, it is a flat random.
**	TODO: Add logical patterns (bioms?) to ressources generation.
*/

void	generate_ressources(t_world_hdl *world_hdl)
{
	int		y;
	int		x;

	y = 0;
	x = 0;
	srand(time(NULL));
	while (y < world_hdl->map_y)
	{
		while (x < world_hdl->map_x)
		{
			world_hdl->world_board[y][x].ressources.food = rand() % 10;
			world_hdl->world_board[y][x].ressources.linemate = rand() % 10;
			world_hdl->world_board[y][x].ressources.deraumere = rand() % 10;
			world_hdl->world_board[y][x].ressources.sibur = rand() % 10;
			world_hdl->world_board[y][x].ressources.mendiane = rand() % 10;
			world_hdl->world_board[y][x].ressources.phiras = rand() % 10;
			world_hdl->world_board[y][x].ressources.thystame = rand() % 10;
			x++;
		}
		y++;
		x = 0;
	}
}

void	print_world_board(t_world_hdl *world_hdl)
{
	int		y;
	int		x;
	int		case_number;

	y = 0;
	x = 0;
	case_number = 0;
	printf(KCYN "World board:\n" KRESET);
	while (y < world_hdl->map_y)
	{
		while (x < world_hdl->map_x)
		{
			printf (KYEL "Case %d : %dx %dy\n" KRESET, case_number, x, y);
			printf(" {food %d} {linemate %d} {deraumere %d} {sibur %d} {mendiane %d} {phiras %d} {thystame %d}\n",
				world_hdl->world_board[y][x].ressources.food,
				world_hdl->world_board[y][x].ressources.linemate,
				world_hdl->world_board[y][x].ressources.deraumere,
				world_hdl->world_board[y][x].ressources.sibur,
				world_hdl->world_board[y][x].ressources.mendiane,
				world_hdl->world_board[y][x].ressources.phiras,
				world_hdl->world_board[y][x].ressources.thystame);
			x++;
			case_number++;
		}
		y++;
		x = 0;
	}
}
