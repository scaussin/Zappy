/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 12:05:01 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/13 12:05:03 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	check_time_input_limit(t_serveur *serv, char *arg)
{
	int	t_val;

	t_val = strtol(arg, NULL, 10);
	if (t_val <= 0 || t_val > MAX_GAME_TIME_UNIT)
	{
		printf(KRED "Error: time value incorrect (1 <= [t] >= %d): %s\n"
			KRESET, MAX_GAME_TIME_UNIT, arg);
		exit(-1);
	}
	serv->world_hdl.t_unit = 1 / (float)t_val;
}
