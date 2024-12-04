#ifndef _gkg_dmri_global_tractography_SpinGlassInformationCommand_h_
#define _gkg_dmri_global_tractography_SpinGlassInformationCommand_h_


#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class SpinGlassInformationCommand : public RpcCommand
{

  public:

    SpinGlassInformationCommand( const RCPointer< SpinGlass >& spinGlass );

    virtual ~SpinGlassInformationCommand();

    void send( SocketStream& server );

  private:

    RCPointer< SpinGlass > _spinGlass;

};


}


#endif
