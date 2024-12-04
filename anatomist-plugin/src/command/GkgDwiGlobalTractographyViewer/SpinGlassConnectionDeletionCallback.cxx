#include "SpinGlassConnectionDeletionCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassConnectionDeletionCallback::SpinGlassConnectionDeletionCallback(
                                                 void* parameter, bool verbose )
                                        : gkg::RpcCallback(
                                   gkg::SPIN_GLASS_CONNECTION_DELETION_CALLBACK,
                                   parameter ),
                                          _verbose( verbose )
{
}


gkg::SpinGlassConnectionDeletionCallback::~SpinGlassConnectionDeletionCallback()
{
}


void gkg::SpinGlassConnectionDeletionCallback::receive(
                                                     gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the spin glass connection identity 
    uint64_t spinGlassConnectionIdentity;
    stream.read( ( char* )&spinGlassConnectionIdentity, sizeof( uint64_t ) );

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "--------- Spin Glass Connection Deletion Callback ---------"
                << std::endl;
      std::cout << " spin glass connection identity : "
                << spinGlassConnectionIdentity << std::endl;

    }

    if ( globalTractographyService )
    {

      globalTractographyService->deleteSpinGlassConnection(
                                                  spinGlassConnectionIdentity );

      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassConnectionDeletionCallback::receive( "
             "gkg::SocketStream& stream )" );

}
