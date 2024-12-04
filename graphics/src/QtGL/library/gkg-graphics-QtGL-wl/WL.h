#ifndef _gkg_graphics_QtGL_wl_WL_h_
#define _gkg_graphics_QtGL_wl_WL_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T >
class WL
{

  public:

    WL();
    virtual ~WL();

    void setRange( T min, T max );
    void setWW_WL( double ww, double wl );
    void setRescale( double offset, double slope );

    uint8_t getValue( T val );

    void mouseDown( int32_t x, int32_t y );
    void mouseMove( int32_t x, int32_t y );

  private:

    double progress( double val, double delta );
    void computeLut();

    int32_t _startX;
    int32_t _startY;
    double _startWw;
    double _startWl;

    double _ww;
    double _wl;
    double _offset;
    double _slope;

    uint8_t* _lut;

};


}


#endif
