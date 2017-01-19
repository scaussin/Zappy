#include "../../includes/serveur.h"

void	cmd_prend(t_serveur *serv, t_client_entity *client_cur, char *param)
{
	(void)serv;
	(void)client_cur;
	(void)param;
	char	*arg;
	int		arg_len;
	int		success;
	char	*gfx_msg;

	// Parse args first.	
	if (!regex_match(param, "^prend [a-zA-Z0-6]+\n$"))
	{
		printf(KMAG "Bad format to cmd [prend]."
					"From sock %d\n" KRESET, client_cur->sock);
		write_buffer(&client_cur->buff_send, "ko\n", 3);
		return ;
	}
	else
	{
		success = 0;
		arg = strchr(param, ' ') + 1;
		arg_len = strlen(arg) - 1; // we dont want the \n
		printf("param = %s\n", arg);

		// Try to take the ressource from the ground for the corresponding ressource.
		if (strncmp(arg, "0", arg_len) == 0 || strncmp(arg, "nourriture", arg_len) == 0)  // TODO: player food == player life time.
			success = try_to_take_res(&client_cur->player.inventory.food,
				&client_cur->player.cur_case->ressources.food, 0);
		else if (strncmp(arg, "1", arg_len) == 0 || strncmp(arg, "linemate", arg_len) == 0)
			success = try_to_take_res(&client_cur->player.inventory.linemate,
				&client_cur->player.cur_case->ressources.linemate, 1);
		else if (strncmp(arg, "2", arg_len) == 0 || strncmp(arg, "deraumere", arg_len) == 0)
			success = try_to_take_res(&client_cur->player.inventory.deraumere,
				&client_cur->player.cur_case->ressources.deraumere, 2);
		else if (strncmp(arg, "3", arg_len) == 0 || strncmp(arg, "sibur", arg_len) == 0)
			success = try_to_take_res(&client_cur->player.inventory.sibur,
				&client_cur->player.cur_case->ressources.sibur, 3);
		else if (strncmp(arg, "4", arg_len) == 0 || strncmp(arg, "mendiane", arg_len) == 0)
			success = try_to_take_res(&client_cur->player.inventory.mendiane,
				&client_cur->player.cur_case->ressources.mendiane, 4);
		else if (strncmp(arg, "5", arg_len) == 0 || strncmp(arg, "phiras", arg_len) == 0)
			success = try_to_take_res(&client_cur->player.inventory.phiras,
				&client_cur->player.cur_case->ressources.phiras, 5);
		else if (strncmp(arg, "6", arg_len) == 0 || strncmp(arg, "thystame", arg_len) == 0)
			success = try_to_take_res(&client_cur->player.inventory.thystame,
				&client_cur->player.cur_case->ressources.thystame, 6);
		else
			printf(KMAG "Invalid parameter for cmd [prend] from sock %d\n", client_cur->sock);
		if (success >= 0)
		{
			write_buffer(&client_cur->buff_send, "ok\n", 3);
			//"pgt #n i\n"
			asprintf(&gfx_msg, "pgt #%d %d\n", client_cur->sock, success);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
			asprintf(&gfx_msg, "bct %d %d %d %d %d %d %d %d %d\n",
				client_cur->player.pos.x,
				client_cur->player.pos.y,
				client_cur->player.cur_case->ressources.food,
				client_cur->player.cur_case->ressources.linemate,
				client_cur->player.cur_case->ressources.deraumere,
				client_cur->player.cur_case->ressources.sibur,
				client_cur->player.cur_case->ressources.mendiane,
				client_cur->player.cur_case->ressources.phiras,
				client_cur->player.cur_case->ressources.thystame);
			push_gfx_msg(serv, gfx_msg);
			free(gfx_msg);
		}
		else 
			write_buffer(&client_cur->buff_send, "ko\n", 3);
	}
}

int		try_to_take_res(int *client_res, int *ressource, int res_nb)
{
	if (*ressource > 0)
	{
		*client_res += 1;
		*ressource -= 1;
		return (res_nb);
	}
	return (-1);
}
