#include <gkg-processing-rpc/RpcTransferVolumeCallback.h>
#include <gkg-processing-container/Volume_i.h>


template < class T >
gkg::RpcTransferVolumeCallback< T >::RpcTransferVolumeCallback( 
                                                       int32_t id,
                                                       gkg::Volume< T >& volume,
                                                       void* parameter )
			            : gkg::RpcCallback( id, parameter ),
			              _volume( volume )
{
}


template < class T >
void gkg::RpcTransferVolumeCallback< T >::receive( gkg::SocketStream& stream )
{

  int32_t sizeX, sizeY, sizeZ, sizeT;
  double resolutionX, resolutionY, resolutionZ, resolutionT;

  stream.read( (char*)&sizeX, sizeof( int32_t ) );
  stream.read( (char*)&sizeY, sizeof( int32_t ) );
  stream.read( (char*)&sizeZ, sizeof( int32_t ) );
  stream.read( (char*)&sizeT, sizeof( int32_t ) );
    
  stream.read( (char*)&resolutionX, sizeof( double ) );
  stream.read( (char*)&resolutionY, sizeof( double ) );
  stream.read( (char*)&resolutionZ, sizeof( double ) );
  stream.read( (char*)&resolutionT, sizeof( double ) );  

  _volume.reallocate( sizeX, sizeY, sizeZ, sizeT );
  _volume.getHeader().addAttribute( "resolutionX", resolutionX );
  _volume.getHeader().addAttribute( "resolutionY", resolutionY );
  _volume.getHeader().addAttribute( "resolutionZ", resolutionZ );
  _volume.getHeader().addAttribute( "resolutionT", resolutionT );
    
  stream.read( ( char* )&_volume( 0 ), _volume.getSizeXYZT() * sizeof( T ) );

}
