/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 11:54:52 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 11:54:54 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Usage: ./serveur -p <port> -x <width> -y <height>
**			-n <team> [<team>] [<team>] ... -c <nb> -t <t>
**
**	-p numero de port
**	-x largeur du Monde
**	-y hauteur du Monde
**	-n nom\_equipe\_1 nom\_\_equipe_2 ...
**	-c nombre de client autorises au commencement du jeu
**	-t diviseur de l'unite de temps (plus t est grand, plus le jeu va vite)
*/

void	get_input(t_serveur *serv, int argc, char **argv)
{
	if (argc < 13)
		error_in_args(0, "Invalid number of arguments");
	else
	{
		check_input_format(serv, argc, argv);
		fill_input(serv, argc, argv);
		parse_filled_input(serv);
		print_input_values(serv);
	}
}

/*
**	For each option, check if their format is OK.
**	We will check later for their meaning.
*/

void	check_input_format(t_serveur *serv, int argc, char **argv)
{
	int i;

	i = 1;
	while (i != argc && i < 8)
	{
		if (i == 1 && strncmp(argv[i], "-p", 2) != 0)
			error_in_args(i, "invalid argument -p");
		if (i == 2 && !regex_match(argv[i], "^[0-9]+$"))
			error_in_args(i, "invalid port number format");
		if (i == 3 && strncmp(argv[i], "-x", 2) != 0)
			error_in_args(i, "invalid map width specifier (-x)");
		if (i == 4 && !regex_match(argv[i], "^[0-9]+$"))
			error_in_args(i, "invalid map width format");
		if (i == 5 && strncmp(argv[i], "-y", 2) != 0)
			error_in_args(i, "invalid map height specifier (-y)");
		if (i == 6 && !regex_match(argv[i], "^[0-9]+$"))
			error_in_args(i, "invalid map height format");
		if (i == 7 && strncmp(argv[i], "-n", 2) != 0)
			error_in_args(i, "invalid team names specifier (-n)");
		i++;
	}
	if (i >= 8)
		get_input_team_names_format(serv, &i, argv);
}

void	get_input_team_names_format(t_serveur *serv, int *i, char **argv)
{
	if (*i == 8 && strncmp(argv[*i], "-c", 2) == 0)
		error_in_args(*i, "at least one team name is required");
	while (argv[*i] && strncmp(argv[*i], "-c", 2) != 0
		&& strncmp(argv[*i], "-t", 2) != 0)
	{
		if (argv[*i] && !regex_match(argv[*i], "^[a-zA-Z0-9]+"))
			error_in_args(*i, "invalid team name format");
		serv->team_hdl.nb_teams += 1;
		(*i)++;
	}
	if (!argv[*i])
		error_in_args(*i, "missing required arguments");
	if (argv[*i] && strncmp(argv[*i], "-c", 2) == 0)
	{
		if (argv[*i + 1] && !regex_match(argv[*i + 1], "^[0-9]+$"))
			error_in_args(*i + 1, "invalid client number format");
		else if (!argv[*i + 1])
			error_in_args(*i + 1, "missing required arguments");
	}
	(*i) += 2;
	get_input_team_names_format_2(serv, i, argv);
}

void	get_input_team_names_format_2(t_serveur *serv, int *i, char **argv)
{
	(void)serv;
	if (argv[*i] && strncmp(argv[*i], "-t", 2) == 0)
	{
		if (argv[*i + 1] && !regex_match(argv[*i + 1], "^[0-9]+$"))
			error_in_args(*i + 1, "invalid time value format");
		else if (!argv[*i + 1])
			error_in_args(*i + 1, "missing required arguments");
	}
}

/*
**	After we filled the values in our variables, we still have to check for
**	their meaning, or their limits.
*/

void	parse_filled_input(t_serveur *serv)
{
	if (serv->network.port < 1023 || serv->network.port > 65535)
		error_in_args(2, "Invalid port number [1023 <=> 65535]");
	if (serv->world_hdl.map_x * serv->world_hdl.map_y > MAX_MAP_AREA)
		error_in_args(4, "Map area is too big [map_x * map_y > 100000]");
	if (serv->team_hdl.nb_teams > MAX_NB_OF_TEAM)
		error_in_args(8, "Too many teams in game [Max 10]");
	if (serv->world_hdl.t_unit > MAX_GAME_TIME_UNIT)
		error_in_args(0, "Time unit too big [Max 1000]");
	if (serv->team_hdl.nb_teams_slots > MAX_NB_SLOT)
		error_in_args(0, "Too much starting slot [Max 20]");
}
