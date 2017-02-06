#include "../../includes/serveur.h"

int		on_start_cmd_avance(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	char		*msg;

	if (param)
	{
		printf(KMAG "Bad format to cmd [avance] "
					"from sock %d\n" KRESET, client_cur->sock);
		return (-1);
	}
	// exec command
	cmd_avance(serv, client_cur, param);
	
	// gfx msg : "ppo #n X Y O\n"
	asprintf(&msg, "ppo #%d %d %d %d\n",
		client_cur->sock,
		client_cur->player.pos.x,
		client_cur->player.pos.y,
		client_cur->player.dir + 1);
	push_gfx_msg(serv, msg);
	free(msg);
	return (0);
}

void	on_end_cmd_avance(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)param;

	write_buffer(&client_cur->buff_send, "ok\n", 3);
}

void	cmd_avance(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)param;
	t_player	*cur_player;

	cur_player = &(client_cur->player);
	cur_player->cur_case->nb_players -= 1;
	if (cur_player->dir == UP) // 0 north
		cur_player->pos.y += 1;
	else if (cur_player->dir == RIGHT) // 1 east
		cur_player->pos.x += 1;
	else if (cur_player->dir == DOWN) // 2 south
		cur_player->pos.y -= 1;
	else if (cur_player->dir == LEFT) // 3 west
		cur_player->pos.x -= 1;

	// Boundary check
	if (cur_player->pos.x >= serv->world_hdl.map_x)
		client_cur->player.pos.x = 0;
	else if (cur_player->pos.x < 0)
		client_cur->player.pos.x = serv->world_hdl.map_x - 1;
	if (cur_player->pos.y >= serv->world_hdl.map_y)
		client_cur->player.pos.y = 0;
	else if (cur_player->pos.y < 0)
		client_cur->player.pos.y = serv->world_hdl.map_y - 1;

	// World board case assignation
	cur_player->cur_case->nb_players -= 1;
	cur_player->cur_case = &(serv->world_hdl.
			world_board[cur_player->pos.y][cur_player->pos.x]);
	cur_player->cur_case->nb_players += 1;
}
