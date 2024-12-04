#ifndef _gkg_graphics_X11_progressive_ProgressiveImageContext_h_
#define _gkg_graphics_X11_progressive_ProgressiveImageContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Raster;
class ProgressiveImage;


class ProgressiveImageContext : public LoopContext< int32_t >
{

  public:

    enum Canal
    {

      Red = 0,
      Green,
      Blue,
      CanalCount

    };

    ProgressiveImageContext( ProgressiveImage* progressiveImage );
    virtual ~ProgressiveImageContext();

    virtual int32_t getStepCount() const;
    virtual void increaseStepCount();
    virtual void resetStepCount();

    virtual int32_t getOffsetX() const;
    virtual int32_t getOffsetY() const;
    virtual int32_t getZoom() const;

    virtual void doIt( int32_t startIndex, int32_t count );
    virtual void fill( Raster* raster );

  private:

    virtual void updatePaddingInformation();
    virtual void pad();

    static void getPaddingInformation( int32_t stepDone, 
                                       int32_t x, int32_t y, 
                                       int32_t zoom,
                                       int32_t &offsetX, int32_t& offsetY, 
                                       int32_t& rzoom );
    template < class T >
    static void composeZoomedBuffer( T* to,
                                     int32_t offsetX, int32_t offsetY, 
                                     int32_t zoom,
                                     PixelCoordinate toPixelWidth,
                                     PixelCoordinate toPixelHeight,
                                     T* from,
                                     int32_t skip );

    template < class T >
    static void zoom2( T* to,
                       PixelCoordinate fromPixelWidth,
                       PixelCoordinate fromPixelHeight,
                       PixelCoordinate toPixelWidth,
                       PixelCoordinate toPixelHeight );

    ProgressiveImage* _progressiveImage;

    FloatColorIntensity* _composeBuffer[ CanalCount ];
    float* _composeAlphaBuffer;

    FloatColorIntensity* _stepBuffer[ CanalCount ];
    float* _stepAlphaBuffer;

    int32_t _stepCount;
    int32_t _offsetX;
    int32_t _offsetY;
    int32_t _zoom;

};


}


#endif
