#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassDeletionCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassDeletionCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class SpinGlassDeletionCallback : public RpcCallback
{

  public:

    SpinGlassDeletionCallback( void* parameter, bool verbose );
    virtual ~SpinGlassDeletionCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

