#include <gkg-communication-rpc/RpcCommandReader.h>


gkg::RpcCommandReader::RpcCommandReader( int32_t port )
                      : gkg::RpcReader( port,
		                        getBinary(),
					false )
{

  setProlog( "RpcCommandWriter" );
  setEpilog( "RpcCommandWriter" );
  addFunction( RECEIVE, &gkg::RpcCommandReader::receive );

}


gkg::RpcCommandReader::~RpcCommandReader()
{

  std::map< int32_t, gkg::RpcCallback* >::iterator
    it = _callbacks.begin(),
    ie = _callbacks.end();
    
  while ( it != ie )
  {
  
    delete it->second;
    ++it;
  
  }

}


void gkg::RpcCommandReader::receive( gkg::SocketReader* rpc_reader,
                                     gkg::SocketHeader&,
                                     gkg::SocketStream& stream )
{

  gkg::RpcCommandReader* reader = 
                           dynamic_cast< gkg::RpcCommandReader* >( rpc_reader );

  if ( reader )
  {
  
    int32_t id;
    
    stream >> id;
    
    gkg::RpcCallback* callback = reader->getCallback( id );
    
    if ( callback )
    {
    
      callback->receive( stream );
    
    }
  
  }

}


void gkg::RpcCommandReader::add( gkg::RpcCallback* callback )
{

  if ( callback )
  {

    _callbacks.insert( std::make_pair( callback->getId(), callback ) );

  }

}


void gkg::RpcCommandReader::remove( gkg::RpcCallback* callback )
{

  std::map< int32_t, gkg::RpcCallback* >::iterator
    it = _callbacks.find( callback->getId() );
    
  if ( it != _callbacks.end() )
  {
  
    _callbacks.erase( it );
  
  }

}


gkg::RpcCallback* gkg::RpcCommandReader::getCallback( int32_t id )
{

  std::map< int32_t, gkg::RpcCallback* >::const_iterator
    it = _callbacks.find( id );
    
  if ( it != _callbacks.end() )
  {
  
    return it->second;
    
  }
  
  return NULL;

}


void gkg::RpcCommandReader::connectionClosed( int32_t )
{
}


bool gkg::RpcCommandReader::getBinary()
{

  return true;
  
}
