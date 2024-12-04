#include <gkg-graphics-X11-core/Grid.h>
#include <gkg-graphics-X11-base/ScaleDivider.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-base/WindowingTransform.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Canvas.h>


gkg::Grid::Grid(
          gkg::ScaleDivider< gkg::FloatCoordinate >* scaleDividerX,
          gkg::ScaleDivider< gkg::FloatCoordinate >* scaleDividerY,
          gkg::WindowingTransform< gkg::FloatCoordinate >* windowingTransformX,
          gkg::WindowingTransform< gkg::FloatCoordinate >* windowingTransformY )
          : _scaleDividerX( scaleDividerX ),
            _scaleDividerY( scaleDividerY ),
            _majorColor( new gkg::Color( 1, 0, 0 ) ),
            _minorColor( new gkg::Color( 0.8, 0.2, 0 ) ),
            _majorBrush( new gkg::Brush( 1.0f ) ),
            _minorBrush( new gkg::Brush( 0xf0f0f0f0, 1.0f ) ),
            _xEnabled( true ),
            _yEnabled( true ),
            _minorXEnabled( true ),
            _minorYEnabled( true )
{

  if ( _scaleDividerX )
  {

    _scaleDividerX->addReference();

  }
  if ( _scaleDividerY )
  {

    _scaleDividerY->addReference();

  }
  if ( _majorColor )
  {

    _majorColor->addReference();

  }
  if ( _minorColor )
  {

    _minorColor->addReference();

  }
  if ( _majorBrush )
  {

    _majorBrush->addReference();

  }
  if ( _minorBrush )
  {

    _minorBrush->addReference();

  }

  _windowingTransforms[ gkg::DimensionX ] = windowingTransformX;
  _windowingTransforms[ gkg::DimensionY ] = windowingTransformY;

  if ( _windowingTransforms[ gkg::DimensionX ] )
  {

    _windowingTransforms[ gkg::DimensionX ]->addReference();

  }
  if ( _windowingTransforms[ gkg::DimensionY ] )
  {

    _windowingTransforms[ gkg::DimensionY ]->addReference();

  }

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  setBody( layoutFactory.createVGlue() );


}


gkg::Grid::~Grid()
{

  if ( _scaleDividerX )
  {

    _scaleDividerX->removeReference();

  }
  if ( _scaleDividerY )
  {

    _scaleDividerY->removeReference();

  }
  if ( _majorColor )
  {

    _majorColor->removeReference();

  }
  if ( _minorColor )
  {

    _minorColor->removeReference();

  }
  if ( _majorBrush )
  {

    _majorBrush->removeReference();

  }
  if ( _minorBrush )
  {

    _minorBrush->removeReference();

  }
  if ( _windowingTransforms[ gkg::DimensionX ] )
  {

    _windowingTransforms[ gkg::DimensionX ]->removeReference();

  }
  if ( _windowingTransforms[ gkg::DimensionY ] )
  {

    _windowingTransforms[ gkg::DimensionY ]->removeReference();

  }

}


void gkg::Grid::setScaleDividerX(
                      gkg::ScaleDivider< gkg::FloatCoordinate >* scaleDividerX )
{

  if ( scaleDividerX )
  {

    if ( _scaleDividerX )
    {

      _scaleDividerX->removeReference();

    }
    _scaleDividerX = scaleDividerX;
    if ( scaleDividerX )
    {

      scaleDividerX->addReference();

    }

  }

}


gkg::ScaleDivider< gkg::FloatCoordinate >* gkg::Grid::getScaleDividerX() const
{

  return _scaleDividerX;

}


void gkg::Grid::setScaleDividerY(
                      gkg::ScaleDivider< gkg::FloatCoordinate >* scaleDividerY )
{

  if ( scaleDividerY )
  {

    if ( _scaleDividerY )
    {

      _scaleDividerY->removeReference();

    }
    _scaleDividerY = scaleDividerY;
    if ( scaleDividerY )
    {

      scaleDividerY->addReference();

    }

  }

}


gkg::ScaleDivider< gkg::FloatCoordinate >* gkg::Grid::getScaleDividerY() const
{

  return _scaleDividerY;

}


void gkg::Grid::setMajorColor( gkg::Color* color )
{

  if ( color )
  {

    if ( _majorColor )
    {

      _majorColor->removeReference();

    }
    _majorColor = color;
    if ( color )
    {

      color->addReference();

    }

  }

}


gkg::Color* gkg::Grid::getMajorColor() const
{

  return _majorColor;

}


void gkg::Grid::setMinorColor( gkg::Color* color )
{

  if ( color )
  {

    if ( _minorColor )
    {

      _minorColor->removeReference();

    }
    _minorColor = color;
    if ( color )
    {

      color->addReference();

    }

  }

}


gkg::Color* gkg::Grid::getMinorColor() const
{

  return _minorColor;

}


