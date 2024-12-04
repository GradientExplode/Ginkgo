#include "SpinGlassInformationCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::SpinGlassInformationCallback::SpinGlassInformationCallback(
                                                 void* parameter, bool verbose )
                                  : gkg::RpcCallback(
                                           gkg::SPIN_GLASS_INFORMATION_CALLBACK,
                                           parameter ),
                                    _verbose( verbose )
{
}


gkg::SpinGlassInformationCallback::~SpinGlassInformationCallback()
{
}


void gkg::SpinGlassInformationCallback::receive( gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >&
      spinGlassAndConnectionMap =
                      globalTractographyService->getSpinGlassAndConnectionMap();

    // receiving the spin glass identity
    uint64_t spinGlassIdentity;
    stream.read( ( char* )&spinGlassIdentity, sizeof( uint64_t ) );

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "------------ Spin Glass Information Callback --------------"
                << std::endl;
      std::cout << " spin glass identity : " << spinGlassIdentity << std::endl;

    }

    if ( globalTractographyService && !spinGlassAndConnectionMap.isNull() )
    {

      spinGlassAndConnectionMap->setCurrentSpinGlass( spinGlassIdentity );
      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassInformationCallback::receive( "
             "gkg::SocketStream& stream )" );

}
