#ifndef _gkg_core_cppext_StdInt_h_
#define _gkg_core_cppext_StdInt_h_


#if defined( BUILD_SEQU ) // Siemens SDK

#include "MrCommon/MrGlobalDefinitions/MrBasicTypes.h"

#ifndef PRId64
#ifdef _MSC_EXTENSIONS
#define PRId64 "I64d"
#else
#define PRId64 "lld"
#endif
#endif

#ifndef PRIu64
#ifdef _MSC_EXTENSIONS
#define PRIu64 "I64u"
#else
#define PRIu64 "llu"
#endif
#endif

#elif ( defined( WIN32 ) && !defined( __GNUC__ ) ) || defined( __osf__ )

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

#if defined( WIN32 )
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif

typedef int intptr_t;
typedef unsigned int uintptr_t;

#ifndef INT8_MIN
#define INT8_MIN   (-128)
#endif

#ifndef INT8_MAX
#define INT8_MAX   (127)
#endif

#ifndef UINT8_MAX
#define UINT8_MAX  (255)
#endif

#ifndef INT16_MIN
#define INT16_MIN  (-32768)
#endif

#ifndef INT16_MAX
#define INT16_MAX  (32767)
#endif

#ifndef UINT16_MAX
#define UINT16_MAX (65535)
#endif

#ifndef INT32_MIN
#define INT32_MIN  (-2147483648)
#endif

#ifndef INT32_MAX
#define INT32_MAX  (2147483647)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

#ifndef INT64_MIN
#define INT64_MIN		(-__INT64_C(9223372036854775807)-1)
#endif

#ifndef INT64_MAX
#define INT64_MAX		(__INT64_C(9223372036854775807))
#endif

#ifndef UINT64_MAX
#define UINT64_MAX		(__UINT64_C(18446744073709551615))
#endif

/* Values to test for integral types holding `void *' pointer.  */
# if __WORDSIZE == 64
#  define INTPTR_MIN		(-9223372036854775807L-1)
#  define INTPTR_MAX		(9223372036854775807L)
#  define UINTPTR_MAX		(18446744073709551615UL)
# else
#  define INTPTR_MIN		(-2147483647-1)
#  define INTPTR_MAX		(2147483647)
#  define UINTPTR_MAX		(4294967295U)
# endif

/* Signed.  */
# define INT8_C(c)	c
# define INT16_C(c)	c
# define INT32_C(c)	c
# if __WORDSIZE == 64
#  define INT64_C(c)	c ## L
# else
#  define INT64_C(c)	c ## LL
# endif

/* Unsigned.  */
# define UINT8_C(c)	c ## U
# define UINT16_C(c)	c ## U
# define UINT32_C(c)	c ## U
# if __WORDSIZE == 64
#  define UINT64_C(c)	c ## UL
# else
#  define UINT64_C(c)	c ## ULL
# endif

typedef int pid_t;

#if defined( WIN32 )

#ifndef PRId64
#ifdef _MSC_EXTENSIONS
#define PRId64 "I64d"
#else
#define PRId64 "lld"
#endif
#endif

#ifndef PRIu64
#ifdef _MSC_EXTENSIONS
#define PRIu64 "I64u"
#else
#define PRIu64 "llu"
#endif
#endif

#endif

#elif defined(__sun) || defined(__sgi)

#include <inttypes.h>

#else

#include <inttypes.h>
#include <sys/types.h>

#endif


#endif
