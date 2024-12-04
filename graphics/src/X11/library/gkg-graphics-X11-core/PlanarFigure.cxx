#include <gkg-graphics-X11-core/PlanarFigure.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <algorithm>


//
// class PlanarFigure
//

gkg::PlanarFigure::PlanarFigure( const gkg::Brush* brush,
                                 const gkg::Color* stroke,
                                 const gkg::Color* fill,
                                 bool closed,
                                 bool curved,
                                 int32_t coords )
                  : _brush( brush ),
                    _stroke( stroke ),
                    _fill( fill ),
                    _closed( closed ),
                    _curved( curved ),
                    _count( 0 ),
                    _x( new gkg::FloatCoordinate[ coords ] ),
                    _y( new gkg::FloatCoordinate[ coords ] )
{

  if ( _brush )
  {

    _brush->addReference();

  }
  if ( _stroke )
  {

    _stroke->addReference();

  }
  if ( _fill )
  {

    _fill->addReference();

  }

}


gkg::PlanarFigure::~PlanarFigure()
{

  if ( _brush )
  {

    _brush->removeReference();

  }
  if ( _stroke )
  {

    _stroke->removeReference();

  }
  if ( _fill )
  {

    _fill->removeReference();

  }
  delete [] _x;
  delete [] _y;

}


void gkg::PlanarFigure::request(
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _count > 0 )
  {

    gkg::GeometryRequirement rx( -_xMin, -_xMin,
                                 -_xMin, _xMax,
                                 _xMax, _xMax );
    gkg::GeometryRequirement ry( -_yMin, -_yMin,
                                 -_yMin, _yMax,
                                 _yMax, _yMax );
    geometryRequisition.setRequirement( gkg::DimensionX, rx );
    geometryRequisition.setRequirement( gkg::DimensionY, ry );

  }

}


void gkg::PlanarFigure::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  if ( _count > 0 )
  {

    gkg::FloatCoordinate w = _brush  ? _brush->getWidth() : 0;
    gkg::FloatCoordinate x = geometryAllocation.getX();
    gkg::FloatCoordinate y = geometryAllocation.getY();
    geometryExtension.mergeXY( canvas,
                               x + _xMin - w,
                               y + _yMin - w,
                               x + _xMax + w,
                               y + _yMax + w );

  }

}


void gkg::PlanarFigure::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( canvas && ( _count > 0 ) )
  {

    gkg::FloatCoordinate x = geometryAllocation.getX();
    gkg::FloatCoordinate y = geometryAllocation.getY();
    canvas->newPath();
    canvas->moveTo( x + _x[ 0 ], y + _y[ 0 ] );
    if ( _curved )
    {

      for ( int32_t i = 1; i < _count; i += 3 )
      {

        canvas->curveTo( x + _x[ i + 2 ], y + _y[ i + 2 ],
                         x + _x[ i ], y + _y[ i ],
                         x + _x[ i + 1 ], y + _y[ i + 1 ] );

      }

    }
    else
    {

      for ( int32_t i = 1; i < _count; i++ )
      {

        canvas->lineTo( x + _x[ i ], y + _y[ i ] );

      }

    }
    if ( _closed )
    {

      canvas->closePath();

    }

  }
  if ( _fill )
  {

    canvas->fill( _fill );

  }
  if ( _brush && _stroke )
  {

    canvas->stroke( _stroke, _brush );

  }

}


void gkg::PlanarFigure::addPoint( gkg::FloatCoordinate x,
                                  gkg::FloatCoordinate y )
{

  if ( _count == 0 )
  {

    _xMin = x - 1;
    _xMax = x + 1;
    _yMin = y - 1;
    _yMax = y + 1;

  }
  else
  {

    _xMin = std::min( _xMin, x );
    _xMax = std::max( _xMax, x );
    _yMin = std::min( _yMin, y );
    _yMax = std::max( _yMax, y );

  }
  _x[ _count ] = x;
  _y[ _count ] = y;
  _count += 1;

}


