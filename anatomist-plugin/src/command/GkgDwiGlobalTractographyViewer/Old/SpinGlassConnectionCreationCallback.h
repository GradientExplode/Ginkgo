#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassConnectionCreationCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassConnectionCreationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class SpinGlassConnectionCreationCallback : public RpcCallback
{

  public:

    SpinGlassConnectionCreationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassConnectionCreationCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

