#ifndef _gkg_dmri_global_tractography_SpinGlassConnectionCreationCommand_h_
#define _gkg_dmri_global_tractography_SpinGlassConnectionCreationCommand_h_


#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-core-pattern/RCPointer.h>

namespace gkg
{


class SpinGlassConnectionCreationCommand : public RpcCommand
{

  public:

    SpinGlassConnectionCreationCommand(
                  const RCPointer< SpinGlassConnection >& spinGlassConnection );

    virtual ~SpinGlassConnectionCreationCommand();

    void send( SocketStream& server );

  private:

    RCPointer< SpinGlassConnection > _spinGlassConnection;

};


}


#endif
