#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/AdjustableImplementation.h>
#include <gkg-core-pattern/Observable.h>
#include <gkg-core-pattern/Observer.h>
#include <gkg-processing-algobase/Math.h>


gkg::Adjustable::Adjustable()
{

  _adjustableImplementation = new gkg::AdjustableImplementation;

}


gkg::Adjustable::~Adjustable()
{

  delete _adjustableImplementation;

}


gkg::Observable* gkg::Adjustable::getObservable( gkg::DimensionName d ) const
{

  return _adjustableImplementation->getObservable( d );

}


void gkg::Adjustable::addObserver( gkg::DimensionName d,
                                   gkg::Observer* observer )
{

  getObservable( d )->addObserver( observer );

}


void gkg::Adjustable::removeObserver( gkg::DimensionName d,
                                      gkg::Observer* observer )
{

  getObservable( d )->removeObserver( observer );

}


gkg::FloatCoordinate gkg::Adjustable::getLower( gkg::DimensionName ) const
{

  return 0;

}


gkg::FloatCoordinate gkg::Adjustable::getUpper( gkg::DimensionName d ) const
{

  return getLower( d ) + getLength( d );

}


gkg::FloatCoordinate gkg::Adjustable::getLength( gkg::DimensionName d ) const
{

  return getUpper( d ) - getLower( d );

}


gkg::FloatCoordinate 
gkg::Adjustable::getCurrentLower( gkg::DimensionName ) const
{

  return 0;

}


gkg::FloatCoordinate 
gkg::Adjustable::getCurrentUpper( gkg::DimensionName d ) const
{

  return getCurrentLower( d ) + getCurrentLength( d );

}


gkg::FloatCoordinate
gkg::Adjustable::getCurrentLength( gkg::DimensionName d ) const
{

  return getCurrentUpper( d ) - getCurrentLower( d );

}


void gkg::Adjustable::setSmallScroll( gkg::DimensionName d,
                                      gkg::FloatCoordinate floatCoordinate )
{

  _adjustableImplementation->setSmallScroll( d, floatCoordinate );

}


gkg::FloatCoordinate 
gkg::Adjustable::getSmallScroll( gkg::DimensionName d ) const
{

  return _adjustableImplementation->getSmallScroll( d );

}


void gkg::Adjustable::setLargeScroll( gkg::DimensionName d,
                                      gkg::FloatCoordinate floatCoordinate )
{

  _adjustableImplementation->setLargeScroll( d, floatCoordinate );

}


gkg::FloatCoordinate 
gkg::Adjustable::getLargeScroll( gkg::DimensionName d ) const
{

  gkg::FloatCoordinate s = _adjustableImplementation->getLargeScroll( d );
  if ( gkg::equal( s, 0.0f, 1e-4f ) )
  {

    s = getCurrentLength( d ) - 1;

  }
  return s;

}


void gkg::Adjustable::beginAdjustment( gkg::DimensionName )
{
}


void gkg::Adjustable::commitAdjustment( gkg::DimensionName )
{
}


void gkg::Adjustable::abortAdjustment( gkg::DimensionName )
{
}


void gkg::Adjustable::scrollForward( gkg::DimensionName d )
{

  scrollTo( d, getCurrentLower( d ) + getSmallScroll( d ) );

}


void gkg::Adjustable::scrollBackward( gkg::DimensionName d )
{

  scrollTo( d, getCurrentLower( d ) - getSmallScroll( d ) );

}


void gkg::Adjustable::pageForward( gkg::DimensionName d )
{

  scrollTo( d, getCurrentLower( d ) + getLargeScroll( d ) );

}


void gkg::Adjustable::pageBackward( gkg::DimensionName d )
{

  scrollTo( d, getCurrentLower( d ) - getLargeScroll( d ) );

}


void gkg::Adjustable::scrollTo( gkg::DimensionName, gkg::FloatCoordinate )
{
}


void gkg::Adjustable::scaleTo( gkg::DimensionName, float )
{
}


void gkg::Adjustable::zoomTo( float )
{
}


void gkg::Adjustable::getConstrain( gkg::DimensionName d,
                                    gkg::FloatCoordinate& newLower ) const
{

  gkg::FloatCoordinate a = getLower( d );
  gkg::FloatCoordinate b = getUpper( d ) - getCurrentLength( d );
  if ( newLower < a )
  {

    newLower = a;

  }
  else if ( newLower > b )
  {

    newLower = b;

  }

}


void gkg::Adjustable::notify( gkg::DimensionName d ) const
{

  getObservable( d )->setChanged();
  getObservable( d )->notifyObservers();

}


void gkg::Adjustable::notifyAll() const
{

  for ( gkg::DimensionName d = 0; d < gkg::Dimension2d; d++ )
  {

    getObservable( d )->setChanged();
    getObservable( d )->notifyObservers();

  }

}


gkg::AdjustableImplementation* gkg::Adjustable::getImplementation() const
{

  return _adjustableImplementation;

}

