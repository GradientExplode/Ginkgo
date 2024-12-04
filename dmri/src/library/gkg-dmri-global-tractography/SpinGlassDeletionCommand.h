#ifndef _gkg_dmri_global_tractography_SpinGlassDeletionCommand_h_
#define _gkg_dmri_global_tractography_SpinGlassDeletionCommand_h_


#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-core-pattern/RCPointer.h>

namespace gkg
{


class SpinGlassDeletionCommand : public RpcCommand
{

  public:

    SpinGlassDeletionCommand( const RCPointer< SpinGlass >& spinGlass );

    virtual ~SpinGlassDeletionCommand();

    void send( SocketStream& server );

  private:

    RCPointer< SpinGlass > _spinGlass;

};


}


#endif
