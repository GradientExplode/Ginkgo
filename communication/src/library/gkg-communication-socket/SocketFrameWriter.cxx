#include <gkg-communication-socket/SocketFrameWriter.h>
#include <gkg-communication-socket/SocketHeader.h>


gkg::SocketFrameWriter::SocketFrameWriter( const std::string& host,
                                           int32_t port,
                                           bool noDelay )
                       : gkg::SocketWriter( host, 
                                            port,
                                            getFatal(),
                                            getBinary(),
                                            noDelay )
{
}


void gkg::SocketFrameWriter::send( gkg::SocketFrame& frame )
{

  gkg::SocketHeader header( this, frame.getId() );
  
  getServer() << header;
  frame.send( getServer() );

}


void gkg::SocketFrameWriter::flush()
{

  getServer().flush();

}


bool gkg::SocketFrameWriter::getFatal()
{

  return true;

}


bool gkg::SocketFrameWriter::getBinary()
{

  return true;
  
}
