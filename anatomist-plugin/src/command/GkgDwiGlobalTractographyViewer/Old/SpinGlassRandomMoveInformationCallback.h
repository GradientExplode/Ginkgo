#ifndef _gkg_dmri_GkgDwiInformationViewer_SpinGlassRandomMoveInformationCallback_h_
#define _gkg_dmri_GkgDwiInformationViewer_SpinGlassRandomMoveInformationCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include "DefaultInformationCallback.h"


namespace gkg
{


class SpinGlassRandomMoveInformationCallback : public RpcCallback,
                                               public DefaultInformationCallback
{

  public:

    SpinGlassRandomMoveInformationCallback( void* parameter, bool verbose );
    virtual ~SpinGlassRandomMoveInformationCallback();

    void receive( SocketStream& stream );

};


}


#endif

