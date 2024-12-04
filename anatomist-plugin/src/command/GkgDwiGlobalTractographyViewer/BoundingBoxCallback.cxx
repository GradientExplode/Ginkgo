#include "BoundingBoxCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::BoundingBoxCallback::BoundingBoxCallback( void* parameter, bool verbose )
                         : gkg::RpcCallback( gkg::BOUNDING_BOX_CALLBACK,
                                             parameter ),
                           _verbose( verbose )
{
}


gkg::BoundingBoxCallback::~BoundingBoxCallback()
{
}


void gkg::BoundingBoxCallback::receive( gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the lower corner
    gkg::Vector3d< int32_t > lowerCorner;
    stream.read( ( char* )&( lowerCorner.x ), sizeof( int32_t ) );
    stream.read( ( char* )&( lowerCorner.y ), sizeof( int32_t ) );
    stream.read( ( char* )&( lowerCorner.z ), sizeof( int32_t ) );

    // receiving the upper corner
    gkg::Vector3d< int32_t > upperCorner;
    stream.read( ( char* )&( upperCorner.x ), sizeof( int32_t ) );
    stream.read( ( char* )&( upperCorner.y ), sizeof( int32_t ) );
    stream.read( ( char* )&( upperCorner.z ), sizeof( int32_t ) );

    // receiving the ROI resolution
    gkg::Vector3d< double > roiResolution;
    stream.read( ( char* )&( roiResolution.x ), sizeof( double ) );
    stream.read( ( char* )&( roiResolution.y ), sizeof( double ) );
    stream.read( ( char* )&( roiResolution.z ), sizeof( double ) );

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "---------------- Bounding Box Callback --------------------"
                << std::endl;
      std::cout << "  lowerCorner : " << lowerCorner << std::endl;
      std::cout << "  upperCorner : " << upperCorner << std::endl;
      std::cout << "  roiResolution : " << roiResolution << std::endl;

    }

    if ( globalTractographyService )
    {

      globalTractographyService->setSpinGlassAndConnectionMapBoundingBox(
                                                                lowerCorner,
                                                                upperCorner,
                                                                roiResolution );

    }

  }
  GKG_CATCH( "void gkg::BoundingBoxCallback::receive( "
             "gkg::SocketStream& stream )" );

}
