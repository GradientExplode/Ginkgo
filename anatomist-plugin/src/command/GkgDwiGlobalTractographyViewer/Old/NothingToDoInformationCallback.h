#ifndef _gkg_dmri_GkgDwiInformationViewer_NothingToDoInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_NothingToDoInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include "DefaultInformationCallback.h"


namespace gkg
{


class NothingToDoInformationCallback : public RpcCallback,
                                       public DefaultInformationCallback
{

  public:

    NothingToDoInformationCallback( void* parameter, bool verbose );
    virtual ~NothingToDoInformationCallback();

    void receive( SocketStream& stream );

};


}


#endif

