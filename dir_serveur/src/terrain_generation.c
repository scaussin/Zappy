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
	(void)	serv;

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

/*
**	This is the flat ressource generation. Just a simple random.
*/

void	generate_ressources_flat(t_world_hdl *world_hdl, int x, int y)
{
	world_hdl->world_board[y][x].ressources[FOOD] = (rand() % 10);
	world_hdl->world_board[y][x].ressources[LINEMATE] = rand() % 10;
	world_hdl->world_board[y][x].ressources[DERAUMERE] = rand() % 10;
	world_hdl->world_board[y][x].ressources[SIBUR] = rand() % 10;
	world_hdl->world_board[y][x].ressources[MENDIANE] = rand() % 10;
	world_hdl->world_board[y][x].ressources[PHIRAS] = rand() % 10;
	world_hdl->world_board[y][x].ressources[THYSTAME] = rand() % 10;
}

/*
**	This is diced ressource generation. Before generating a random, a dice is
**	cast to give more meaning to the generation.
**	This is a random of rarity.
*/

void	generate_ressources_diced(t_world_hdl *world_hdl, int x, int y)
{
	int		dice;

	dice = rand() % 100; // d100 cast.
	if (dice < 70) // 70% that case has food.
		world_hdl->world_board[y][x].ressources[FOOD] = rand() % 40;
	dice = rand() % 100;
	if (dice < 60) // 60% that case has linemate.
		world_hdl->world_board[y][x].ressources[LINEMATE] = rand() % 8;
	dice = rand() % 100;
	if (dice < 50) // 50% that case has deraumere.
		world_hdl->world_board[y][x].ressources[DERAUMERE] = rand() % 8;
	dice = rand() % 100;
	if (dice < 40) // 40% that case has sibur.
		world_hdl->world_board[y][x].ressources[SIBUR] = rand() % 6;
	dice = rand() % 100;
	if (dice < 30) // 30% that case has mendiane.
		world_hdl->world_board[y][x].ressources[MENDIANE] = rand() % 5;
	dice = rand() % 100;
	if (dice < 20) // 20% that case has phiras.
		world_hdl->world_board[y][x].ressources[PHIRAS] = rand() % 3;
	dice = rand() % 100;
	if (dice < 15) // 15% that case has thystame.
		world_hdl->world_board[y][x].ressources[THYSTAME] = rand() % 2;
}

/*
**	Add ressources to make more winning games. Aborted idea, too much problems
**	results from overloading cases.
*/

void	generate_added_ressources(t_serveur *serv, t_world_hdl *world_hdl, int x, int y)
{
	int		mult_coef;
	int		map_area_coef;

	mult_coef = (VICTORY_CDT_PLAYER_NB - serv->team_hdl.nb_teams_slots) + serv->team_hdl.nb_teams;
	map_area_coef = (world_hdl->map_x * world_hdl->map_y) / 50;
	mult_coef -= map_area_coef;
	if (mult_coef < 0)
	{
		mult_coef = 0;
	}
	world_hdl->world_board[y][x].ressources[FOOD] += (world_hdl->world_board[y][x].ressources[FOOD] * mult_coef);
	world_hdl->world_board[y][x].ressources[LINEMATE] += (world_hdl->world_board[y][x].ressources[LINEMATE] * mult_coef);
	world_hdl->world_board[y][x].ressources[DERAUMERE] += (world_hdl->world_board[y][x].ressources[DERAUMERE] * mult_coef);
	world_hdl->world_board[y][x].ressources[SIBUR] += (world_hdl->world_board[y][x].ressources[SIBUR] * mult_coef);
	world_hdl->world_board[y][x].ressources[MENDIANE] += (world_hdl->world_board[y][x].ressources[MENDIANE] * mult_coef);
	world_hdl->world_board[y][x].ressources[PHIRAS] += (world_hdl->world_board[y][x].ressources[PHIRAS] * mult_coef);
	world_hdl->world_board[y][x].ressources[THYSTAME] += (world_hdl->world_board[y][x].ressources[THYSTAME] * mult_coef);
}

/*
**	Utility function to display the content of each case of the world board.
*/

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
				world_hdl->world_board[y][x].ressources[FOOD],
				world_hdl->world_board[y][x].ressources[LINEMATE],
				world_hdl->world_board[y][x].ressources[DERAUMERE],
				world_hdl->world_board[y][x].ressources[SIBUR],
				world_hdl->world_board[y][x].ressources[MENDIANE],
				world_hdl->world_board[y][x].ressources[PHIRAS],
				world_hdl->world_board[y][x].ressources[THYSTAME]);
			x++;
			case_number++;
		}
		y++;
		x = 0;
	}
}
