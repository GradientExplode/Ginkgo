#ifndef _gkg_dmri_global_tractography_ResetCommand_h_
#define _gkg_dmri_global_tractography_ResetCommand_h_


#include <gkg-communication-rpc/RpcCommand.h>

namespace gkg
{


class ResetCommand : public RpcCommand
{

  public:

    ResetCommand();

    virtual ~ResetCommand();

    void send( SocketStream& server );

};


}


#endif
