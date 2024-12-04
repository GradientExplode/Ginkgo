#include "SpinGlassConnectionCreationCommand.h"
#include "GlobalTractographyCommandIds.h"
#include <gkg-core-exception/Exception.h>


gkg::SpinGlassConnectionCreationCommand::SpinGlassConnectionCreationCommand(
         const gkg::RCPointer< gkg::SpinGlassConnection >& spinGlassConnection )
                                       : gkg::RpcCommand(
                                 gkg::SPIN_GLASS_CONNECTION_CREATION_CALLBACK ),
                                         _spinGlassConnection(
                                                           spinGlassConnection )
{
}


gkg::SpinGlassConnectionCreationCommand::~SpinGlassConnectionCreationCommand()
{
}


void gkg::SpinGlassConnectionCreationCommand::send( gkg::SocketStream& server )
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

      // sending the spin glass 1 information
      uint64_t spinGlass1Identity = _spinGlassConnection->getSpinGlass1()->
                                                                  getIdentity();
      int32_t spinGlassExtremityType1 = 0;
      if ( _spinGlassConnection->getSpinGlassExtremityType1() ==
           gkg::SpinGlass::Forward )
      {

        spinGlassExtremityType1 = +1;

      }
      else
      {

        spinGlassExtremityType1 = -1;

      }
      server.write( ( const char* )&spinGlass1Identity, sizeof( uint64_t ) );
      server.write( ( const char* )&spinGlassExtremityType1,
                    sizeof( int32_t ) );

      // sending the spin glass 2 information
      uint64_t spinGlass2Identity = _spinGlassConnection->getSpinGlass2()->
                                                                  getIdentity();
      int32_t spinGlassExtremityType2 = 0;
      if ( _spinGlassConnection->getSpinGlassExtremityType2() ==
           gkg::SpinGlass::Forward )
      {

        spinGlassExtremityType2 = +1;

      }
      else
      {

        spinGlassExtremityType2 = -1;

      }
      server.write( ( const char* )&spinGlass2Identity, sizeof( uint64_t ) );
      server.write( ( const char* )&spinGlassExtremityType2,
                    sizeof( int32_t ) );

    }

  }
  GKG_CATCH( "void gkg::SpinGlassConnectionCreationCommand::send( "
             "gkg::SocketStream& server )" );

}

