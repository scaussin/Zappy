/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terrain_generation_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 12:27:15 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 12:27:16 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

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

	dice = rand() % 100;
	if (dice < 70)
		world_hdl->world_board[y][x].ressources[FOOD] = rand() % 40;
	dice = rand() % 100;
	if (dice < 60)
		world_hdl->world_board[y][x].ressources[LINEMATE] = rand() % 8;
	dice = rand() % 100;
	if (dice < 50)
		world_hdl->world_board[y][x].ressources[DERAUMERE] = rand() % 8;
	dice = rand() % 100;
	if (dice < 40)
		world_hdl->world_board[y][x].ressources[SIBUR] = rand() % 6;
	dice = rand() % 100;
	if (dice < 30)
		world_hdl->world_board[y][x].ressources[MENDIANE] = rand() % 5;
	dice = rand() % 100;
	if (dice < 20)
		world_hdl->world_board[y][x].ressources[PHIRAS] = rand() % 3;
	dice = rand() % 100;
	if (dice < 15)
		world_hdl->world_board[y][x].ressources[THYSTAME] = rand() % 2;
}

/*
**	Utility function to display the content of each case of the world board.
**	Please enter 0 in x and y (norme);
*/

void	print_world_board(t_world_hdl *world_hdl, int x, int y)
{
	int		case_number;

	case_number = 0;
	printf(KCYN "World board:\n" KRESET);
	while (y < world_hdl->map_y)
	{
		while (x < world_hdl->map_x)
		{
			printf(KYEL "Case %d : %dx %dy\n" KRESET, case_number, x, y);
			printf(" {food %d} {linemate %d} {deraumere %d} {sibur %d} "
				"{mendiane %d} {phiras %d} {thystame %d}\n",
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
