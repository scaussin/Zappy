#include "../includes/serveur.h"

/*
**	Called when a player has finished laying an egg.
**	The spawned egg's status will be checked at each program's loop.
*/

void	add_new_egg(t_serveur *serv, t_client_entity *client)
{
	t_egg	*new_egg;
	t_egg	*egg_tmp;

	new_egg = (t_egg *)malloc(sizeof(t_egg));

	// set egg number and team (there is no egg #0)
	serv->world_hdl.nb_of_eggs += 1;
	new_egg->egg_nb = serv->world_hdl.nb_of_eggs; // set the egg nb as the last one.
	new_egg->team = client->team;

	// set egg time of hatching.
	get_time(&new_egg->hatch_time);
	add_nsec_to_timespec(&new_egg->hatch_time,
		EGG_HATCH_TIME * serv->world_hdl.t_unit * 1000000000);
	
	// set egg position and status
	new_egg->pos.x = client->player.pos.x;
	new_egg->pos.y = client->player.pos.y;
	new_egg->has_hatched = 0;

	new_egg->next = NULL;
	printf(KGRN "[Serveur]: player #%d layed egg #%d\n" KRESET, client->sock, new_egg->egg_nb);

	// add t_egg node to chained list of eggs.
	if (serv->world_hdl.eggs == NULL)
	{
		serv->world_hdl.eggs = new_egg;
		return ;
	}
	else
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp->next)
		{
			egg_tmp = egg_tmp->next;
		}
		egg_tmp->next = new_egg;
		return ;
	}
}

/*
**	For a given player, will check if there is an egg corresponding to its team.
**	If yes, will return the egg pointer, otherwise, will return NULL;
*/

t_egg	*egg_available(t_serveur *serv, t_client_entity *client)
{
	t_egg	*egg_tmp;

	if (serv->world_hdl.eggs) 
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp)
		{
			if (egg_tmp->has_hatched && egg_tmp->team == client->team)
			{
				return (egg_tmp);
			}
			egg_tmp = egg_tmp->next;
		}
	}
	return (NULL);

}

/*
**	Called when the egg must be clear of the world's eggs.
**	Can happen when a player connected to a hatched egg, or when
**	an egg dies of hunger. Destroy the node of the egg in the chained list.
*/

void	clear_egg(t_serveur *serv, t_egg *egg)
{
	t_egg	*egg_tmp;

	if (serv->world_hdl.eggs == egg)
	{
		free(egg);
		serv->world_hdl.eggs = egg->next;
		serv->world_hdl.nb_of_eggs -= 1;
		return ;
	}
	else
	{
		egg_tmp = serv->world_hdl.eggs;
		while (egg_tmp->next && egg_tmp->next != egg)
			egg_tmp = egg_tmp->next;
		if (egg_tmp->next == egg)
		{
			egg_tmp->next = egg->next;
			free(egg);
			serv->world_hdl.nb_of_eggs -= 1;
		}
		else
			printf(KMAG "[Serveur]: could not find egg #%d to clear."
					KRESET, egg->egg_nb);
	}
}