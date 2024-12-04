#ifndef _gkg_core_cppext_CUniStd_h_
#define _gkg_core_cppext_CUniStd_h_


#ifdef __GNUC__

#include <unistd.h>

#elif ( defined(__APPLE__) && defined(__MACH__) )

#include <unistd.h>

#endif


#endif
