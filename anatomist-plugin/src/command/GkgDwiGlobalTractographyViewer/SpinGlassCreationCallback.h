#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassCreationCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassCreationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class SpinGlassCreationCallback : public RpcCallback
{

  public:

    SpinGlassCreationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassCreationCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

