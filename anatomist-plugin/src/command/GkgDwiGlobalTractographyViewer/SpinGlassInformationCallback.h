#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassInformationCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_SpinGlassInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class SpinGlassInformationCallback : public RpcCallback
{

  public:

    SpinGlassInformationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassInformationCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

