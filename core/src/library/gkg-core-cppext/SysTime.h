#ifndef _gkg_core_cppext_SysTime_h_
#define _gkg_core_cppext_SysTime_h_


#ifdef __GNUC__

#include <sys/time.h>

#elif ( defined(__APPLE__) && defined(__MACH__) )

#include <sys/time.h>

#elif defined( WIN32 )

#include <time.h>
#include <windows.h>

#if defined( _MSC_VER ) || defined( _MSC_EXTENSIONS )
# define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
# define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif

struct timezone
{
	int tz_minuteswest;
	int tz_dsttime;
};

int gettimeofday( struct timeval* tv, struct timezone* tz );

#endif


#endif
