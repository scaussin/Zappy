/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication_gfx.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 18:00:56 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 18:48:41 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	gfx_cmd_msz(serv, gfx_client, "msz\n");
	gfx_cmd_sgt(serv, gfx_client, "sgt\n");
	gfx_cmd_mct(serv, gfx_client, "mct\n");
	gfx_cmd_tna(serv, gfx_client, "tna\n");
	tmp_client = serv->client_hdl.list_clients;
	while (tmp_client)
	{
		if (tmp_client->is_gfx == 0 && tmp_client->is_in_game == 1)
		{
			asprintf(&msg, "pnw #%d %d %d %d %d %s\n", tmp_client->sock,
				tmp_client->player.pos.x, tmp_client->player.pos.y,
				tmp_client->player.dir + 1, tmp_client->player.level,
				tmp_client->team->name);
			push_gfx_msg(serv, msg);
			free(msg);
		}
		tmp_client = tmp_client->next;
	}
	send_spawned_eggs(serv);
}

void	send_spawned_eggs(t_serveur *serv)
{
	t_egg	*eggs;
	char	*msg;

	eggs = serv->world_hdl.eggs;
	while (eggs)
	{
		asprintf(&msg, "enw #%d #%d %d %d\n",
			eggs->egg_nb,
			eggs->father_nb,
			eggs->pos.x,
			eggs->pos.y);
		push_gfx_msg(serv, msg);
		free(msg);
	}
}

void	lex_and_parse_gfx_cmds(t_serveur *serv, t_client_entity *gfx_client)
{
	char *cmd;

	while ((cmd = get_first_cmd(&gfx_client->buff_recv)))
	{
		parse_gfx_cmd(serv, gfx_client, cmd);
		free(cmd);
	}
}

void	parse_gfx_cmd(t_serveur *serv, t_client_entity *gfx_client, char *cmd)
{
	char	*arg_cmd;
	int		nb_of_parsed_chars;

	if (cmd[0] && cmd[0] != ' ')
	{
		if ((arg_cmd = strchr(cmd, ' ')))
			nb_of_parsed_chars = arg_cmd - cmd;
		else
			nb_of_parsed_chars = strlen(cmd);
		if (nb_of_parsed_chars > 4)
		{
			printf(KMAG "[Serveur]: gfx cmd too long (no gfx cmd takes more "
				"than 3 char): %s on gfx client command parsing." KRESET, cmd);
			return ;
		}
		exec_gfx_cmd(serv, gfx_client, cmd, nb_of_parsed_chars);
		return ;
	}
	printf(KMAG "[Serveur]: Unknown GFX command: %s on sock: %d\n" KRESET, cmd,
		gfx_client->sock);
	write_buffer(&gfx_client->buff_send, "suc\n", 4);
	return ;
}
