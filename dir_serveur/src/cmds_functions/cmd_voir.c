#include "../../includes/serveur.h"
/*	Player and World grid
	+y
	^
	|
	|
	|
	|------------> +x
*/


void	cmd_voir(struct s_serveur *serv, struct s_client_entity *client_cur, char *param)
{
	t_world_case	**world;
	t_player		*player;
	t_pos			*tab_pos;
	int				i;
	int				j;
	int				k;
	int				x;
	int				y;
	int				multi_ressources;
	char			*see_str;
	char			*sep_rsrc;
	char			*sep_case;

	(void) param;
	player = &(client_cur->player);
	world = serv->world_hdl.world_board;
	tab_pos = get_see_case_coordinates(serv, player);

	sep_rsrc = " ";
	sep_case = ", ";
	see_str = (char *)s_malloc(sizeof(char) * 2);
	see_str[0] = '{';
	see_str[1] = '\0';
	i = -1;
	while (++i < player->nb_see_case)
	{
		y = tab_pos[i].y;
		x = tab_pos[i].x;
		j = -1;
		multi_ressources = 0;
		while (++j < (i == 0 ? world[y][x].nb_players - 1 : world[y][x].nb_players))
		{
			if (multi_ressources)
			{
				see_str = str_concat_realloc1(see_str, sep_rsrc);
				see_str = str_concat_realloc1(see_str, "joueur");
			}
			else
			{
				see_str = str_concat_realloc1(see_str, "joueur");
				multi_ressources = 1;
			}
		}
		j = -1;
		while (++j < NB_RESSOURCES)
		{
			k = -1;
			while (++k < world[y][x].ressources[j])
			{
				if (multi_ressources)
				{
					see_str = str_concat_realloc1(see_str, sep_rsrc);
					see_str = str_concat_realloc1(see_str,
						serv->world_hdl.name_ressources[j]);
				}
				else
				{
					see_str = str_concat_realloc1(see_str,
						serv->world_hdl.name_ressources[j]);
					multi_ressources = 1;
				}
			}
		}
		// j = -1;
		// multi_ressources = 0;
		// while (++j < world[y][x].nb_players)
		// {
		// 	if (multi_ressources)
		// 	{
		// 		see_str = str_concat_realloc1(see_str, sep_rsrc);
		// 		see_str = str_concat_realloc1(see_str, "joueur");
		// 	}
		// 	else
		// 	{
		// 		see_str = str_concat_realloc1(see_str, "joueur");
		// 		multi_ressources = 1;
		// 	}
		// }
		if (i != player->nb_see_case - 1)
			see_str = str_concat_realloc1(see_str, sep_case);
		else
			see_str = str_concat_realloc1(see_str, "}\n");
	}

	printf("%s\n", see_str);
	write_buffer(&client_cur->buff_send, see_str, strlen(see_str));
	free(see_str);
	free(tab_pos);
}

t_pos	*get_see_case_coordinates(t_serveur *serv, t_player *player)
{
	t_pos	*tab_pos;
	t_pos	rel_pos;
	int		nb_case;
	int		i_line;
	int		i_case;
	int		i_tab;

	nb_case = get_nb_case(player->level);
	tab_pos = (t_pos *)s_malloc(nb_case * sizeof(t_pos));
	i_line = 0;
	i_tab = 0;
	while (i_line <= player->level)
	{
		i_case = 0;
		rel_pos.x = i_line * -1;
		rel_pos.y = i_line;
		while (i_case < 2 * i_line + 1)
		{
			fill_tab(tab_pos + i_tab, &rel_pos, player, serv);
			printf("*x %d y %d*", rel_pos.x, rel_pos.y); // Debug
			i_case++;
			i_tab++;
			rel_pos.x++;
		}
		i_line++;
		printf("\n"); // Debug
	}
	return (tab_pos);
}

int		get_nb_case(int level)
{
	int	total;
	int	i;

	total = 0;
	i = -1;
	while (++i <= level)
		total += 2 * i + 1;
	return (total);
}

void	fill_tab(t_pos *abs_pos, t_pos *rel_pos, t_player *player,
	t_serveur *serv)
{
	int	world_width;
	int	world_height;

	world_width = serv->world_hdl.map_x;
	world_height = serv->world_hdl.map_y;
	if (player->dir == UP)
	{
		abs_pos->x = modulo(player->pos.x + rel_pos->x, world_width);
		abs_pos->y = modulo(player->pos.y + rel_pos->y, world_height);
	}
	else if (player->dir == RIGHT)
	{
		abs_pos->x = modulo(player->pos.x + rel_pos->y, world_width);
		abs_pos->y = modulo(player->pos.y - rel_pos->x, world_height);
	}
	else if (player->dir == DOWN)
	{
		abs_pos->x = modulo(player->pos.x - rel_pos->x, world_width);
		abs_pos->y = modulo(player->pos.y - rel_pos->y, world_height);
	}
	else if (player->dir == LEFT)
	{
		abs_pos->x = modulo(player->pos.x - rel_pos->y, world_width);
		abs_pos->y = modulo(player->pos.y + rel_pos->x, world_height);
	}
}
