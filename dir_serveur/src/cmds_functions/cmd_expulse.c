#include "../../includes/serveur.h"

/*static void	avance_client_in_cur_direction(t_serveur *serv, t_client_entity *client, int dir);*/

void	cmd_expulse(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)client_cur;
	(void)param;
	/*t_player	*cur_player;
	t_client_entity			*list_clients;

	cur_player = &(client_cur->player);
	list_clients = serv->client_hdl.list_clients;
	while (list_clients)
	{
		if (list_clients->is_in_game == 1
			&& list_clients->is_gfx == 0
			&& list_clients->is_disconnecting == 0
			&& list_clients->player.pos.x == cur_player.pos.x
			&& list_clients->player.pos.y == cur_player.pos.y
			&& list_clients->player != cur_player
			)
		{
			avance_client_in_cur_direction(serv, list_clients, cur_player->dir);
		}
		// player client response.
		write_buffer(&client_cur->buff_send, "expul\n", 6);
		list_clients = list_clients->next;
	}*/
}
/*
static void	avance_client_in_cur_direction(t_serveur *serv, t_client_entity *client, int dir)
{
	(void) serv;
	(void) client;
	(void) dir;
	t_player	*player;
	char		*msg;
	char		*msg_client;

	player = &(client_cur->player);
	if (dir == 0) // north
	{
		player->pos.y += 1;
	}
	else if (dir == 1) // east
	{
		player->pos.x += 1;
	}
	else if (dir == 2) // south
	{
		player->pos.y -= 1;
	}
	else if (dir == 3) // west
	{
		player->pos.x -= 1;
	}
	// Boundary check
	if (player->pos.x >= serv->world_hdl.map_x)
		client->player.pos.x = 0;
	else if (player->pos.x < 0)
		client->player.pos.x = serv->world_hdl.map_x - 1;
	if (player->pos.y >= serv->world_hdl.map_y)
		client->player.pos.y = 0;
	else if (player->pos.y < 0)
		client->player.pos.y = serv->world_hdl.map_y - 1;
	// Case assignation
	player->cur_case = &(serv->world_hdl.
			world_board[player->pos.y][player->pos.x]);

	// player client response.
	asprintf(&msg_client, "deplacement %d\n",dir + 1,);
	write_buffer(&client_cur->buff_send, msg_client, strlen(msg_client));


	// gfx msg : "ppo #n X Y O\n" 
	asprintf(&msg, "ppo #%d %d %d %d\n",
		client_cur->sock,
		cur_player->pos.x,
		cur_player->pos.y,
		cur_player->dir + 1);
	push_gfx_msg(serv, msg);
	free(msg);
}
*/