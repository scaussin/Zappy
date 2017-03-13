/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 12:27:58 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 12:28:00 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	At this point, the values are format OK. it is just about filling
**	and checking their meaning.
*/

void	fill_input(t_serveur *serv, int argc, char **argv)
{
	int i;

	i = 1;
	while (i != argc && i < 7)
	{
		if (i == 2)
			serv->network.port = strtol(argv[i], NULL, 10);
		if (i == 4)
			serv->world_hdl.map_x = strtol(argv[i], NULL, 10);
		if (i == 6)
			serv->world_hdl.map_y = strtol(argv[i], NULL, 10);
		i++;
	}
	i = 8;
	serv->team_hdl.array_teams = (t_team_entity *)malloc(
		sizeof(t_team_entity) * serv->team_hdl.nb_teams);
	fill_input_part_2(serv, &i, argv, 0);
	serv->network.max_clients = serv->team_hdl.nb_teams
		* serv->team_hdl.nb_teams_slots;
}

void	fill_input_part_2(t_serveur *serv, int *i, char **argv, int team_i)
{
	if (*i == 8)
	{
		while (argv[*i] && strncmp(argv[*i], "-c", 2) != 0
			&& strncmp(argv[*i], "-t", 2) != 0)
		{
			if (strlen(argv[*i]) > 40)
				error_in_args(*i, "Team Name is too long (40 char max)");
			serv->team_hdl.array_teams[team_i].name =
				(char *)malloc(sizeof(char) * strlen(argv[*i]) + 1);
			strncpy(serv->team_hdl.array_teams[team_i].name, argv[*i],
				strlen(argv[*i]));
			serv->team_hdl.array_teams[team_i].name[strlen(argv[*i])] = '\0';
			team_i++;
			(*i)++;
		}
		if (argv[*i] && strncmp(argv[*i], "-c", 2) == 0)
			serv->team_hdl.nb_teams_slots = strtol(argv[*i + 1], NULL, 10);
		(*i) += 2;
		if (argv[*i] && strncmp(argv[*i], "-t", 2) == 0)
		{
			check_time_input_limit(serv, argv[*i + 1]);
		}
	}
}

void	error_in_args(int pos, char *str)
{
	printf(KRED "Error: col %d: %s\n" KRESET, pos, str);
	printf(KMAG "Usage: ./serveur -p <port> -x <width> -y <height>");
	printf(" -n <team> [<team>] [<team>] ... -c <nb> -t <t>\n" KRESET);
	exit(-1);
}

int		regex_match(char *string_to_search, char *regex_str)
{
	int				err;
	int				match;
	regex_t			regex;

	err = regcomp(&regex, regex_str, REG_NOSUB | REG_EXTENDED);
	if (err)
	{
		printf("Could not compile regex.\n");
		exit(1);
	}
	else
	{
		match = regexec(&regex, string_to_search, 0, NULL, 0);
		regfree(&regex);
		if (match == 0)
			return (1);
		else if (match == REG_NOMATCH)
			return (0);
		return (-1);
	}
	return (0);
}

void	print_input_values(t_serveur *serv)
{
	int i;
	int y;

	i = 0;
	y = 0;
	printf(KGRN "Server settings:\n" KRESET);
	printf("Port: %d\n", serv->network.port);
	printf("Map width: %d\n", serv->world_hdl.map_x);
	printf("Map height: %d\n", serv->world_hdl.map_y);
	printf("Nb of teams: %d\nTeam names:", serv->team_hdl.nb_teams);
	while (i < serv->team_hdl.nb_teams)
	{
		printf("  [%s]", serv->team_hdl.array_teams[i].name);
		serv->team_hdl.array_teams[i].cur_nb_players = 0;
		while (y < MAX_LV)
		{
			serv->team_hdl.array_teams[i].nb_players_per_lv[y] = 0;
			y++;
		}
		y = 0;
		i++;
	}
	printf("\nClients per Team: %d\n", serv->team_hdl.nb_teams_slots);
	printf("t: %f\n\n", serv->world_hdl.t_unit);
}
