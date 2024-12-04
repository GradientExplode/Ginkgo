#include "SpinGlassConnectionDeletionCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::SpinGlassConnectionDeletionCommand::SpinGlassConnectionDeletionCommand(
         const gkg::RCPointer< gkg::SpinGlassConnection >& spinGlassConnection )
                                       : gkg::RpcCommand(
                                 gkg::SPIN_GLASS_CONNECTION_DELETION_CALLBACK ),
                                         _spinGlassConnection(
                                                           spinGlassConnection )
{
}


gkg::SpinGlassConnectionDeletionCommand::~SpinGlassConnectionDeletionCommand()
{
}


void gkg::SpinGlassConnectionDeletionCommand::send( gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

      // sending the spin glass connection identity
      uint64_t spinGlassConnectionIdentity =
                                            _spinGlassConnection->getIdentity();
      server.write( ( const char* )&spinGlassConnectionIdentity,
                    sizeof( uint64_t ) );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassConnectionDeletionCommand::send( "
             "gkg::SocketStream& server )" );

}

