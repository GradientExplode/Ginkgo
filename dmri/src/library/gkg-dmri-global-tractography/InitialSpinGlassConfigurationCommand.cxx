#include "InitialSpinGlassConfigurationCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::InitialSpinGlassConfigurationCommand::InitialSpinGlassConfigurationCommand(
               const std::set< gkg::RCPointer< gkg::SpinGlass > >& spinGlasses )
                                          : gkg::RpcCommand(
                               gkg::INITIAL_SPIN_GLASS_CONFIGURATION_CALLBACK ),
                                           _spinGlasses( spinGlasses )
{
}


gkg::InitialSpinGlassConfigurationCommand::
                                         ~InitialSpinGlassConfigurationCommand()
{
}


void gkg::InitialSpinGlassConfigurationCommand::send(
                                                     gkg::SocketStream& server )
{

  try
  {

    if ( server.good() )
    {

      gkg::RpcCommand::send( server );

      // sending the spin glass count
      int32_t spinGlassCount = ( int32_t )_spinGlasses.size();
      server.write( ( const char* )&spinGlassCount, sizeof( int32_t ) );

      std::set< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
        s = _spinGlasses.begin(),
        se = _spinGlasses.end();

      while ( s != se )
      {

        // sending the spin glass identity
        uint64_t spinGlassIdentity = ( *s )->getIdentity();
        server.write( ( const char* )&spinGlassIdentity, sizeof( uint64_t ) );

        // sending the spin glass position
        const gkg::Vector3d< float >& spinGlassPosition = ( *s )->getPosition();
        server.write( ( const char* )&( spinGlassPosition.x ),
                      sizeof( float ) );
        server.write( ( const char* )&( spinGlassPosition.y ),
                      sizeof( float ) );
        server.write( ( const char* )&( spinGlassPosition.z ),
                      sizeof( float ) );

        // sending the spin glass orientation
        const gkg::Vector3d< float >& spinGlassOrientation =
                                                       ( *s )->getOrientation();
        server.write( ( const char* )&( spinGlassOrientation.x ),
                      sizeof( float ) );
        server.write( ( const char* )&( spinGlassOrientation.y ),
                      sizeof( float ) );
        server.write( ( const char* )&( spinGlassOrientation.z ),
                      sizeof( float ) );

        // sending the spin glass length
        const float& spinGlassLength = ( *s )->getLength();
        server.write( ( const char* )&( spinGlassLength ), sizeof( float ) );
 
        ++ s;

      }

    }

  }
  GKG_CATCH( "void gkg::InitialSpinGlassConfigurationCommand::send( "
             "gkg::SocketStream& server )" );

}