void gkg::PlanarFigure::addCurve( gkg::FloatCoordinate x,
                                  gkg::FloatCoordinate y,
                                  gkg::FloatCoordinate x1,
                                  gkg::FloatCoordinate y1,
                                  gkg::FloatCoordinate x2,
                                  gkg::FloatCoordinate y2 )
{

  addPoint( x1, y1 );
  addPoint( x2, y2 );
  addPoint( x, y );

}


void gkg::PlanarFigure::bSplineMoveTo( gkg::FloatCoordinate x,
                                       gkg::FloatCoordinate y,
                                       gkg::FloatCoordinate x1,
                                       gkg::FloatCoordinate y1,
                                       gkg::FloatCoordinate x2,
                                       gkg::FloatCoordinate y2 )
{

  gkg::FloatCoordinate p1x = ( x + x + x1 ) / 3;
  gkg::FloatCoordinate p1y = ( y + y + y1 ) / 3;
  gkg::FloatCoordinate p2x = ( x + x + x2 ) / 3;
  gkg::FloatCoordinate p2y = ( y + y + y2 ) / 3;
  addPoint( ( p1x + p2x ) / 2, ( p1y + p2y ) / 2 );
}


void gkg::PlanarFigure::bSplineCurveTo( gkg::FloatCoordinate x,
                                        gkg::FloatCoordinate y,
                                        gkg::FloatCoordinate x1,
                                        gkg::FloatCoordinate y1,
                                        gkg::FloatCoordinate x2,
                                        gkg::FloatCoordinate y2 )
{

  gkg::FloatCoordinate p1x = ( x + x + x1 ) / 3;
  gkg::FloatCoordinate p1y = ( y + y + y1 ) / 3;
  gkg::FloatCoordinate p2x = ( x + x + x2 ) / 3;
  gkg::FloatCoordinate p2y = ( y + y + y2 ) / 3;
  gkg::FloatCoordinate p3x = ( x1 + x1 + x ) / 3;
  gkg::FloatCoordinate p3y = ( y1 + y1 + y ) / 3;
  addCurve( ( p1x + p2x ) / 2, ( p1y + p2y ) / 2, p3x, p3y, p1x, p1y );

}


//
// class PlanarFigureLine
//

gkg::PlanarFigureLine::PlanarFigureLine( const gkg::Brush* brush,
                                         const gkg::Color* stroke,
                                         const gkg::Color* fill,
                                         gkg::FloatCoordinate x1,
                                         gkg::FloatCoordinate y1,
                                         gkg::FloatCoordinate x2,
                                         gkg::FloatCoordinate y2 )
                      : gkg::PlanarFigure( brush,
                                           stroke,
                                           fill,
                                           false,
                                           false,
                                           2 )
{

  addPoint( x1, y1 );
  addPoint( x2, y2 );

}


gkg::PlanarFigureLine::~PlanarFigureLine() 
{
}


//
// class PlanarFigureLine
//

gkg::PlanarFigureRectangle::PlanarFigureRectangle (
                                                    const gkg::Brush* brush,
                                                    const gkg::Color* stroke,
                                                    const gkg::Color* fill,
                                                    gkg::FloatCoordinate left,
                                                    gkg::FloatCoordinate bottom,
                                                    gkg::FloatCoordinate right,
                                                    gkg::FloatCoordinate top )
                            : gkg::PlanarFigure( brush,
                                                    stroke,
                                                    fill,
                                                    true,
                                                    false,
                                                    4 )
{

  addPoint( left, bottom );
  addPoint( left, top );
  addPoint( right, top );
  addPoint( right, bottom );

}


gkg::PlanarFigureRectangle::~PlanarFigureRectangle()
{
}


//
// class PlanarFigureCircle
//

static float p0 = 1.00000000;
static float p1 = 0.89657547;   // cos 30 * sqrt(1 + tan 15 * tan 15)
static float p2 = 0.70710678;   // cos 45 
static float p3 = 0.51763809;   // cos 60 * sqrt(1 + tan 15 * tan 15)
static float p4 = 0.26794919;   // tan 15

