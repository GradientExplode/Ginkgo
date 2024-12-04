#include <gkg-graphics-X11-core/LRMarker.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <algorithm>


//
// static functions
//

static void doDraw( gkg::Canvas* canvas,
                    const gkg::Color* color,
                    gkg::FloatCoordinate left,
                    gkg::FloatCoordinate right,
                    gkg::FloatCoordinate x1,
                    gkg::FloatCoordinate b1,
                    gkg::FloatCoordinate t1,
                    gkg::FloatCoordinate x2,
                    gkg::FloatCoordinate b2,
                    gkg::FloatCoordinate t2 )
{

  if ( x1 < x2 )
  {

    canvas->fillRectangle( left, b1, x1, b2, color );
    canvas->fillRectangle( x2, t1, right, t2, color );
    canvas->fillRectangle( x1, b2, x2, t1, color );

  }
  else if ( x1 > x2 )
  {

    canvas->fillRectangle( left, b1, x2, b2, color );
    canvas->fillRectangle( x1, t1, right, t2, color );
    canvas->fillRectangle( x1, b1, x2, t2, color );

  }
  else
  {

    canvas->fillRectangle( left, b1, x1, b2, color );
    canvas->fillRectangle( x2, t1, right, t2, color );

  }

}


static void doDamage( gkg::Canvas* canvas,
                      gkg::FloatCoordinate left,
                      gkg::FloatCoordinate right,
                      gkg::FloatCoordinate x1,
                      gkg::FloatCoordinate b1,
                      gkg::FloatCoordinate t1,
                      gkg::FloatCoordinate x2,
                      gkg::FloatCoordinate b2,
                      gkg::FloatCoordinate t2 )
{

  if ( x1 < x2 )
  {

    if ( b1 != b2 )
    {

      canvas->setDamage( left, std::min( b1, b2 ), x1, std::max( b1, b2 ) );

    }
    if ( t1 != t2 )
    {

      canvas->setDamage( x2, std::min( t1, t2 ), right, std::max( t1, t2 ) );

    }
    if ( b2 != t1 )
    {

      canvas->setDamage( x1, std::min( b2, t1 ), x2, std::max( b2, t1 ) );

    }

  }
  else if ( x1 > x2 )
  {

    if ( b1 != b2 )
    {

      canvas->setDamage( left, std::min( b1, b2 ), x2, std::max( b1, b2 ) );

    }
    if ( t1 != t2 )
    {

      canvas->setDamage( x1, std::min( t1, t2 ), right, std::max( t1, t2 ) );

    }
    if ( b1 != t2 )
    {

      canvas->setDamage( x2, std::min( t2, b1 ), x1, std::max( t2, b1 ) );

    }

  }
  else
  {

    if ( b1 != b2 )
    {

      canvas->setDamage( left, std::min( b1, b2 ), x2, std::max( b1, b2 ) );

    }
    if ( t1 != t2 )
    {

      canvas->setDamage( x2, std::min( t1, t2 ), right, std::max( t1, t2 ) );

    }

  }

}


//
// class LRMarker
//

gkg::LRMarker::LRMarker( gkg::Glyph* body,
                         const gkg::Color* overlay,
                         const gkg::Color* underlay )
              : gkg::MonoGlyph( body ),
                _canvas( 0 ),
                _overlay( overlay ),
                _underlay( underlay ),
                _marked( false ),
                _bounded( false )
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


gkg::LRMarker::~LRMarker()
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


void gkg::LRMarker::setBound( gkg::FloatCoordinate left,
                              gkg::FloatCoordinate bottom,
                              gkg::FloatCoordinate right,
                              gkg::FloatCoordinate top )
{

  if ( _canvas )
  {

    if ( !_bounded )
    {

      _canvas->setDamage( left, bottom, right, top );

    }
    else
    {

      gkg::FloatCoordinate lx = std::max( _left, left );
      gkg::FloatCoordinate ln = std::min( _left, left );
      gkg::FloatCoordinate bx = std::max( _bottom, bottom );
      gkg::FloatCoordinate bn = std::min( _bottom, bottom );
      gkg::FloatCoordinate rx = std::max( _right, right );
      gkg::FloatCoordinate rn = std::min( _right, right );
      gkg::FloatCoordinate tx = std::max( _top, top );
      gkg::FloatCoordinate tn = std::min( _top, top );
      if ( ln != lx )
      {

        _canvas->setDamage( ln, bn, lx, tn );

      }
      if ( tn != tx )
      {

        _canvas->setDamage( ln, tn, rn, tx );

      }
      if ( rn != rx )
      {

        _canvas->setDamage( rn, bx, rx, tx );

      }
      if ( bn != bx )
      {

        _canvas->setDamage( lx, bn, rx, bx );

      }

    }

  }
  _left = left;
  _bottom = bottom;
  _right = right;
  _top = top;
  _bounded = true;

}


void gkg::LRMarker::mark( gkg::FloatCoordinate x1,
                          gkg::FloatCoordinate y1,
                          gkg::FloatCoordinate h1,
                          gkg::FloatCoordinate x2,
                          gkg::FloatCoordinate y2,
                          gkg::FloatCoordinate h2 )
{

  if ( _canvas )
  {

    if ( !_marked )
    {

      doDamage( _canvas, _left, _right, x1, y1, y1 + h1, x2, y2, y2 + h2 );

    }
    else if ( ( ( y2 > _y1 ) || ( ( y2 == _y1 ) && ( x2 < _x1 ) ) ) ||
              ( ( y1 < _y2 ) || ( ( y1 == _y2 ) && ( x1 > _x2 ) ) ) )
    {

      doDamage( _canvas, _left, _right,
                _x1, _y1, _y1 + _h1, _x2, _y2, _y2 + _h2 );
      doDamage( _canvas, _left, _right,
                x1, y1, y1 + h1, x2, y2, y2 + h2 );

    }
    else
    {

      doDamage( _canvas, _left, _right,
                _x1, _y1, _y1 + _h1, x1, y1, y1 + h1 );
      doDamage( _canvas, _left, _right,
                _x2, _y2, _y2 + _h2, x2, y2, y2 + h2 );

    }

  }
  _x1 = x1;
  _y1 = y1;
  _h1 = h1;
  _x2 = x2;
  _y2 = y2;
  _h2 = h2;
  _marked = true;

}


void gkg::LRMarker::unmark()
{

  if ( _marked )
  {

    mark( _x1, _y1, _h1, _x1, _y1, _h1 );
    _marked = false;

  }

}


void gkg::LRMarker::allocate( gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );
  geometryExtension.merge( canvas, geometryAllocation );
  _canvas = canvas;

}


void gkg::LRMarker::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _canvas )
  {

    if ( _marked && _underlay )
    {

      doDraw( canvas, _underlay,
              _left, _right, _x1, _y1, _y1 + _h1, _x2, _y2, _y2 + _h2 );

    }

  }
  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );
  if ( _canvas )
  {

    if ( _marked && _overlay )
    {

      doDraw( canvas, _overlay,
              _left, _right, _x1, _y1, _y1 + _h1, _x2, _y2, _y2 + _h2 );

    }

  }

}


void gkg::LRMarker::undraw()
{

  this->gkg::MonoGlyph::undraw();
  _canvas = 0;

}
