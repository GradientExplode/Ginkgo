#include "SpinGlassCreationCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::SpinGlassCreationCommand::SpinGlassCreationCommand(
                             const gkg::RCPointer< gkg::SpinGlass >& spinGlass )
                             : gkg::RpcCommand(
                                            gkg::SPIN_GLASS_CREATION_CALLBACK ),
                               _spinGlass( spinGlass )
{
}


gkg::SpinGlassCreationCommand::~SpinGlassCreationCommand()
{
}


void gkg::SpinGlassCreationCommand::send( gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

      // sending the spin glass identity
      uint64_t spinGlassIdentity = _spinGlass->getIdentity();
      server.write( ( const char* )&spinGlassIdentity, sizeof( uint64_t ) );

      // sending the spin glass position
      const gkg::Vector3d< float >& spinGlassPosition =
                                                      _spinGlass->getPosition();
      server.write( ( const char* )&( spinGlassPosition.x ), sizeof( float ) );
      server.write( ( const char* )&( spinGlassPosition.y ), sizeof( float ) );
      server.write( ( const char* )&( spinGlassPosition.z ), sizeof( float ) );

      // sending the spin glass orientation
      const gkg::Vector3d< float >& spinGlassOrientation =
                                                   _spinGlass->getOrientation();
      server.write( ( const char* )&( spinGlassOrientation.x ),
                    sizeof( float ) );
      server.write( ( const char* )&( spinGlassOrientation.y ),
                    sizeof( float ) );
      server.write( ( const char* )&( spinGlassOrientation.z ),
                    sizeof( float ) );

      // sending the spin glass length
      float spinGlassLength = _spinGlass->getLength();
      server.write( ( const char* )&spinGlassLength, sizeof( float ) );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassCreationCommand::send( "
             "gkg::SocketStream& server )" );

}