gkg::PlanarFigureCircle::PlanarFigureCircle( const gkg::Brush* brush,
                                             const gkg::Color* stroke,
                                             const gkg::Color* fill,
                                             gkg::FloatCoordinate x,
                                             gkg::FloatCoordinate y,
                                             gkg::FloatCoordinate radius )
                        : gkg::PlanarFigure( brush,
                                             stroke,
                                             fill,
                                             true,
                                             true,
                                             25 )
{

  float px0 = p0 * radius, py0 = p0 * radius;
  float px1 = p1 * radius, py1 = p1 * radius;
  float px2 = p2 * radius, py2 = p2 * radius;
  float px3 = p3 * radius, py3 = p3 * radius;
  float px4 = p4 * radius, py4 = p4 * radius;
    
  addPoint( x + radius, y );
  addCurve( x + px2, y + py2, x + px0, y + py4, x + px1, y + py3 );
  addCurve( x, y + radius, x + px3, y + py1, x + px4, y + py0 );
  addCurve( x - px2, y + py2, x - px4, y + py0, x - px3, y + py1 );
  addCurve( x - radius, y, x - px1, y + py3, x - px0, y + py4 );
  addCurve( x - px2, y - py2, x - px0, y - py4, x - px1, y - py3 );
  addCurve( x, y - radius, x - px3, y - py1, x - px4, y - py0 );
  addCurve( x + px2, y - py2, x + px4, y - py0, x + px3, y - py1 );
  addCurve( x + radius, y, x + px1, y - py3, x + px0, y - py4 );

}


gkg::PlanarFigureCircle::~PlanarFigureCircle()
{
}


//
// class PlanarFigureEllipse
//

gkg::PlanarFigureEllipse::PlanarFigureEllipse( const gkg::Brush* brush,
                                               const gkg::Color* stroke,
                                               const gkg::Color* fill,
                                               gkg::FloatCoordinate x,
                                               gkg::FloatCoordinate y,
                                               gkg::FloatCoordinate radiusX,
                                               gkg::FloatCoordinate radiusY )
                         : gkg::PlanarFigure( brush,
                                                 stroke,
                                                 fill,
                                                 true,
                                                 true,
                                                 25 )
{

  float px0 = p0 * radiusX, py0 = p0 * radiusY;
  float px1 = p1 * radiusX, py1 = p1 * radiusY;
  float px2 = p2 * radiusX, py2 = p2 * radiusY;
  float px3 = p3 * radiusX, py3 = p3 * radiusY;
  float px4 = p4 * radiusX, py4 = p4 * radiusY;
    
  addPoint( x + radiusX, y );
  addCurve( x + px2, y + py2, x + px0, y + py4, x + px1, y + py3 );
  addCurve( x, y + radiusY, x + px3, y + py1, x + px4, y + py0 );
  addCurve( x - px2, y + py2, x - px4, y + py0, x - px3, y + py1 );
  addCurve( x - radiusX, y, x - px1, y + py3, x - px0, y + py4 );
  addCurve( x - px2, y - py2, x - px0, y - py4, x - px1, y - py3 );
  addCurve( x, y - radiusY, x - px3, y - py1, x - px4, y - py0 );
  addCurve( x + px2, y - py2, x + px4, y - py0, x + px3, y - py1 );
  addCurve( x + radiusX, y, x + px1, y - py3, x + px0, y - py4 );

}


gkg::PlanarFigureEllipse::~PlanarFigureEllipse()
{
}


//
// class PlanarFigureOpenBSpline
//

