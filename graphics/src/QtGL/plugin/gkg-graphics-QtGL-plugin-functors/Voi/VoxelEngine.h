#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_VoxelEngine_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_VoxelEngine_h_


#include <gkg-processing-coordinates/Vector3d.h>

#include <list>


namespace gkg
{


class VoiData;
class Chunk;


class VoxelEngine
{

  public:

    VoxelEngine();
    virtual ~VoxelEngine();

    void initialize( VoiData& data );
    void render( VoiData& data );
    void update( VoiData& data, std::list< Vector3d< int32_t > >& points );
    void updateVisible( int32_t label, bool show );
    void leave();

  private:

    Chunk* _chunks;
    std::vector< std::vector< Chunk* > > _chunkAccessor;
    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;

};


}


#endif
