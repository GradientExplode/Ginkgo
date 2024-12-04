#include "ResetCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::ResetCommand::ResetCommand()
                  : gkg::RpcCommand( gkg::RESET_CALLBACK )
{
}


gkg::ResetCommand::~ResetCommand()
{
}


void gkg::ResetCommand::send( gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

    }

  }
  GKG_CATCH( "void gkg::ResetCommand::send( gkg::SocketStream& server )" );

}

