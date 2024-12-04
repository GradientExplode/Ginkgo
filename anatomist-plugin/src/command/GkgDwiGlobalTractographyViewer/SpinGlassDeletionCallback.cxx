#include "SpinGlassDeletionCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassDeletionCallback::SpinGlassDeletionCallback(
                                                 void* parameter, bool verbose )
                              : gkg::RpcCallback(
                                              gkg::SPIN_GLASS_DELETION_CALLBACK,
                                              parameter ),
                                _verbose( verbose )
{
}


gkg::SpinGlassDeletionCallback::~SpinGlassDeletionCallback()
{
}


void gkg::SpinGlassDeletionCallback::receive( gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the spin glass identity
    uint64_t spinGlassIdentity;
    stream.read( ( char* )&spinGlassIdentity, sizeof( uint64_t ) );

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "-------------- Spin Glass Deletion Callback ---------------"
                << std::endl;
      std::cout << " spin glass identity : " << spinGlassIdentity << std::endl;

    }

    if ( globalTractographyService )
    {

      globalTractographyService->deleteSpinGlass( spinGlassIdentity );
      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassDeletionCallback::receive( "
             "gkg::SocketStream& stream )" );

}
