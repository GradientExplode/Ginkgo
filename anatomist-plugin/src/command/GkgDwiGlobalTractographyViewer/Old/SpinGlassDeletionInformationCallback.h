#ifndef _gkg_dmri_GkgDwiInformationViewer_SpinGlassDeletionInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_SpinGlassDeletionInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include "DefaultInformationCallback.h"


namespace gkg
{


class SpinGlassDeletionInformationCallback : public RpcCallback,
                                             public DefaultInformationCallback
{

  public:

    SpinGlassDeletionInformationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassDeletionInformationCallback();

    void receive( SocketStream& stream );

};


}


#endif

