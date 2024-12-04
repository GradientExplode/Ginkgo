#include <gkg-communication-kokkos/CreateKokkosScopeGuard.h>
#include <gkg-simulation-kokkos/Defines.h>
#include <gkg-core-exception/Exception.h>


void gkg::createKokkosScopeGuard()
{

  try
  {

    //
    // initializing Kokkos through a ScopeGuard class
    //

    Kokkos::ScopeGuard kokkosScopeGuard;

  }
  GKG_CATCH( "void gkg::createKokkosScopeGuard()" );

}




