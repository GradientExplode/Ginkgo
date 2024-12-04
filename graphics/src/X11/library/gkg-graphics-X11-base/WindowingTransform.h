#ifndef _gkg_graphics_X11_base_WindowingTransform_h_
#define _gkg_graphics_X11_base_WindowingTransform_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


template < class T >
class WindowingTransform : public RCGUIObject
{

  public:

    virtual ~WindowingTransform();

    virtual void setFromRange( const T& fromCoordinate1,
                               const T& fromCoordinate2 );
    virtual void setToRange( const T& toCoordinate1,
                             const T& toCoordinate2 );

    virtual T getFromCoordinate1() const;
    virtual T getFromCoordinate2() const;
    virtual T getToCoordinate1() const;
    virtual T getToCoordinate2() const;

    virtual bool containsFromCoordinate( const T& fromCoordinate ) const;
    virtual bool containsToCoordinate( const T& toCoordinate ) const;

    virtual T getDirect( const T& fromCoordinate ) const;
    virtual T getInverse( const T& toCoordinate ) const;

    virtual T getLimitTransform( const T& fromCoordinate ) const;

  protected:

    WindowingTransform( const T& fromCoordinate1, const T& fromCoordinate2,
                        const T& toCoordinate1, const T& toCoordinate2 );

    virtual T applyFunction( const T& value ) const = 0;
    virtual T applyInverseFunction( const T& value ) const = 0;
    virtual void processFactor();

    T _fromCoordinate1;
    T _fromCoordinate2;
    T _toCoordinate1;
    T _toCoordinate2;
    T _factor;

};


}


#endif
