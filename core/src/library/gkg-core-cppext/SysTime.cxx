#include <gkg-core-cppext/SysTime.h>
#include <gkg-core-cppext/StdInt.h>


#if defined( WIN32 )

int gettimeofday( struct timeval* tv, struct timezone* tz )
{

  FILETIME ft;
  uint64_t tmpres = 0;
  static int tzflag;

  if ( tv != 0 )
  {

    GetSystemTimeAsFileTime( &ft );

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tmpre /= 10;
    tv->tv_sec = (long)( tmpres / 1000000UL );
    tv->tv_usec = (long)( tmpres % 1000000UL );

  }

  if ( tz != 0 )
  {

    if ( !tzflag )
	{

      _tzset();
      tzflag++;

    }

    tz->tz_minuteswest = _timezone / 60;
	tz->tz_dsttime = _daylight;

  }

  return 0;

}

#endif

