#ifndef _gkg_processing_rpc_RpcTransferVolumeCommand_i_h_
#define _gkg_processing_rpc_RpcTransferVolumeCommand_i_h_

#include <gkg-processing-rpc/RpcTransferVolumeCommand.h>
#include <gkg-processing-container/Volume_i.h>


template< class T >
gkg::RpcTransferVolumeCommand< T >::RpcTransferVolumeCommand( int32_t id )
				   : gkg::RpcCommand( id )
{
}


template < class T >
gkg::Volume< T >& gkg::RpcTransferVolumeCommand< T >::getVolume()
{

  return _volume;

}


template < class T >
void gkg::RpcTransferVolumeCommand< T >::send( gkg::SocketStream& server )
{

  if ( server.good() )
  {
  
    gkg::RpcCommand::send( server );
  
    double resolutionX = 1.0, resolutionY = 1.0;
    double resolutionZ = 1.0, resolutionT = 1.0;

    if ( _volume.getHeader().hasAttribute( "resolutionX" ) )
    {
  
      _volume.getHeader().getAttribute( "resolutionX", resolutionX );
    
    }
    if ( _volume.getHeader().hasAttribute( "resolutionY" ) )
    {
  
      _volume.getHeader().getAttribute( "resolutionY", resolutionY );
      
    }
    if ( _volume.getHeader().hasAttribute( "resolutionZ" ) )
    {
  
      _volume.getHeader().getAttribute( "resolutionZ", resolutionZ );
    
    }
    if ( _volume.getHeader().hasAttribute( "resolutionT" ) )
    {
  
      _volume.getHeader().getAttribute( "resolutionT", resolutionT );
    
    }
  
    int32_t sizeX = _volume.getSizeX();
    int32_t sizeY = _volume.getSizeY();
    int32_t sizeZ = _volume.getSizeZ();
    int32_t sizeT = _volume.getSizeT();

    server.write( (const char*)&sizeX, sizeof( int32_t ) );
    server.write( (const char*)&sizeY, sizeof( int32_t ) );
    server.write( (const char*)&sizeZ, sizeof( int32_t ) );
    server.write( (const char*)&sizeT, sizeof( int32_t ) );

    server.write( (const char*)&resolutionX, sizeof( double ) );
    server.write( (const char*)&resolutionY, sizeof( double ) );
    server.write( (const char*)&resolutionZ, sizeof( double ) );
    server.write( (const char*)&resolutionT, sizeof( double ) );
  
    server.writePackets( ( const char* )&_volume( 0 ), 
                         _volume.getSizeXYZT() * sizeof( T ) );

  }
  
}


#endif
