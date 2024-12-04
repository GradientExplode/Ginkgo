#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_BoundingBoxCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_BoundingBoxCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class BoundingBoxCallback : public RpcCallback
{

  public:

    BoundingBoxCallback( void* parameter, bool verbose );
    virtual ~BoundingBoxCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

