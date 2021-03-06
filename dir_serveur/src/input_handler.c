#include "../includes/serveur.h"

/*
**	Usage: ./serveur -p <port> -x <width> -y <height> -n <team> [<team>] [<team>] ... -c <nb> -t <t>
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
	int i;
	int y;

	i = 0;
	y = 0;
	if (argc < 13)
		error_in_args(0, "Invalid number of arguments");
	else
	{
		//serv_settings->  = 0;
		check_input_format(serv, argc, argv);
		fill_input(serv, argc, argv);
		parse_filled_input(serv);

		// printing Server settings values.
		printf(KGRN "Server settings:\n" KRESET);
		printf("Port: %d\n", serv->network.port);
		printf("Map width: %d\n", serv->world_hdl.map_x);
		printf("Map height: %d\n", serv->world_hdl.map_y);
		printf("Nb of teams: %d\n", serv->team_hdl.nb_teams);
		printf("Team names:");
		while (i < serv->team_hdl.nb_teams)
		{
			printf("  [%s]", serv->team_hdl.array_teams[i].name);
			// set team number of players by lvl to zero -> teams are empty.
			while (y < MAX_LV)
			{
				serv->team_hdl.array_teams[i].nb_players_per_lv[y] = 0;
				y++;
			}
			y = 0;
			i++;
		}
		printf("\n");
		printf("Clients per Team: %d\n", serv->team_hdl.nb_teams_slots);
		printf("t: %f\n\n", serv->world_hdl.t_unit);
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
	// for each arg.
	while (i != argc && i < 8)
	{
		// port -p n
		if (i == 1 && strncmp(argv[i], "-p", 2) != 0)
			error_in_args(i, "invalid argument -p");
		if (i == 2 && !regex_match(argv[i], "^[0-9]+$"))
			error_in_args(i, "invalid port number format");
		// map width -x n
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

	// Handling multiple possible team names.
	if (i >= 8)
	{
		// if we have -c just after team names specifier.
		if (i == 8 && strncmp(argv[i], "-c", 2) == 0)
			error_in_args(i, "at least one team name is required");
		// we catch every teams.
		while (argv[i]
			&& strncmp(argv[i], "-c", 2) != 0
			&& strncmp(argv[i], "-t", 2) != 0)
		{
			if (argv[i] && !regex_match(argv[i], "^[a-zA-Z0-9]+"))
				error_in_args(i, "invalid team name format");
			serv->team_hdl.nb_teams += 1;
			i++;
		}
		if (!argv[i])
			error_in_args(i, "missing required arguments");
		if (argv[i] && strncmp(argv[i], "-c", 2) == 0)
		{
			if (argv[i + 1] && !regex_match(argv[i + 1], "^[0-9]+$"))
				error_in_args(i + 1, "invalid client number format");
			else if (!argv[i + 1])
				error_in_args(i + 1, "missing required arguments");
		}
		i += 2;
		if (argv[i] && strncmp(argv[i], "-t", 2) == 0)
		{
			if (argv[i + 1] && !regex_match(argv[i + 1], "^[0-9]+$"))
				error_in_args(i + 1, "invalid time value format");
			else if (!argv[i + 1])
				error_in_args(i + 1, "missing required arguments");
		}
	}
}

/*
**	At this point, the values are format OK. it is just about filling
**	and checking their meaning.
*/

void	fill_input(t_serveur *serv, int argc, char **argv)
{
	int i;
	int team_i;

	i = 1;
	team_i = 0;
	// for each arg.
	while (i != argc && i < 7)
	{
		// port -p n
		if (i == 2)
			serv->network.port = strtol(argv[i], NULL, 10);
		// map width -x n
		if (i == 4)
			serv->world_hdl.map_x = strtol(argv[i], NULL, 10);
		// map height -y n
		if (i == 6)
			serv->world_hdl.map_y = strtol(argv[i], NULL, 10);
		i++;
	}
	i = 8;
	// team names -n teamname [teamname] [teamname] ...
	serv->team_hdl.array_teams = (t_team_entity *)malloc(sizeof(t_team_entity)
		* serv->team_hdl.nb_teams);
	if (i == 8)
	{
		while (argv[i]
		&& strncmp(argv[i], "-c", 2) != 0
		&& strncmp(argv[i], "-t", 2) != 0)
		{
			// -------- Init new team 
			if (strlen(argv[i]) > 40)
				error_in_args(i, "Team Name is too long (40 char max)");
			serv->team_hdl.array_teams[team_i].name =
				(char *)malloc(sizeof(char) * strlen(argv[i]) + 1);
			strncpy(serv->team_hdl.array_teams[team_i].name, argv[i], strlen(argv[i]));
			serv->team_hdl.array_teams[team_i].name[strlen(argv[i])] = '\0';
			team_i++;
			i++;
		}

		// get slot number.
		if (argv[i] && strncmp(argv[i], "-c", 2) == 0)
			serv->team_hdl.nb_teams_slots = strtol(argv[i + 1], NULL, 10);
		i += 2;
		// get time unit.
		if (argv[i] && strncmp(argv[i], "-t", 2) == 0)
		{
			serv->world_hdl.t_unit = strtol(argv[i + 1], NULL, 10);
			serv->world_hdl.t_unit = 1 / serv->world_hdl.t_unit;
		}
	}
	serv->network.max_clients = serv->team_hdl.nb_teams
		* serv->team_hdl.nb_teams_slots;
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
