#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_InitialSpinGlassConfigurationCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_InitialSpinGlassConfigurationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class InitialSpinGlassConfigurationCallback : public RpcCallback
{

  public:

    InitialSpinGlassConfigurationCallback( void* parameter, bool verbose );
    virtual ~InitialSpinGlassConfigurationCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

