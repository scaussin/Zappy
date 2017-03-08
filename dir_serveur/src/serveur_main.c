/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 19:13:35 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 19:14:10 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

int		main(int argc, char **argv)
{
	t_serveur			serv;

	init_data(&serv);
	get_input(&serv, argc, argv);
	init_terrain(&serv);
	init_serveur(&serv);
	init_cmd_match_table(&serv);
	main_loop(&serv);
	close_all_connections(&serv);
	free(serv.cmd_hdl.cmd_match_table);
	return (0);
}
