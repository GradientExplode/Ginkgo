#include "SpinGlassCreationCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassCreationCallback::SpinGlassCreationCallback(
                                                 void* parameter, bool verbose )
                              : gkg::RpcCallback(
                                              gkg::SPIN_GLASS_CREATION_CALLBACK,
                                              parameter ),
                                _verbose( verbose )
{
}


gkg::SpinGlassCreationCallback::~SpinGlassCreationCallback()
{
}


void gkg::SpinGlassCreationCallback::receive( gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the spin glass identity
    uint64_t spinGlassIdentity;
    stream.read( ( char* )&spinGlassIdentity, sizeof( uint64_t ) );

    // receving the spin glass position
    gkg::Vector3d< float > spinGlassPosition;
    stream.read( ( char* )&( spinGlassPosition.x ), sizeof( float ) );
    stream.read( ( char* )&( spinGlassPosition.y ), sizeof( float ) );
    stream.read( ( char* )&( spinGlassPosition.z ), sizeof( float ) );

    // receving the spin glass orientation
    gkg::Vector3d< float > spinGlassOrientation;
    stream.read( ( char* )&( spinGlassOrientation.x ), sizeof( float ) );
    stream.read( ( char* )&( spinGlassOrientation.y ), sizeof( float ) );
    stream.read( ( char* )&( spinGlassOrientation.z ), sizeof( float ) );

    if ( globalTractographyService )
    {

      globalTractographyService->createSpinGlass( spinGlassIdentity,
                                                  spinGlassPosition,
                                                  spinGlassOrientation );

      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "-------------- Spin Glass Creation Callback ---------------"
                << std::endl;
      std::cout << " spin glass identity : " << spinGlassIdentity << std::endl;
      std::cout << " spin glass position : " << spinGlassPosition << std::endl;
      std::cout << " spin glass orientation : "
                << spinGlassOrientation << std::endl;

    }

  }
  GKG_CATCH( "void gkg::SpinGlassCreationCallback::receive( "
             "gkg::SocketStream& stream )" );

}
