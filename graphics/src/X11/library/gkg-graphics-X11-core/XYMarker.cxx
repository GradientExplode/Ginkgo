#include <gkg-graphics-X11-core/XYMarker.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-processing-algobase/Math.h>


static const gkg::FloatCoordinate th = 1;
static const float e = 0.01;


//
// static functions
//

static void doDraw( gkg::Canvas* canvas,
                    const gkg::Color* color,
                    gkg::FloatCoordinate left,
                    gkg::FloatCoordinate bottom,
                    gkg::FloatCoordinate right,
                    gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate l = std::min( left, right );
  gkg::FloatCoordinate b = std::min( bottom, top );
  gkg::FloatCoordinate r = std::max( left, right );
  gkg::FloatCoordinate t = std::max( bottom, top );

  if ( ( ( r - l ) < 2 * th ) || ( ( t - b ) < 2 * th ) )
  {

    canvas->fillRectangle( l - th, b - th, r + th, t + th, color );

  }
  else
  {

    canvas->fillRectangle( l - th, b + th, l + th, t + th, color );
    canvas->fillRectangle( l + th, t + th, r + th, t - th, color );
    canvas->fillRectangle( r + th, t - th, r - th, b - th, color );
    canvas->fillRectangle( r - th, b - th, l - th, b + th, color );

  }

}


static void doDamage( gkg::Canvas* canvas,
                      gkg::FloatCoordinate left,
                      gkg::FloatCoordinate bottom,
                      gkg::FloatCoordinate right,
                      gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate l = std::min( left, right );
  gkg::FloatCoordinate b = std::min( bottom, top );
  gkg::FloatCoordinate r = std::max( left, right );
  gkg::FloatCoordinate t = std::max( bottom, top );

  if ( ( ( r - l ) < 2 * th ) || ( ( t - b ) < 2 * th ) )
  {

    canvas->setDamage( l - th, b - th, r + th, t + th );

  }
  else
  {

    canvas->setDamage( l - th, b + th, l + th, t + th );
    canvas->setDamage( l + th, t + th, r + th, t - th );
    canvas->setDamage( r + th, t - th, r - th, b - th );
    canvas->setDamage( r - th, b - th, l - th, b + th );

  }

}


//
// class XYMarker
//

gkg::XYMarker::XYMarker( gkg::Glyph* body,
                         const gkg::Color* overlay,
                         const gkg::Color* underlay )
              : gkg::MonoGlyph( body ),
                _canvas( 0 ),
                _overlay( overlay ),
                _underlay( underlay ),
                _marked( false )
{

  if ( _overlay )
  {

    _overlay->addReference();

  }
  if ( _underlay )
  {

    _underlay->addReference();

  }

}


gkg::XYMarker::~XYMarker()
{

  if ( _overlay )
  {

    _overlay->removeReference();

  }
  if ( _underlay )
  {

    _underlay->removeReference();

  }

}


void gkg::XYMarker::mark( gkg::FloatCoordinate left,
                          gkg::FloatCoordinate bottom,
                          gkg::FloatCoordinate right,
                          gkg::FloatCoordinate top )
{

  if ( _canvas )
  {

    if ( !_marked )
    {

      doDamage( _canvas, left, bottom, right, top );

    }
    else if ( ( left > _right ) || ( right < _left ) ||
              ( top < _bottom ) || ( bottom > _top ) )
    {

      doDamage( _canvas, left, bottom, right, top );
      doDamage( _canvas, _left, _bottom, _right, _top );

    }
    else
    {

      if ( !gkg::equal( _left, left, e ) )
      {

        doDamage( _canvas, left, _bottom, _left, _top);

      }
      if ( !gkg::equal( _right, right, e ) )
      {

        doDamage( _canvas, right, _bottom, _right, _top );

      }
      if ( !gkg::equal( _bottom, bottom, e ) )
      {

        doDamage( _canvas, _left, bottom, _right, _bottom );

      }
      if ( !gkg::equal( _top, top, e ) )
      {

        doDamage( _canvas, _left, top, _right, _top );

      }
      if ( !gkg::equal( _top, top, e ) &&
           !gkg::equal( _left, left, e ) )
      {

        doDamage( _canvas, left, top, _left, _top );

      }
      if ( !gkg::equal( _top, top, e ) &&
           !gkg::equal( _right, right, e ) )
      {

        doDamage( _canvas, right, top, _right, _top );

      }
      if ( !gkg::equal( _bottom, bottom, e ) &&
           !gkg::equal( _left, left, e ) )
      {

        doDamage( _canvas, left, bottom, _left, _bottom );

      }
      if ( !gkg::equal( _bottom, bottom, e ) &&
           !gkg::equal( _right, right, e ) )
      {

        doDamage( _canvas, right, bottom, _right, _bottom );

      }

    }

  }
  _left = left;
  _bottom = bottom;
  _right = right;
  _top = top;
  _marked = true;

}


void gkg::XYMarker::unmark()
{

  if ( _marked )
  {

    if ( _canvas )
    {

      doDamage( _canvas, _left, _bottom, _right, _top );

    }
    _marked = false;

  }

}


void gkg::XYMarker::allocate( gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );
  geometryExtension.mergeXY( canvas,
                             geometryAllocation.getLeft() - th,
                             geometryAllocation.getBottom() - th,
                             geometryAllocation.getRight() + th,
                             geometryAllocation.getTop() + th );
  _canvas = canvas;

}


void 
gkg::XYMarker::draw( gkg::Canvas* canvas,
                     const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _marked && _underlay )
  {

    doDraw( canvas, _underlay, _left, _bottom, _right, _top );

  }
  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );
  if ( _marked && _overlay )
  {

    doDraw( canvas, _overlay, _left, _bottom, _right, _top );

  }

}


void gkg::XYMarker::undraw()
{

  this->gkg::MonoGlyph::undraw();
  _canvas = 0;

}
