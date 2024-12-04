#ifndef _gkg_graphics_X11_core_AdjustableImplementation_h_
#define _gkg_graphics_X11_core_AdjustableImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Observable;


class AdjustableImplementation
{

  public:

    struct AdjustableInfo 
    {

      Observable* observable;
      FloatCoordinate small;
      FloatCoordinate large;

    };

    AdjustableImplementation();
    virtual ~AdjustableImplementation();

    virtual Observable* getObservable( DimensionName d ) const;

    virtual void setSmallScroll( DimensionName d,
                                 FloatCoordinate floatCoordinate );
    virtual FloatCoordinate getSmallScroll( DimensionName d ) const;
    virtual void setLargeScroll( DimensionName d,
                                 FloatCoordinate floatCoordinate );
    virtual FloatCoordinate getLargeScroll( DimensionName d ) const;

  private:

    AdjustableInfo _info[ Dimension2d ];

};


}


#endif
