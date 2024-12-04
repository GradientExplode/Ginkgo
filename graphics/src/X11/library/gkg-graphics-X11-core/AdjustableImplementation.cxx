#include <gkg-graphics-X11-core/AdjustableImplementation.h>
#include <gkg-core-pattern/Observable.h>


gkg::AdjustableImplementation::AdjustableImplementation()
{

  for ( gkg::DimensionName d = 0; d < gkg::Dimension2d; d++ )
  {

    _info[ d ].observable = new gkg::Observable;
    _info[ d ].small = 1;
    _info[ d ].large = 0;

  }

}


gkg::AdjustableImplementation::~AdjustableImplementation()
{

  for ( gkg::DimensionName d = 0; d < gkg::Dimension2d; d++ )
  {

    delete _info[ d ].observable;

  }

}


gkg::Observable* 
gkg::AdjustableImplementation::getObservable( gkg::DimensionName d ) const
{

  return _info[ d ].observable;

}


void gkg::AdjustableImplementation::setSmallScroll(
                                          gkg::DimensionName d,
                                          gkg::FloatCoordinate floatCoordinate )
{

  _info[ d ].small = floatCoordinate;

}


gkg::FloatCoordinate 
gkg::AdjustableImplementation::getSmallScroll( gkg::DimensionName d ) const
{

  return _info[ d ].small;

}


void gkg::AdjustableImplementation::setLargeScroll(
                                          gkg::DimensionName d,
                                          gkg::FloatCoordinate floatCoordinate )
{

  _info[ d ].large = floatCoordinate;

}


gkg::FloatCoordinate 
gkg::AdjustableImplementation::getLargeScroll( gkg::DimensionName d ) const
{

  return _info[ d ].large;

}

