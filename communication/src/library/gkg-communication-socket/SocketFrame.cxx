#include <gkg-communication-socket/SocketFrame.h>


gkg::SocketFrame::SocketFrame( int32_t id, 
                               int32_t elementSize, 
                               int64_t elementCount )
                 : _id( id ),
                   _elementSize( elementSize ),
                   _elementCount( elementCount )
{
}


gkg::SocketFrame::SocketFrame()
                 : _id( 0 ),
                   _elementSize( 0 ),
                   _elementCount( 0 )
{
}


gkg::SocketFrame::~SocketFrame()
{
}


int32_t gkg::SocketFrame::getId()
{

  return _id;

}


void gkg::SocketFrame::send( SocketStream& server )
{

  if ( _elementCount && server.good() )
  {

    server.write( (const char*)&_elementSize, sizeof( int32_t ) );
    server.write( (const char*)&_elementCount, sizeof( int64_t ) );
    sendData( server );

  }

}
