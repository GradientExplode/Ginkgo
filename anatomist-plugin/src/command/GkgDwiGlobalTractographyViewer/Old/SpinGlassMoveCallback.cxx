#include "SpinGlassMoveCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassMoveCallback::SpinGlassMoveCallback(
                                                 void* parameter, bool verbose )
                          : gkg::RpcCallback( gkg::SPIN_GLASS_MOVE_CALLBACK,
                                              parameter ),
                            _verbose( verbose )
{
}


gkg::SpinGlassMoveCallback::~SpinGlassMoveCallback()
{
}


void gkg::SpinGlassMoveCallback::receive( gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the spin glass identity
    uint64_t spinGlassIdentity;
    stream.read( ( char* )&spinGlassIdentity, sizeof( uint64_t ) );

    // receving the new spin glass position
    gkg::Vector3d< float > newSpinGlassPosition;
    stream.read( ( char* )&( newSpinGlassPosition.x ), sizeof( float ) );
    stream.read( ( char* )&( newSpinGlassPosition.y ), sizeof( float ) );
    stream.read( ( char* )&( newSpinGlassPosition.z ), sizeof( float ) );

    // receving the new spin glass orientation
    gkg::Vector3d< float > newSpinGlassOrientation;
    stream.read( ( char* )&( newSpinGlassOrientation.x ), sizeof( float ) );
    stream.read( ( char* )&( newSpinGlassOrientation.y ), sizeof( float ) );
    stream.read( ( char* )&( newSpinGlassOrientation.z ), sizeof( float ) );

    if ( globalTractographyService )
    {

      globalTractographyService->moveSpinGlass( spinGlassIdentity,
                                                newSpinGlassPosition,
                                                newSpinGlassOrientation );

      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "---------------- Spin Glass Move Callback -----------------"
                << std::endl;
      std::cout << " spin glass identity : " << spinGlassIdentity << std::endl;
      std::cout << " new spin glass position : "
                << newSpinGlassPosition << std::endl;
      std::cout << " new spin glass orientation : "
                << newSpinGlassOrientation << std::endl;

    }

  }
  GKG_CATCH( "void gkg::SpinGlassMoveCallback::receive( "
             "gkg::SocketStream& stream )" );

}
