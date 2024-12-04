#ifndef _gkg_dmri_global_tractography_BoundingBoxCommand_h_
#define _gkg_dmri_global_tractography_BoundingBoxCommand_h_


#include <gkg-communication-rpc/RpcCommand.h>
#include <gkg-processing-coordinates/Vector3d.h>

namespace gkg
{


class BoundingBoxCommand : public RpcCommand
{

  public:

    BoundingBoxCommand( const Vector3d< int32_t >& lowerCorner,
                        const Vector3d< int32_t >& upperCorner,
                        const Vector3d< double >& roiResolution );

    virtual ~BoundingBoxCommand();

    void send( SocketStream& server );

  private:

    Vector3d< int32_t > _lowerCorner;
    Vector3d< int32_t > _upperCorner;
    Vector3d< double > _roiResolution;

};


}


#endif
