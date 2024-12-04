#include <gkg-communication-socket/SocketRegistry.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcWriter.h>
#include <cstdlib>  // for abort()
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

#ifndef __DECCXX

#include <cstdlib>
#include <gkg-core-cppext/CUniStd.h>
#include <time.h>

#ifdef _G_HAVE_SYS_RESOURCE

#include <sys/time.h>
#include <sys/resource.h>

#endif

#endif

#include <sys/socket.h>
#include <netinet/tcp.h>



// open a connection to an RPC service at its registered host name and
// port number, or give the host name and port number needed to open a
// connection to the RPC service, or give the number of an already
// open file descriptor, or give the address of an already open
// rpcstream (connection); negotiate the I/O format if opening a
// connection; terminate the program if errors should be fatal

gkg::RpcWriter::RpcWriter( const std::string& path, 
                           bool fatal, 
                           bool binary,
                           bool noDelay )
               : gkg::SocketWriter( path, fatal, binary, noDelay ),
	         _prolog( "" ),
                 _epilog( "" )
{
}


gkg::RpcWriter::RpcWriter( const std::string& host, 
                           in_port_t port,
                           bool fatal, 
                           bool binary,
                           bool noDelay )
               : gkg::SocketWriter( host, port, fatal, binary, noDelay ),
	         _prolog( "" ),
                 _epilog( "" )
{
}


gkg::RpcWriter::RpcWriter( int32_t fd, bool fatal, bool binary, bool noDelay )
               : gkg::SocketWriter( fd, fatal, binary, noDelay ),
	         _prolog( "" ),
                 _epilog( "" )
{
}


gkg::RpcWriter::RpcWriter( gkg::SocketStream* server, bool noDelay )
               : gkg::SocketWriter( server, noDelay ),
	         _prolog( "" ),
                 _epilog( "" )
{
}


void gkg::RpcWriter::setProlog( const std::string& prolog )
{

  _prolog = prolog;

}


void gkg::RpcWriter::setEpilog( const std::string& epilog )
{

  _epilog = epilog;

}
