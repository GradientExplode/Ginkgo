#include <gkg-processing-plot/CurvePlotterImplementation.h>
#include <cmath>
#include <iostream>


gkg::CurvePlotterImplementation::CurvePlotterImplementation()
                  : _tickType( gkg::CurvePlotterImplementation::PointTick ),
                    _lineType( gkg::CurvePlotterImplementation::AxisLine ),
                    _justifyType( gkg::CurvePlotterImplementation::LeftJustify )
{
}


gkg::CurvePlotterImplementation::~CurvePlotterImplementation()
{
}


void gkg::CurvePlotterImplementation::drawPoint(
                            double x, double y,
                            gkg::CurvePlotterImplementation::TickType tickType )
{

  double dx = getTicWidth() / 2;
  double dy = getTicHeight() / 2;

  switch ( tickType )
  {


    case gkg::CurvePlotterImplementation::PointTick:

      move( x, y );
      drawVector( x, y );
      break;

    case gkg::CurvePlotterImplementation::DiamondTick:

      move( x - dx, y );
      drawVector( x, y - dy );
      drawVector( x + dx, y );
      drawVector( x, y + dy );
      drawVector( x - dx, y );
      move( x, y );
      drawVector( x, y );
      break;

    case gkg::CurvePlotterImplementation::PlusTick:

      move( x - dx, y );
      drawVector( x - dx, y );
      drawVector( x + dx, y );
      move( x, y - dy );
      drawVector( x, y - dy );
      drawVector( x, y + dy );
      break;

    case gkg::CurvePlotterImplementation::BoxTick:

      move( x - dx, y - dy );
      drawVector( x - dx, y - dy );
      drawVector( x + dx, y - dy );
      drawVector( x + dx, y + dy );
      drawVector( x - dx, y + dy );
      drawVector( x - dx, y - dy );
      move( x, y );
      drawVector( x, y );
      break;

    case gkg::CurvePlotterImplementation::CrossTick:

      move( x - dx, y - dy );
      drawVector( x - dx, y - dy );
      drawVector( x + dx, y + dy );
      move( x - dx, y + dy );
      drawVector( x - dx, y + dy );
      drawVector( x + dx, y - dy );
      break;

    case gkg::CurvePlotterImplementation::TriangleTick:

      move( x, y + ( 4 * dy / 3 ) );
      drawVector( x - ( 4 * dx / 3 ), y - ( 2 * dy / 3 ) );
      drawVector( x + ( 4 * dx / 3 ), y - ( 2 * dy / 3 ) );
      drawVector( x, y + ( 4 * dy / 3 ) );
      move( x, y );
      drawVector( x, y );
      break;

    case gkg::CurvePlotterImplementation::StarTick:

      move( x - dx, y );
      drawVector( x - dx, y );
      drawVector( x + dx, y );
      move( x, y - dy );
      drawVector( x, y - dy );
      drawVector( x, y + dy );
      move( x - dx, y - dy );
      drawVector( x - dx, y - dy );
      drawVector( x + dx, y + dy );
      move( x - dx, y + dy );
      drawVector( x - dx, y + dy );
      drawVector( x + dx, y - dy );
      break;

  }

}


void gkg::CurvePlotterImplementation::drawArrow( double startX, double startY,
                                                 double endX, double endY,
                                                 bool hasHead )
{

  double length = ( getTicWidth() + getTicHeight() ) / 2.0;

  // drawing the line
  move( startX, startY );
  drawVector( endX, endY );

  if ( hasHead )
  {

    // drawing the arrow itself
    if ( startX == endX )
    {

      double delta = length / std::sqrt( 2.0 );
      if ( startY < endY )
      {

        delta = -delta;

      }
      move( endX - delta, endY + delta );
      drawVector( endX, endY );
      drawVector( endX + delta, endY + delta );

    }
    else
    {

      double dx = startX - endX;
      double dy = startY - endY;
      double coefficient = length / std::sqrt( 2.0 * ( dx * dx + dy * dy ) );
      move( endX + ( dx + dy ) * coefficient,
            endY + ( dy - dx ) * coefficient );
      drawVector( endX, endY );
      drawVector( endX + ( dx - dy ) * coefficient,
                  endY + ( dy + dx ) * coefficient );

    }

  }

}


void gkg::CurvePlotterImplementation::drawPoly( const std::vector< double >& x,
                                                const std::vector< double >& y )
{

  if ( x.size() != y.size() )
  {

    std::cerr << "gkg::CurvePlotterImplementation::drawPoly: x and y don't have"
              << " same size(s)"
              << std::endl;

  }
  move( x[ 0 ], y[ 0 ] );
  for ( size_t i = 1; i < x.size(); i++ )
  {

    drawVector( x[ i ], y[ i ] );

  }

}


