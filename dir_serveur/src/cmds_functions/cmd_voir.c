#include "../../includes/serveur.h"
/*	Player and World grid
	+y
	^
	|
	|
	|
	|------------> +x
*/


void	cmd_voir(struct s_serveur *serv, struct s_client_entity *client_cur, char *param) /* typedef ?? */
{
	(void) param;
	(void) serv;
	//get_voir_case_positions(serv, &(client->player));
	client_cur->list_cmds->clock_end = clock() + (CLOCKS_PER_SEC * 7 * serv->world_hdl.t_unit);
	write_buffer(&client_cur->buff_send, "OK\n", 3);
}

void	get_voir_case_positions(t_serveur *serv, t_player *player)
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
