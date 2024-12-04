#include "ResetCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include "GlobalTractographyEvent.h"
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::ResetCallback::ResetCallback( void* parameter, bool verbose )
                   : gkg::RpcCallback( gkg::RESET_CALLBACK, parameter ),
                     _verbose( verbose )
{
}


gkg::ResetCallback::~ResetCallback()
{
}


void gkg::ResetCallback::receive( gkg::SocketStream& /* stream */ )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "--------------------- Reset Callback ----------------------"
                << std::endl;

    }

    if ( globalTractographyService )
    {

      globalTractographyService->resetSpinGlassAndConnectionMap();
      globalTractographyService->postEvent( GT_OPENGL_OFFSET );

    }

  }
  GKG_CATCH( "void gkg::ResetCallback::receive( gkg::SocketStream& stream )" );

}

