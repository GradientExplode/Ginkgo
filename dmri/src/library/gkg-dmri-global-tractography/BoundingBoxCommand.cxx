#include "BoundingBoxCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::BoundingBoxCommand::BoundingBoxCommand(
                                  const gkg::Vector3d< int32_t >& lowerCorner,
                                  const gkg::Vector3d< int32_t >& upperCorner,
                                  const gkg::Vector3d< double >& roiResolution )
                       : gkg::RpcCommand( gkg::BOUNDING_BOX_CALLBACK ),
                         _lowerCorner( lowerCorner ),
                         _upperCorner( upperCorner ),
                         _roiResolution( roiResolution )
{
}


gkg::BoundingBoxCommand::~BoundingBoxCommand()
{
}


void gkg::BoundingBoxCommand::send( gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

      // sending the lower corner
      server.write( ( const char* )&( _lowerCorner.x ), sizeof( int32_t ) );
      server.write( ( const char* )&( _lowerCorner.y ), sizeof( int32_t ) );
      server.write( ( const char* )&( _lowerCorner.z ), sizeof( int32_t ) );

      // sending the upper corner
      server.write( ( const char* )&( _upperCorner.x ), sizeof( int32_t ) );
      server.write( ( const char* )&( _upperCorner.y ), sizeof( int32_t ) );
      server.write( ( const char* )&( _upperCorner.z ), sizeof( int32_t ) );

      // sending the ROI resolution
      server.write( ( const char* )&( _roiResolution.x ), sizeof( double ) );
      server.write( ( const char* )&( _roiResolution.y ), sizeof( double ) );
      server.write( ( const char* )&( _roiResolution.z ), sizeof( double ) );

    }

  }
  GKG_CATCH( "void gkg::BoundingBoxCommand::send( gkg::SocketStream& server )" );

}

