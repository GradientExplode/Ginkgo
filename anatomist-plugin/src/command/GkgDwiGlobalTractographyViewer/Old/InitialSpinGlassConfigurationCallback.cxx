#include "InitialSpinGlassConfigurationCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::InitialSpinGlassConfigurationCallback::
          InitialSpinGlassConfigurationCallback( void* parameter, bool verbose )
                                          : gkg::RpcCallback(
                                 gkg::INITIAL_SPIN_GLASS_CONFIGURATION_CALLBACK,
                                 parameter ),
                                            _verbose( verbose )
{
}


gkg::InitialSpinGlassConfigurationCallback::
                                        ~InitialSpinGlassConfigurationCallback()
{
}


void gkg::InitialSpinGlassConfigurationCallback::receive(
                                                     gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the spin glass count
    int32_t spinGlassCount;
    stream.read( ( char* )&spinGlassCount, sizeof( int32_t ) );

    int32_t i = 0;
    for ( i = 0; i < spinGlassCount; i++ )
    {

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

      }

    }

    if ( globalTractographyService )
    {

      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

    /////////// display ////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "-------- Initial Spin Glass Configuration Callback --------"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::InitialSpinGlassConfigurationCallback::receive( "
             "gkg::SocketStream& stream )" );

}
