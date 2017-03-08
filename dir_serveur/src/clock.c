/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clock.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaussin <scaussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:48:25 by scaussin          #+#    #+#             */
/*   Updated: 2017/03/03 16:15:06 by scaussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**  struct timeval
**  {
**  	long	tv_sec;		// secondes
**  	long	tv_usec;	// microsecondes -> 1 000 000 us == 1s
**  };
**  struct timespec
**  {
**  	long	tv_sec;		// secondes
**  	long	tv_nsec;	// nanosecondes -> 1 000 000 000 ns == 1s
**  };
*/

#ifdef __MACH__

void			get_time(struct timespec *ts)
{
	clock_serv_t	cclock;
	mach_timespec_t	mts;

	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
}

#else

void			get_time(struct timespec *ts)
{
	clock_gettime(CLOCK_REALTIME, ts);
}

#endif

/*
**	Substraction of two times.
*/

struct timespec	timespec_diff(struct timespec *start, struct timespec *stop)
{
	struct timespec result;

	if ((stop->tv_nsec - start->tv_nsec) < 0)
	{
		result.tv_sec = stop->tv_sec - start->tv_sec - 1;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	}
	else
	{
		result.tv_sec = stop->tv_sec - start->tv_sec;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
	return (result);
}

/*
**	Set the timeout for select according to the list of clients.
**	Timeout cannot be null or the select blocks the main loop.
*/

struct timeval	*set_timeout_select(struct timeval *timeout,
	struct timespec *lower_time_end)
{
	struct timespec now;

	if (!lower_time_end)
	{
		timeout->tv_sec = 0;
		timeout->tv_usec = 100;
		return (timeout);
	}
	get_time(&now);
	if (min_timespec(&now, lower_time_end) == &now)
	{
		now = timespec_diff(&now, lower_time_end);
		timeout->tv_usec = (now.tv_nsec / 1000);
		timeout->tv_sec = now.tv_sec;
	}
	else
	{
		timeout->tv_sec = 0;
		timeout->tv_usec = 0;
	}
	return (timeout);
}
