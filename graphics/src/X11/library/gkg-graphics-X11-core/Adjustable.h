#ifndef _gkg_graphics_X11_core_Adjustable_h_
#define _gkg_graphics_X11_core_Adjustable_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Observable;
class Observer;
class AdjustableImplementation;


// Object to scroll and/or zoom
class Adjustable
{

  public:

    Adjustable();
    virtual ~Adjustable();

    virtual Observable* getObservable( DimensionName d ) const;

    virtual void addObserver( DimensionName d, Observer* observer );
    virtual void removeObserver( DimensionName d, Observer* observer );

    virtual FloatCoordinate getLower( DimensionName d ) const;
    virtual FloatCoordinate getUpper( DimensionName d ) const;
    virtual FloatCoordinate getLength( DimensionName d ) const;
    virtual FloatCoordinate getCurrentLower( DimensionName d ) const;
    virtual FloatCoordinate getCurrentUpper( DimensionName d ) const;
    virtual FloatCoordinate getCurrentLength( DimensionName d ) const;

    virtual void setSmallScroll( DimensionName d,
                                 FloatCoordinate floatCoordinate );
    virtual FloatCoordinate getSmallScroll( DimensionName d ) const;
    virtual void setLargeScroll( DimensionName d,
                                 FloatCoordinate floatCoordinate );
    virtual FloatCoordinate getLargeScroll( DimensionName d ) const;

    virtual void beginAdjustment( DimensionName d );
    virtual void commitAdjustment( DimensionName d );
    virtual void abortAdjustment( DimensionName d );

    virtual void scrollForward( DimensionName d );
    virtual void scrollBackward( DimensionName d );
    virtual void pageForward( DimensionName d );
    virtual void pageBackward( DimensionName d );

    virtual void scrollTo( DimensionName d, FloatCoordinate lower );
    virtual void scaleTo( DimensionName d, float fractionVisible );
    virtual void zoomTo( float magnification );

    virtual void getConstrain( DimensionName d,
                               FloatCoordinate& newLower ) const;
    virtual void notify( DimensionName d ) const;
    virtual void notifyAll() const;

    AdjustableImplementation* getImplementation() const;

  private:

    AdjustableImplementation* _adjustableImplementation;

};


}


#endif
