#include "../includes/serveur.h"

t_team_entity	*get_team_by_name(t_serveur *serv, char *name)
{
	int				i;
	t_team_entity	*current_team;

	i = 0;
	while (i < serv->team_hdl.nb_teams)
	{
		current_team = serv->team_hdl.array_teams + i;
		if (strncmp(current_team->name, name, strlen(current_team->name)) == 0 // parse only of the size of server teamname.
			&& strlen(name) - 1 == strlen(current_team->name)) // we dont want the \n and boundary size crossing.
		{
			return (current_team);
		}
		i++;
	}
	return (NULL);
}
