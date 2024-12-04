#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Chunk_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Chunk_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Block.h>

#include <GL/gl.h>
#include <vector>


#define CHUNK_SIZE 16


namespace gkg
{


class VoiData;


class Chunk
{

  public:

    Chunk();
    virtual ~Chunk();

    int32_t getActiveBlockCount();

    void setup( VoiData& data, int32_t x, int32_t y, int32_t z );
    void updateBlock( int32_t x, int32_t y, int32_t z, int32_t label );
    void updateBlocks( int32_t label, bool show );
    void render( VoiData& data );
    void leave();

  protected:

    void createMesh( VoiData& data );

  private:

    Block _blocks[ CHUNK_SIZE ][ CHUNK_SIZE ][ CHUNK_SIZE ];
    int32_t _activeBlockCount;
    int32_t _nX;
    int32_t _nY;
    int32_t _nZ;
    int32_t _posX;
    int32_t _posY;
    int32_t _posZ;
    bool _needUpdate;

    std::vector< GLfloat > _colorVertex;
    std::vector< GLuint > _indice;

    GLuint _buffers[ 2 ];

};


}


#endif
