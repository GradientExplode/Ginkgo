#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_ResetCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_ResetCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class ResetCallback : public RpcCallback
{

  public:

    ResetCallback( void* parameter, bool verbose );
    virtual ~ResetCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

