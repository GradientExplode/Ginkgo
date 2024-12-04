#ifndef _gkg_dmri_global_tractography_SpinGlassCreationCommand_h_
#define _gkg_dmri_global_tractography_SpinGlassCreationCommand_h_


#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-core-pattern/RCPointer.h>

namespace gkg
{


class SpinGlassCreationCommand : public RpcCommand
{

  public:

    SpinGlassCreationCommand( const RCPointer< SpinGlass >& spinGlass );
    virtual ~SpinGlassCreationCommand();

    void send( SocketStream& server );

  private:

    RCPointer< SpinGlass > _spinGlass;

};


}


#endif
