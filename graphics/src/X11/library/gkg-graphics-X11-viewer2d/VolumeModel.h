#ifndef _gkg_graphics_X11_viewer2d_VolumeModel_h_
#define _gkg_graphics_X11_viewer2d_VolumeModel_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Raster;
class Image;
class TransformFitter;
class Patch;


template < class T >
class VolumeModel : public RCGUIObject
{

  public:

    VolumeModel( const Volume< T >& volume,
                 bool verbose );
    virtual ~VolumeModel();

    int32_t getSizeX() const;
    int32_t getSizeY() const;
    int32_t getSizeZ() const;
    int32_t getSizeT() const;

    double getResolutionX() const;
    double getResolutionY() const;
    double getResolutionZ() const;
    double getResolutionT() const;

    FloatCoordinate getSpanX() const;
    FloatCoordinate getSpanY() const;

    double getMinimum() const;
    double getMaximum() const;

    double getLowerThreshold() const;
    double getUpperThreshold() const;

    void setLowerThreshold( double value );
    void setUpperThreshold( double value );

    const T& getValueAt( int32_t x,
                         int32_t y,
                         int32_t z,
                         int32_t t ) const;

    Raster* getRaster() const;
    Image* getImage() const;
    Patch* getPatch() const;

    void fillRaster( int32_t slice, int32_t rank );
    void threshold( const T& inputValue,
                    FloatColorIntensity& r,
                    FloatColorIntensity& g,
                    FloatColorIntensity& b,
                    FloatColorIntensity& a ) const;
    void update( const Volume< T >& volume, int32_t slice, int32_t rank );

  protected:

    void processMinimum();
    void processMaximum();

    Volume< T > _volume;
    bool _verbose;

    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    int32_t _sizeT;

    double _resolutionX;
    double _resolutionY;
    double _resolutionZ;
    double _resolutionT;

    double _minimum;
    double _maximum;

    double _lowerThreshold;
    double _upperThreshold;

    Raster* _raster;
    Image* _image;
    TransformFitter* _transformFitter;
    Patch* _patch;

};


}


#endif
