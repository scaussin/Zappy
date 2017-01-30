#include "../includes/serveur.h"

/*
**	Functions for communicating especially to the gfx.
*/

void	push_gfx_msg(t_serveur *serv, char *msg)
{
	if (serv->client_hdl.gfx_client)
	{
		write_buffer(&(serv->client_hdl.gfx_client->buff_send),
						msg, strlen(msg));
	}
}

void	send_current_world_state(t_serveur *serv, t_client_entity *gfx_client)
{
	char				*msg;
	t_client_entity		*tmp_client;
	// int				y;
	//int				x;
	//t_world_case	**world_board;

	
	// ------------------------------------	//
	// sending World size					//
	// ------------------------------------	//
	// "msz X Y\n"
	gfx_cmd_msz(serv, gfx_client, "msz\n");

	// ------------------------------------	//
	// sending server time unit				//
	// ------------------------------------	//
	// "sgt T\n"
	asprintf(&msg, "sgt %f\n", serv->world_hdl.t_unit);
	push_gfx_msg(serv, msg);
	free(msg);

	// ------------------------------------	//
	// Sending map content					//
	// ------------------------------------	//
	// "bct X Y q q q q q q q\n" * nbr_cases
	gfx_cmd_mct(serv, gfx_client, "mct\n");

	// ------------------------------------	//
	// Sending team names					//
	// ------------------------------------	//
	gfx_cmd_tna(serv, gfx_client, "tna\n");

	// ------------------------------------	//
	// Sending currently connected players	//
	// ------------------------------------	//
	tmp_client = serv->client_hdl.list_clients;
	while (tmp_client)
	{
		// we dont want to send non authentified and the gfx
		if (tmp_client->is_gfx == 0 && tmp_client->is_in_game == 1)
		{
			// send "pnw #n X Y O L N\n"
			asprintf(&msg, "pnw #%d %d %d %d %d %s\n",
				tmp_client->sock,
				tmp_client->player.pos.x,
				tmp_client->player.pos.y,
				tmp_client->player.dir + 1, // +1 cause enum start at 0, and gfx protocol wants 1;
				tmp_client->player.level,
				tmp_client->team->name);
			push_gfx_msg(serv, msg);
		}
		tmp_client = tmp_client->next;
	}
}

void	lex_and_parse_gfx_cmds(t_serveur *serv, t_client_entity *gfx_client)
{
	char *cmd;

	while ((cmd = get_first_cmd(&gfx_client->buff_recv))) // -> Buffer lexer.
	{
		parse_gfx_cmd(serv, gfx_client, cmd);
		free(cmd);
	}
}

void	parse_gfx_cmd(t_serveur *serv, t_client_entity *gfx_client, char *cmd)
{
	int		i;
	char	*arg_cmd;
	int		nb_of_parsed_chars;

	i = 0;
	// Check for arguments
	if ((arg_cmd = strchr(cmd, ' ')))
		nb_of_parsed_chars = arg_cmd - cmd;
	else
		nb_of_parsed_chars = strlen(cmd);
	if (nb_of_parsed_chars > 4)
	{
		printf(KMAG "[Serveur]: gfx cmd too long (no gfx cmd takes more than 3 char):"
					"%s on gfx client command parsing." KRESET, cmd);
		return ;
	}

	// parsing with gfx match table.
	while (i < SIZE_GFX_CMD_MATCH_TABLE)
	{
		if (strncmp(serv->cmd_hdl.gfx_cmd_match_table[i].name,
			cmd, nb_of_parsed_chars) == 0)
		{
			// -> Direct execution of cmds.
			serv->cmd_hdl.gfx_cmd_match_table[i].func(serv, gfx_client, cmd);
			return ;
		}
		i++;
	}
	printf(KMAG "[Serveur]: Unknown GFX command: %s on sock: %d\n" KRESET, cmd, gfx_client->sock);
	// send gfx "suc\n"
	write_buffer(&gfx_client->buff_send, "suc\n", 4);
	return ;
}
