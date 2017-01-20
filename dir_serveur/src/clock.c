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

struct timespec timespec_diff(struct timespec *start, struct timespec *stop)
{
	struct timespec result;

	if ((stop->tv_nsec - start->tv_nsec) < 0) {
		result.tv_sec = stop->tv_sec - start->tv_sec - 1;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	} else {
		result.tv_sec = stop->tv_sec - start->tv_sec;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
	return result;
}

struct timeval	*set_timeout_select(struct timeval *timeout, struct timespec *lower_time_end)
{
	struct timespec now;

	if (!lower_time_end) // no cmd in progress : timeout is useless
	{
		//printf(KYEL "timeout select: NULL (sleeping)\n" KRESET);
		return (NULL);
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