void gkg::Grid::setMajorBrush( gkg::Brush* brush )
{

  if ( brush )
  {

    if ( _majorBrush )
    {

      _majorBrush->removeReference();

    }
    _majorBrush = brush;
    if ( brush )
    {

      brush->addReference();

    }

  }

}


gkg::Brush* gkg::Grid::getMajorBrush() const
{

  return _majorBrush;

}


void gkg::Grid::setMinorBrush( gkg::Brush* brush )
{

  if ( brush )
  {

    if ( _minorBrush )
    {

      _minorBrush->removeReference();

    }
    _minorBrush = brush;
    if ( brush )
    {

      brush->addReference();

    }

  }

}


gkg::Brush* gkg::Grid::getMinorBrush() const
{

  return _minorBrush;

}


void gkg::Grid::enableX( bool value )
{

  _xEnabled = value;

}


bool gkg::Grid::isXEnabled() const
{

  return _xEnabled;

}


void gkg::Grid::enableY( bool value )
{

  _yEnabled = value;

}


bool gkg::Grid::isYEnabled() const
{

  return _yEnabled;

}


void gkg::Grid::enableMinorX( bool value )
{

  _minorXEnabled = value;

}


bool gkg::Grid::isMinorXEnabled() const
{

  return _minorXEnabled;

}


void gkg::Grid::enableMinorY( bool value )
{

  _minorYEnabled = value;

}


bool gkg::Grid::isMinorYEnabled() const
{

  return _minorYEnabled;

}


void gkg::Grid::setWindowingTransform(
           gkg::DimensionType d,
           gkg::WindowingTransform< gkg::FloatCoordinate >* windowingTransform )
{

  if ( windowingTransform )
  {

    if ( _windowingTransforms[ d ] )
    {

      _windowingTransforms[ d ]->removeReference();

    }
    _windowingTransforms[ d ] = windowingTransform;
    if ( windowingTransform )
    {

      windowingTransform->addReference();

    }

  }

}


gkg::WindowingTransform< gkg::FloatCoordinate >* 
gkg::Grid::getWindowingTransform( gkg::DimensionType d ) const
{

  return _windowingTransforms[ d ];

}


void gkg::Grid::draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate x1, y1, x2, y2, x, y;

  x1 = _windowingTransforms[ gkg::DimensionX ]->getToCoordinate1() +
       geometryAllocation.getLeft();
  x2 = _windowingTransforms[ gkg::DimensionX ]->getToCoordinate2() +
       geometryAllocation.getLeft();
  y1 = _windowingTransforms[ gkg::DimensionY ]->getToCoordinate1() +
       geometryAllocation.getBottom();
  y2 = _windowingTransforms[ gkg::DimensionY ]->getToCoordinate2() +
       geometryAllocation.getBottom();

  //
  // draw minor gridlines
  //

  // draw x minor lines

  if ( _xEnabled && _minorXEnabled )
  {

    int32_t i;
    for ( i = 0; i < _scaleDividerX->getMinorCount(); i++ )
    {

      x = _windowingTransforms[ gkg::DimensionX ]->getDirect( 
                                           _scaleDividerX->getMinorMark( i ) ) +
          geometryAllocation.getLeft();
      if ( ( x >= x1 ) && ( x <= x2 ) )
      {

        canvas->line( x, y1, x, y2, _minorColor, _minorBrush );

      }

    }

  }

  // draw y minor lines

  if ( _yEnabled && _minorYEnabled )
  {

    int32_t i;
    for ( i = 0; i < _scaleDividerY->getMinorCount(); i++ )
    {

      y = _windowingTransforms[ gkg::DimensionY ]->getDirect( 
                                          _scaleDividerY->getMinorMark( i ) ) +
          geometryAllocation.getBottom();
      if ( ( y >= y1 ) && ( y <= y2 ) )
      {

        canvas->line( x1, y, x2, y, _minorColor, _minorBrush );

      }

    }

  }

  //
  // draw major gridlines
  //

  // draw x major lines

  if ( _xEnabled )
  {

    int32_t i;
    for ( i = 0; i < _scaleDividerX->getMajorCount(); i++ )
    {

      x = _windowingTransforms[ gkg::DimensionX ]->getDirect( 
                                           _scaleDividerX->getMajorMark( i ) ) +
          geometryAllocation.getLeft();
      if ( ( x >= x1 ) && ( x <= x2 ) )
      {

        canvas->line( x, y1, x, y2, _majorColor, _majorBrush );

      }

    }

  }

  // draw y major lines

  if ( _yEnabled )
  {

    int32_t i;
    for ( i = 0; i < _scaleDividerY->getMajorCount(); i++ )
    {

      y = _windowingTransforms[ gkg::DimensionY ]->getDirect( 
                                           _scaleDividerY->getMajorMark( i ) ) +
          geometryAllocation.getBottom();
      if ( ( y >= y1 ) && ( y <= y2 ) )
      {

        canvas->line( x1, y, x2, y, _majorColor, _majorBrush );

      }

    }

  }

}
