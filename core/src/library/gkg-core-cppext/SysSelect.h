#ifndef _gkg_core_cppext_SysSelect_h_
#define _gkg_core_cppext_SysSelect_h_


#ifdef __GNUC__

#include <sys/select.h>

#if defined(__GLIBC__) && (__GLIBC__==2 && __GLIBC_MINOR__>0 || __GLIBC__>2) \
    && __GNUG__<3
#define fds_bits __fds_bits
#endif

#elif defined( WIN32 )

typedef long int __fd_mask;

#define __FD_SETSIZE 1024

#define NFDBITS (8 * (int)sizeof(__fd_mask))

#define fds_bits fd_array

#endif


#endif
