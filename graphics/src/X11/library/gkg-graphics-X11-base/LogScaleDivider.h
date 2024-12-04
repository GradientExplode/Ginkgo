#ifndef _gkg_graphics_X11_base_LogScaleDivider_h_
#define _gkg_graphics_X11_base_LogScaleDivider_h_


#include <gkg-graphics-X11-base/ScaleDivider.h>


namespace gkg
{


template < class T >
class LogScaleDivider : public ScaleDivider< T >
{

  public:

    LogScaleDivider( const T& lowerBound,
                     const T& upperBound,
                     int32_t majorCount,
                     int32_t minorCount, 
                     const T& step = ( T )0,
                     bool ascend = true );
    virtual ~LogScaleDivider();

    static const T LogMin;
    static const T LogMax;

};


}


#endif
