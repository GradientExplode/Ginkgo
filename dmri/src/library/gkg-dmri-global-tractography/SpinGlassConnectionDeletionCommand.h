#ifndef _gkg_dmri_global_tractography_SpinGlassConnectionDeletionCommand_h_
#define _gkg_dmri_global_tractography_SpinGlassConnectionDeletionCommand_h_


#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-core-pattern/RCPointer.h>

namespace gkg
{


class SpinGlassConnectionDeletionCommand : public RpcCommand
{

  public:

    SpinGlassConnectionDeletionCommand(
                  const RCPointer< SpinGlassConnection >& spinGlassConnection );

    virtual ~SpinGlassConnectionDeletionCommand();

    void send( SocketStream& server );

  private:

    RCPointer< SpinGlassConnection > _spinGlassConnection;

};


}


#endif
