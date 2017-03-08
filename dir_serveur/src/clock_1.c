/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clock_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:50:41 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 15:51:28 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Add nano seconds to the timespec. Dont forget to multiply your sec time by
**	1000000000!
**	Example:
**		add_nsec_to_timespec(&player->next_dinner_time,
**		FOOD_LIFE_TIME * serv->world_hdl.t_unit * 1000000000);
*/

void			add_nsec_to_timespec(struct timespec *time, long nanosec)
{
	time->tv_nsec += nanosec;
	while (time->tv_nsec > 1000000000)
	{
		time->tv_nsec -= 1000000000;
		time->tv_sec += 1;
	}
}

/*
**	Extract nanoseconds from a given timespec. Useful for precise time
**	calculations.
*/

long			convert_timespec_to_nsec(struct timespec time)
{
	while (time.tv_sec > 0)
	{
		time.tv_sec -= 1;
		time.tv_nsec += 1000000000;
	}
	return (time.tv_nsec);
}

/*
**	Will check if time_end is reached compared to current frame time.
**	If yes, return 1, return 0 otherwise.
*/

int				timespec_is_over(struct timespec time_end)
{
	struct timespec	now;

	get_time(&now);
	if (now.tv_sec > time_end.tv_sec ||
		(now.tv_sec == time_end.tv_sec && now.tv_nsec >= time_end.tv_nsec))
		return (1);
	return (0);
}

/*
**	Compare two times and returns the closest one.
*/

struct timespec	*min_timespec(struct timespec *a, struct timespec *b)
{
	if (!a)
		return (b);
	if (a->tv_sec == b->tv_sec)
		return (a->tv_nsec < b->tv_nsec ? a : b);
	else
		return (a->tv_sec < b->tv_sec ? a : b);
}
