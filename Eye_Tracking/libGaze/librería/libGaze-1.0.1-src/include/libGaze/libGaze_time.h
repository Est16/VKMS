#ifndef LIBGAZE_TIME_H_
#define LIBGAZE_TIME_H_


#if defined __WIN32__ || defined WIN32
#include <windows.h>

#define SEC_SLEEP(s) Sleep(s*1000)
#define MILLISEC_SLEEP(s) Sleep(s)

static int getwindowstimeofday(struct timeval *tp, void* pointer){
	DWORD time=GetTickCount();
	long seconds=time/1000;
	long rest=time-seconds*1000;
	long microseconds=rest*1000;
	tp->tv_sec=seconds;
	tp->tv_usec=microseconds;
	return 1;
}
#define gettimeofday(tv,NULL) getwindowstimeofday(tv,NULL)

#include <sys/timeb.h>
static unsigned int windowstime(void* pointer)
{
	struct _timeb timebuffer;
	_ftime( &timebuffer ); // C4996
	return (unsigned int)timebuffer.time;
}
#define randtime(p) windowstime(p)

#else
#include <sys/time.h>
#define MILLISEC_SLEEP(s) usleep(s*1000)
#define SEC_SLEEP(s) sleep(s)
#define randtime(p) time(p)
#endif


#endif /*LIBGAZE_TIME_H_*/
