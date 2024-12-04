#ifndef _gkg_dmri_GkgDwiInformationViewer_SpinGlassOptimalMoveInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_SpinGlassOptimalMoveInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include "DefaultInformationCallback.h"


namespace gkg
{


class SpinGlassOptimalMoveInformationCallback :
                                               public RpcCallback,
                                               public DefaultInformationCallback
{

  public:

    SpinGlassOptimalMoveInformationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassOptimalMoveInformationCallback();

    void receive( SocketStream& stream );

};


}


#endif

