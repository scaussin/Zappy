#include "../../includes/serveur.h"

void	cmd_gauche(struct s_serveur *serv, struct s_client_entity *client_cur, char *param)
{
	t_player	*cur_player;
	char		*msg;

	if (param)
	{
		printf(KMAG "Bad format to cmd [gauche] "
					"from sock %d\n" KRESET, client_cur->sock);
		return ;
	}
	cur_player = &(client_cur->player);
	if (cur_player->dir == 0)
		cur_player->dir = 3;
	else
		cur_player->dir -= 1;

	// player client response.
	write_buffer(&client_cur->buff_send, "ok\n", 3);
	// gfx msg : "ppo #n X Y O\n" 
	asprintf(&msg, "ppo #%d %d %d %d\n",
		client_cur->sock,
		cur_player->pos.x,
		cur_player->pos.y,
		cur_player->dir + 1);
	push_gfx_msg(serv, msg);
	free(msg);
}
