#ifndef _gkg_dmri_GkgDwiInformationViewer_SpinGlassCreationInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_SpinGlassCreationInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include "DefaultInformationCallback.h"


namespace gkg
{


class SpinGlassCreationInformationCallback : public RpcCallback,
                                             public DefaultInformationCallback
{

  public:

    SpinGlassCreationInformationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassCreationInformationCallback();

    void receive( SocketStream& stream );

};


}


#endif

