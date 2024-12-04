#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassConnectionDeletionCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassConnectionDeletionCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class SpinGlassConnectionDeletionCallback : public RpcCallback
{

  public:

    SpinGlassConnectionDeletionCallback( void* parameter, bool verbose );
    virtual ~SpinGlassConnectionDeletionCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

