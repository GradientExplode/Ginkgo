#ifndef _gkg_dmri_global_tractography_SpinGlassMoveCommand_h_
#define _gkg_dmri_global_tractography_SpinGlassMoveCommand_h_


#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class SpinGlassMoveCommand : public RpcCommand
{

  public:

    SpinGlassMoveCommand( const RCPointer< SpinGlass >& spinGlass );
    virtual ~SpinGlassMoveCommand();

    void send( SocketStream& server );

  private:

    RCPointer< SpinGlass > _spinGlass;

};


}


#endif
