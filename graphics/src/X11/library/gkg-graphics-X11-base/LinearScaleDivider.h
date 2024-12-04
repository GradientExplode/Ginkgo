#ifndef _gkg_graphics_X11_base_LinearScaleDivider_h_
#define _gkg_graphics_X11_base_LinearScaleDivider_h_


#include <gkg-graphics-X11-base/ScaleDivider.h>


namespace gkg
{


template < class T >
class LinearScaleDivider : public ScaleDivider< T >
{

  public:

    LinearScaleDivider( const T& lowerBound,
                        const T& upperBound,
                        int32_t majorCount,
                        int32_t minorCount, 
                        const T& step = 0.0f,
                        bool ascend = true );
    virtual ~LinearScaleDivider();

};


}


#endif
