#include "../../includes/serveur.h"

void	cmd_prend(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	int		res_i;
	char	*gfx_msg;

	// Parse args first.	
	if (!regex_match(param, "^prend [a-zA-Z0-6]+\n$"))
	{
		printf(KMAG "Bad format to cmd [prend] "
					"from sock %d\n" KRESET, client_cur->sock);
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		return ;
	}
	else
	{
		if ((res_i = parse_ressource_index(param)) >= 0 &&
			(try_to_take_res(&client_cur->player, res_i)) >= 0)
		{
			write_buffer(&client_cur->buff_send, "ok\n", 3);
			
			// send prend cmd"pgt #n i\n"
			asprintf(&gfx_msg, "pgt #%d %d\n", client_cur->sock, res_i);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);

			// send inventaire "pin #n X Y q q q q q q q\n"
			asprintf(&gfx_msg, "pin #%d %d %d %d %d %d %d %d\n",
				client_cur->sock,
				client_cur->player.inventory[FOOD],
				client_cur->player.inventory[LINEMATE],
				client_cur->player.inventory[DERAUMERE],
				client_cur->player.inventory[SIBUR],
				client_cur->player.inventory[MENDIANE],
				client_cur->player.inventory[PHIRAS],
				client_cur->player.inventory[THYSTAME]);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
 
			// gfx world block ressource update.
			asprintf(&gfx_msg, "bct %d %d %d %d %d %d %d %d %d\n",
				client_cur->player.pos.x,
				client_cur->player.pos.y,
				client_cur->player.cur_case->ressources[FOOD],
				client_cur->player.cur_case->ressources[LINEMATE],
				client_cur->player.cur_case->ressources[DERAUMERE],
				client_cur->player.cur_case->ressources[SIBUR],
				client_cur->player.cur_case->ressources[MENDIANE],
				client_cur->player.cur_case->ressources[PHIRAS],
				client_cur->player.cur_case->ressources[THYSTAME]);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
		}
		else
			write_buffer(&client_cur->buff_send, "ko\n", 3);
	}
}

/*
**	Get index for ressource from param string, splitting it at the space.
**	Returns the index of the ressource (0-6 cf gfx_protocol) or -1
**	if no ressource is found.
**	Example : "prend 1" and "pose linemate" will both return (1);
*/

int		parse_ressource_index(char *param)
{
	char	*arg;
	int		arg_len;

	arg = strchr(param, ' ') + 1;
	arg_len = strlen(arg) - 1; // we dont want the \n
	// Try to take the ressource from the ground for the corresponding ressource.
	if (strncmp(arg, "0", arg_len) == 0 || strncmp(arg, "nourriture", arg_len) == 0)
		return (0);
	else if (strncmp(arg, "1", arg_len) == 0 || strncmp(arg, "linemate", arg_len) == 0)
		return (1);
	else if (strncmp(arg, "2", arg_len) == 0 || strncmp(arg, "deraumere", arg_len) == 0)
		return (2);
	else if (strncmp(arg, "3", arg_len) == 0 || strncmp(arg, "sibur", arg_len) == 0)
		return (3);
	else if (strncmp(arg, "4", arg_len) == 0 || strncmp(arg, "mendiane", arg_len) == 0)
		return (4);
	else if (strncmp(arg, "5", arg_len) == 0 || strncmp(arg, "phiras", arg_len) == 0)
		return (5);
	else if (strncmp(arg, "6", arg_len) == 0 || strncmp(arg, "thystame", arg_len) == 0)
		return (6);
	else
		printf(KMAG "Invalid parameter for cmd [%s]\n" KRESET, param);
	return (-1);

}

/*
** Client try to take res on its current case.
** Returns the res index if success, -1 otherwise.
*/
int		try_to_take_res(t_player *player, int res_nb)
{
	if (player->cur_case->ressources[res_nb] > 0)
	{
		player->inventory[res_nb] += 1;
		player->cur_case->ressources[res_nb] -= 1;
		return (res_nb);
	}
	return (-1);
}
