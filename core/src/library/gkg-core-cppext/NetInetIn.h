#ifndef _gkg_communication_socket_NetInetIn_h_
#define _gkg_communication_socket_NetInetIn_h_


// C++ interface to internet definitions

// don't try to hide the ntohl prototypes because they may be
// defined as macros, but don't define them as extern "C" either.

#include <netinet/in.h>

/*
#ifndef __lucid

#ifndef ntohl

/// the operations are defined as functions, possibly without prototypes,
// so we provide our own extern "C" declarations.

extern "C" 
{

  unsigned long ntohl( unsigned long );
  unsigned long htonl( unsigned long );
  unsigned short ntohs( unsigned short );
  unsigned short htons( unsigned short );

}

#endif

#endif
*/

#endif
