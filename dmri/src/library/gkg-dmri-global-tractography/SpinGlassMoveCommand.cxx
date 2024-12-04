#include "SpinGlassMoveCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::SpinGlassMoveCommand::SpinGlassMoveCommand(
                             const gkg::RCPointer< gkg::SpinGlass >& spinGlass )
                         : gkg::RpcCommand( gkg::SPIN_GLASS_MOVE_CALLBACK ),
                           _spinGlass( spinGlass )
{
}


gkg::SpinGlassMoveCommand::~SpinGlassMoveCommand()
{
}


void gkg::SpinGlassMoveCommand::send( gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

      // sending the spin glass identity
      uint64_t spinGlassIdentity = _spinGlass->getIdentity();
      server.write( ( const char* )&spinGlassIdentity, sizeof( uint64_t ) );

      // sending the new spin glass position
      const gkg::Vector3d< float >& newSpinGlassPosition =
                                                      _spinGlass->getPosition();
      server.write( ( const char* )&( newSpinGlassPosition.x ),
                    sizeof( float ) );
      server.write( ( const char* )&( newSpinGlassPosition.y ),
                    sizeof( float ) );
      server.write( ( const char* )&( newSpinGlassPosition.z ),
                    sizeof( float ) );

      // sending the new spin glass orientation
      const gkg::Vector3d< float >& newSpinGlassOrientation =
                                                   _spinGlass->getOrientation();
      server.write( ( const char* )&( newSpinGlassOrientation.x ),
                    sizeof( float ) );
      server.write( ( const char* )&( newSpinGlassOrientation.y ),
                    sizeof( float ) );
      server.write( ( const char* )&( newSpinGlassOrientation.z ),
                    sizeof( float ) );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassMoveCommand::send( "
             "gkg::SocketStream& server )" );

}

