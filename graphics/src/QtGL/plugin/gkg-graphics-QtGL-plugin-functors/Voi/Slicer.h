#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Slicer_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Slicer_h_


#include <gkg-core-cppext/StdInt.h>

#include <GL/gl.h>


namespace gkg
{


class VoiData;


class Slicer
{

  public:

    enum SliceType
    {

      Axial = 0,
      Sagittal,
      Coronal

    };

    Slicer();
    virtual ~Slicer();

    void initialize( VoiData& data );
    void getTexelCoordinates( int32_t x, int32_t y, int32_t z,
                              double& x0, double& y0, double& z0,
                              double& x1, double& y1, double& z1 );
    void getTextureCoordinate( int32_t x, int32_t y, int32_t z,
                               double& r, double& s, double& t );
    void getVolumeCoordinate( double x, double y, double z,
                              int32_t& posX, int32_t& posY, int32_t& posZ );
    void setTexture( SliceType type, uint8_t* buffer );
    GLuint getTextureId( SliceType type );

    double getMax();
    double getSlabX();
    double getStartX();
    double getStopX();
    double getSlabY();
    double getStartY();
    double getStopY();
    double getSlabZ();
    double getStartZ();
    double getStopZ();

  private:

    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;

    double _deltaX;
    double _deltaY;
    double _deltaZ;

    double _max;
    double _slabX;
    double _startX;
    double _stopX;
    double _slabY;
    double _startY;
    double _stopY;
    double _slabZ;
    double _startZ;
    double _stopZ;

    GLuint _sliceTexture[ 3 ];

};


}


#endif