gkg::PlanarFigureOpenBSpline::PlanarFigureOpenBSpline(
                                            const gkg::Brush* brush,
                                            const gkg::Color* stroke,
                                            const gkg::Color* fill,
                                            gkg::FloatCoordinate* x,
                                            gkg::FloatCoordinate* y, 
                                            int32_t count )
                             : gkg::PlanarFigure( brush,
                                                  stroke,
                                                  fill,
                                                  false,
                                                  true,
                                                  ( count + 2 ) * 3 + 1 )
{

  bSplineMoveTo( x[ 0 ], y[ 0 ], x[ 0 ], y[ 0 ], x[ 0 ], y[ 0 ] );
  bSplineCurveTo( x[ 0 ], y[ 0 ], x[ 0 ], y[ 0 ], x[ 1 ], y[ 1 ] );
  for ( int32_t i = 1; i < count - 1; i++ )
  {

    bSplineCurveTo( x[ i ], y[ i ],
                    x[ i - 1 ], y[ i - 1 ],
                    x[ i + 1 ], y[ i + 1 ] );

  }
  bSplineCurveTo( x[ count - 1 ], y[ count - 1 ],
                  x[ count - 2 ], y[ count - 2 ],
                  x[ count - 1 ], y[ count - 1 ] );
  bSplineCurveTo( x[ count - 1 ], y[ count - 1 ],
                  x[ count - 1 ], y[ count - 1 ],
                  x[ count - 1 ], y[ count - 1 ] );

}


gkg::PlanarFigureOpenBSpline::~PlanarFigureOpenBSpline()
{
}


//
// class PlanarFigureClosedBSpline
//

gkg::PlanarFigureClosedBSpline::PlanarFigureClosedBSpline(
                                                       const gkg::Brush* brush,
                                                       const gkg::Color* stroke,
                                                       const gkg::Color* fill,
                                                       gkg::FloatCoordinate* x,
                                                       gkg::FloatCoordinate* y,
                                                       int32_t count )
                               : gkg::PlanarFigure( brush,
                                                    stroke,
                                                    fill,
                                                    true,
                                                    true,
                                                    count * 3 + 1 )
{

  bSplineMoveTo( x[ 0 ], y[ 0 ],
                 x[ count - 1 ], y[ count - 1 ],
                 x[ 1 ], y[ 1 ] );
  for ( int32_t i = 1; i < count - 1; i++ )
  {

    bSplineCurveTo( x[ i ], y[ i ],
                    x[ i - 1 ], y[ i - 1 ],
                    x[ i + 1 ], y[ i + 1 ] );

  }
  bSplineCurveTo( x[ count - 1 ], y[ count - 1 ],
                  x[ count - 2 ], y[ count - 2 ],
                  x[ 0 ], y[ 0 ] );
  bSplineCurveTo( x[ 0 ], y[ 0 ],
                  x[ count - 1 ], y[ count - 1 ],
                  x[ 1 ], y[ 1 ] );

}


gkg::PlanarFigureClosedBSpline::~PlanarFigureClosedBSpline()
{
}


//
// class PlanarFigurePolyline
//

gkg::PlanarFigurePolyline::PlanarFigurePolyline( const gkg::Brush* brush,
                                                 const gkg::Color* stroke,
                                                 const gkg::Color* fill,
                                                 gkg::FloatCoordinate* x,
                                                 gkg::FloatCoordinate* y,
                                                 int32_t count )
                          : gkg::PlanarFigure( brush,
                                               stroke,
                                               fill,
                                               false,
                                               false,
                                               count )
{

  addPoint( x[ 0 ], y[ 0 ] );
  for ( int32_t i = 1; i < count; i++ )
  {

    addPoint( x[ i ], y[ i ] );

  }

}


gkg::PlanarFigurePolyline::~PlanarFigurePolyline()
{
}


//
// class PlanarFigurePolygon
//

gkg::PlanarFigurePolygon::PlanarFigurePolygon( const gkg::Brush* brush,
                                               const gkg::Color* stroke,
                                               const gkg::Color* fill,
                                               gkg::FloatCoordinate* x,
                                               gkg::FloatCoordinate* y,
                                               int32_t count )
                         : gkg::PlanarFigure( brush,
                                              stroke,
                                              fill,
                                              true,
                                              false,
                                              count )
{

  addPoint( x[ 0 ], y[ 0 ] );
  for ( int32_t i = 1; i < count; i++ )
  {

    addPoint( x[ i ], y[ i ] );

  }

}


gkg::PlanarFigurePolygon::~PlanarFigurePolygon()
{
}

