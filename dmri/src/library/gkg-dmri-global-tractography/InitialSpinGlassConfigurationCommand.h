#ifndef _gkg_dmri_global_tractography_InitialSpinGlassConfigurationCommand_h_
#define _gkg_dmri_global_tractography_InitialSpinGlassConfigurationCommand_h_


#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-core-pattern/RCPointer.h>
#include <set>


namespace gkg
{


class InitialSpinGlassConfigurationCommand : public RpcCommand
{

  public:

    InitialSpinGlassConfigurationCommand(
                        const std::set< RCPointer< SpinGlass > >& spinGlasses );

    virtual ~InitialSpinGlassConfigurationCommand();

    void send( SocketStream& server );

  private:

    const std::set< RCPointer< SpinGlass > >& _spinGlasses;

};


}


#endif
