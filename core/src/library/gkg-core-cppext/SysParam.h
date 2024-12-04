#ifndef _gkg_core_cppext_SysParam_h_
#define _gkg_core_cppext_SysParam_h_


#ifdef __GNUC__

#include <sys/param.h>

#elif defined( WIN32 )

#define NOFILE 256

#ifndef howmany
# define howmany(x, y) (((x) + ((y) - 1 )) / (y))
#endif

#endif

#elif defined(sun) && NOFILE==20

#undef NOFILE
#define NOFILE 256

#endif
