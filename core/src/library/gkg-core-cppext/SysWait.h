#ifndef _gkg_core_cppext_SysWait_h_
#define _gkg_core_cppext_SysWait_h_


#ifdef __GNUC__

#include <sys/wait.h>

#elif defined( WIN32 )

#define WNOHANG 0x00000001

#endif


#endif
