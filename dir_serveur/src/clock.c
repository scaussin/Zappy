#include "../includes/serveur.h"

/*
	struct timeval
	{
		long	tv_sec;		// secondes
		long	tv_usec;	// microsecondes -> 1 000 000 us == 1s
	};
	struct timespec
	{
		long	tv_sec;		// secondes
		long	tv_nsec;	// nanosecondes -> 1 000 000 000 ns == 1s
	};
*/

void get_time(struct timespec *ts)
{

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
}

/*
**	Substraction of two times.
*/

struct timespec timespec_diff(struct timespec *start, struct timespec *stop)
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
	return result;
}

/*
**	Set the timeout for select according to the list of clients.
**	Timeout cannot be null or the select blocks the main loop.
*/
struct timeval	*set_timeout_select(struct timeval *timeout, struct timespec *lower_time_end)
{
	struct timespec now;

	if (!lower_time_end) // no cmd in progress
	{
		//printf(KYEL "timeout select: NULL (sleeping)\n" KRESET);
		timeout->tv_sec = 0;
		timeout->tv_usec = 100;
		return (timeout);
	}
	get_time(&now);
	if (min_timespec(&now, lower_time_end) == &now)
	{
		now = timespec_diff(&now, lower_time_end);
		//printf(KYEL "diff: %lds %ldns\n" KRESET, now.tv_sec, now.tv_nsec);
		timeout->tv_usec = (now.tv_nsec / 1000);
		timeout->tv_sec = now.tv_sec;// + ((now.tv_nsec * 1000) / 1000000);
	}
	else
	{
		timeout->tv_sec = 0;
		timeout->tv_usec = 0;
	}
	//printf(KYEL "timeout select: %06lds %06ldus\n" KRESET, timeout->tv_sec, (long)timeout->tv_usec);
	return (timeout);
}

/*
**	Add nano seconds to the timespec. Dont forget to multiply your sec time by 1000000000!
**	Example:
**		add_nsec_to_timespec(&player->next_dinner_time,
**		FOOD_LIFE_TIME * serv->world_hdl.t_unit * 1000000000);
*/

void	add_nsec_to_timespec(struct timespec *time, long nanosec)
{
	time->tv_nsec += nanosec;
	while (time->tv_nsec > 1000000000)
	{
		time->tv_nsec -= 1000000000;
		time->tv_sec += 1;
	}
}

/*
**	Extract nanoseconds from a given timespec. Useful for precise time calculations.
*/

long	convert_timespec_to_nsec(struct timespec time)
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

int		timespec_is_over(struct timespec time_end)
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
		return a->tv_nsec < b->tv_nsec ? a : b;
	else
		return a->tv_sec < b->tv_sec ? a : b;
}
