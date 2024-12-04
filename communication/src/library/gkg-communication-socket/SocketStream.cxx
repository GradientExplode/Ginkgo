#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-socket/SocketBuffer.h>
#include <gkg-core-cppext/CUniStd.h>
#include <iostream>
#include <cstdio>


// specialize this class to socket requests by initializing an SocketBuffer;
// set setIncompleteRequest to ensure the first attempt to extract a socket
// request will call underflow
gkg::SocketStream::SocketStream( gkg::SocketBuffer* buffer )
                  : gkg::IOStream( buffer ),
                    _buffer( buffer )
{

  _packetsSize = BUFSIZ;
  _buffer->setStream( this );
  init( _buffer );

}

gkg::SocketStream::~SocketStream()
{

  if ( _buffer )
  {

    delete _buffer;

  }

}


// provide operations on the SocketBuffer; most change the stream's state
// so radically that it's either cleared or failed
void gkg::SocketStream::listen()
{

  verify( getSocketBuffer()->listen() != 0 );

}

void gkg::SocketStream::connect()
{

  verify( getSocketBuffer()->connect() != 0 );

}


int32_t gkg::SocketStream::accept()
{

  // no effect on stream's state
  int32_t fd;
  if ( !getSocketBuffer()->accept( fd ) )
  {

    return -1;

  }
  return fd;

}


void gkg::SocketStream::close()
{

  verify( getSocketBuffer()->close() != 0 );

}


void gkg::SocketStream::setNonBlocking( bool value )
{

  // can only fail, not clear
  if ( !getSocketBuffer()->setNonBlocking( value ) )
  {

    setstate( std::ios::failbit );

  }

}


void gkg::SocketStream::setVerbose( bool value )
{

  // no effect on stream's state
  getSocketBuffer()->setVerbose( value );

}


// Negotiate the option of binary I/O with the remote IOStream.  If
// binary I/O is set, compare endians to enable swapping if necessary.
// Ordinarily we don't need an explicit flush when we're switching
// from insertion to extraction, but we do here because the first
// underflow may read both format and remoteEndian, thus preventing
// underflow from being called again and flushing localEndian.  Tying
// the stream to itself won't work either because 1) ipfx won't call
// flush if remoteEndian is already available, and 2) opfx will make
// the stream flush itself before every insertion because it doesn't
// check for streams tied to themselves.  Sigh....

void gkg::SocketStream::negotiate( bool b )
{

  if ( !good() )
  {

    return;

  }

  setBinary( b );
  char format = getBinary() ? 'T' : 'F';

  ssize_t byteCount = ::write( _buffer->getFd(), &format, 1 );
  
  if ( byteCount != 1 )
  {
  
    return;
  
  }
  
  ::fsync( _buffer->getFd() );
  byteCount = ::read( _buffer->getFd(), &format, 1 );

  if ( byteCount != 1 )
  {
  
    return;
  
  }

  if ( format != 'T' && format != 'F' )
  {

    setstate( std::ios::badbit );

  }
  else if ( format == 'F' )
  {

    setBinary( false );

  }

  if ( getBinary() )
  {

    int32_t indian = 1;
    char localEndian = ( *( char* )&indian ) ? 'l' : 'B';
    char remoteEndian = localEndian;

    byteCount = ::write( _buffer->getFd(), &localEndian, 1 );

    if ( byteCount != 1 )
    {
  
      return;
  
    }

    ::fsync( _buffer->getFd() );
    byteCount = ::read( _buffer->getFd(), &remoteEndian, 1 );

    if ( byteCount != 1 )
    {
  
      return;
  
    }

    if ( remoteEndian != 'l' && remoteEndian != 'B' )
    {

      setstate( std::ios::badbit );

    }
    else if ( remoteEndian != localEndian )
    {

      setSwapped( true );

    }

  }

  std::cerr << "Socket stream : binary[" << ( getBinary() ? true : false );
  std::cerr << "], swapped[" << ( getSwapped() ? true : false );
  std::cerr << "]" << std::endl;

}


void gkg::SocketStream::setPacketsSize( int32_t packetsSize )
{

  _packetsSize = packetsSize;

}


void gkg::SocketStream::writePackets( const char* buffer, int64_t count )
{

  int32_t n = (int32_t)( count / _packetsSize );
  int32_t r = (int32_t)( count % _packetsSize );
  const char* bufferPtr = buffer;

  while ( n-- )
  {

    write( bufferPtr, _packetsSize );
    bufferPtr += _packetsSize;

  }

  if ( r )
  {

    write( bufferPtr, r );

  }

}


void gkg::SocketStream::readPackets( char* buffer, int64_t count )
{

  int32_t n = (int32_t)( count / _packetsSize );
  int32_t r = (int32_t)( count % _packetsSize );
  char* bufferPtr = buffer;

  while ( n-- )
  {

    read( bufferPtr, _packetsSize );
    bufferPtr += _packetsSize;

  }

  if ( r )
  {

    read( bufferPtr, r );

  }

}


// return or set protected member variables.
gkg::SocketBuffer* gkg::SocketStream::getSocketBuffer()
{

  return _buffer;

}


// for some of the functions above, success means starting over with a
// clean slate while failure means setting failbit as usual
void gkg::SocketStream::verify( int32_t ok )
{

  if ( ok )
  {

    clear();

  }
  else
  {

    setstate( std::ios::failbit );

  }

}
