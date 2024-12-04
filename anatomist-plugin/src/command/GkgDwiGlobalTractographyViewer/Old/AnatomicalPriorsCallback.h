#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AnatomicalPriorsCallback_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AnatomicalPriorsCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class AnatomicalPriorsCallback : public RpcCallback
{

  public:

    AnatomicalPriorsCallback( void* parameter, bool verbose );
    virtual ~AnatomicalPriorsCallback();

    void receive( SocketStream& stream );

  private:

    bool _verbose;

};


}


#endif

