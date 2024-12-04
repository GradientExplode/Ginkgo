#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassMoveCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassMoveCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class SpinGlassMoveCallback : public RpcCallback
{

  public:

    SpinGlassMoveCallback( void* parameter, bool verbose );
    virtual ~SpinGlassMoveCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

