#ifndef _gkg_graphics_X11_base_LinearWindowingTransform_h_
#define _gkg_graphics_X11_base_LinearWindowingTransform_h_


#include <gkg-graphics-X11-base/WindowingTransform.h>


namespace gkg
{


template < class T >
class LinearWindowingTransform : public WindowingTransform< T >
{

  public:

    LinearWindowingTransform( const T& fromCoordinate1 = 0.0f,
                              const T& fromCoordinate2 = 1.0f,
                              const T& toCoordinate1 = 0.0f,
                              const T& toCoordinate2 = 1.0f );
    virtual ~LinearWindowingTransform();

  protected:


    virtual T applyFunction( const T& fromCoordinate ) const;
    virtual T applyInverseFunction( const T& fromCoordinate ) const;

};


}


#endif
