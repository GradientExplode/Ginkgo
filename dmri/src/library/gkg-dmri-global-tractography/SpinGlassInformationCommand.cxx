#include "SpinGlassInformationCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::SpinGlassInformationCommand::SpinGlassInformationCommand(
                             const gkg::RCPointer< gkg::SpinGlass >& spinGlass )
                                 : gkg::RpcCommand(
                                         gkg::SPIN_GLASS_INFORMATION_CALLBACK ),
                                   _spinGlass( spinGlass )
{
}


gkg::SpinGlassInformationCommand::~SpinGlassInformationCommand()
{
}


void gkg::SpinGlassInformationCommand::send( gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

      // sending the spin glass identity
      uint64_t spinGlassIdentity = _spinGlass->getIdentity();
      server.write( ( const char* )&spinGlassIdentity, sizeof( uint64_t ) );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassInformationCommand::send( "
             "gkg::SocketStream& server )" );

}

