#ifndef _gkg_dmri_GkgDwiInformationViewer_SpinGlassConnectionInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_SpinGlassConnectionInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include "DefaultInformationCallback.h"


namespace gkg
{


class SpinGlassConnectionInformationCallback : public RpcCallback,
                                               public DefaultInformationCallback
{

  public:

    SpinGlassConnectionInformationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassConnectionInformationCallback();

    void receive( SocketStream& stream );

};


}


#endif

