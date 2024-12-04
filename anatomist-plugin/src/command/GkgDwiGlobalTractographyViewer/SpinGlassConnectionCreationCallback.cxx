#include "SpinGlassConnectionCreationCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassConnectionCreationCallback::SpinGlassConnectionCreationCallback(
                                                 void* parameter, bool verbose )
                                        : gkg::RpcCallback(
                                   gkg::SPIN_GLASS_CONNECTION_CREATION_CALLBACK,
                                   parameter ),
                                          _verbose( verbose )
{
}


gkg::SpinGlassConnectionCreationCallback::~SpinGlassConnectionCreationCallback()
{
}


void gkg::SpinGlassConnectionCreationCallback::receive(
                                                     gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    // receiving the spin glass connection identity
    uint64_t spinGlassConnectionIdentity;
    stream.read( ( char* )&spinGlassConnectionIdentity, sizeof( uint64_t ) );

    // receiving the spin glass 1 information
    uint64_t spinGlass1Identity;
    int32_t spinGlassExtremityType1;
    stream.read( ( char* )&spinGlass1Identity, sizeof( uint64_t ) );
    stream.read( ( char* )&spinGlassExtremityType1, sizeof( int32_t ) );

    // receiving the spin glass 2 information
    uint64_t spinGlass2Identity;
    int32_t spinGlassExtremityType2;
    stream.read( ( char* )&spinGlass2Identity, sizeof( uint64_t ) );
    stream.read( ( char* )&spinGlassExtremityType2, sizeof( int32_t ) );

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "--------- Spin Glass Connection Creation Callback ---------"
                << std::endl;

      std::cout << " spin glass connection identity : "
                << spinGlassConnectionIdentity << std::endl;
      std::cout << " spin glass 1 identity : "
                << spinGlass1Identity << std::endl;
      std::cout << " spin glass 2 identity : "
                << spinGlass2Identity << std::endl;
      std::cout << " spin glass 1 extremity type : "
                << spinGlassExtremityType1 << std::endl;
      std::cout << " spin glass 2 extremity type : "
                << spinGlassExtremityType2 << std::endl;

    }

    if ( globalTractographyService )
    {

      globalTractographyService->createSpinGlassConnection(
             spinGlassConnectionIdentity,
             spinGlass1Identity,
             ( ( spinGlassExtremityType1 == +1 ) ? gkg::SpinGlass::Forward :
                                                   gkg::SpinGlass::Backward ),
             spinGlass2Identity,
             ( ( spinGlassExtremityType2 == +1 ) ? gkg::SpinGlass::Forward :
                                                   gkg::SpinGlass::Backward ) );

      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassConnectionCreationCallback::receive( "
             "gkg::SocketStream& stream )" );

}
