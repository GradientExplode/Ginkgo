#include <cstdlib>
#include <iostream>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Target.h>
#include <gkg-graphics-X11-core/XYMarker.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/DisplayImplementation.h>
#include <gkg-graphics-X11-core/Visual.h>
#include "Axis.h"
#include "Graph.h"
#include "Plot.h"


const gkg::Color* getColor( const std::string& name )
{

  std::string value;
  gkg::Session& session = gkg::Session::getInstance();
  gkg::Style* style = gkg::Session::getInstance().getStyle();
  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  style->findAttribute( name, value );

  gkg::FloatColorIntensity r, g, b;
  gkg::DisplayImplementation* 
    dispImp = session.getDefaultDisplay()->getImplementation();
  if ( ( value != "" ) &&
       dispImp->getDefaultVisual()->findColor( value, r, g, b ) )
  {

    return new gkg::Color( r, g, b );

  }
  else
  {

    return widgetFactory->getForeground();

  }

}


static const int32_t bufferSize = 1000;


Graph::Graph( float w, float h,
              float xBegin, float xEnd,
              float yBegin, float yEnd,
              const gkg::Color* bg,
              const std::string& symbol )
      : gkg::InputHandler( 0, gkg::Session::getInstance().getStyle() )
{

  _init = false;
  _current = -1;
  _count = 0;
  _w = w;
  _h = h;
  _xOrigin = xBegin;
  _yOrigin = yBegin;
  _xRange = xEnd - xBegin;
  _yRange = yEnd - yBegin;
  _x = new gkg::FloatCoordinate[ bufferSize ];
  _y = new gkg::FloatCoordinate[ bufferSize ];

  _plot = new Plot( _count, _x, _y, getColor( "plot_color" ),
                    new gkg::Brush( 2 ) );
  _pPatch = new gkg::Patch( _plot );
  _marker = new gkg::XYMarker( _pPatch, 0, getColor( "mark_color" ) );
  _xAxis = new XAxis( xBegin, xEnd );
  _xPatch = new gkg::Patch( _xAxis );
  _yAxis = new YAxis( yBegin, yEnd );
  _yPatch = new gkg::Patch( _yAxis );
  _symbol = symbol;

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  setBody( new gkg::Background(
             layoutFactory.createMargin(
               layoutFactory.createVBox(
                 layoutFactory.createHBox(
                   _yPatch,
                   new gkg::Target( _marker, gkg::Target::PrimitiveHit )),
                   _xPatch ), 35, 15, 25, 15 ), bg ) );

}


Graph::~Graph()
{

  delete _x;
  delete _y;

}


void Graph::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  this->gkg::MonoGlyph::request( geometryRequisition );
  gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
  gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
  rx.setStretch( fil );
  rx.setShrink( 0.0 );
  rx.setAlignment( 0.0 );
  rx.setNatural( _w );
  ry.setStretch( fil );
  ry.setShrink( 0.0 );
  ry.setAlignment( 1.0 );
  ry.setNatural( _h );

}


void Graph::addPoint( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  if ( !_init )
  {

    _init = true;
    _xOrigin = x;
    _yOrigin = y;
    _xAxis->setRange( _xOrigin, _xOrigin + _xRange );
    _yAxis->setRange( _yOrigin, _yOrigin + _yRange );
    _xPatch->reallocate();
    _xPatch->redraw();
    _yPatch->reallocate();
    _yPatch->redraw();

  }
  if ( _count >= bufferSize )
  {

    std::cerr << "Too many data points." << std::endl;
    ::exit( 1 );

  }

  gkg::FloatCoordinate xLast = _xOrigin + _xRange;
  gkg::FloatCoordinate yLast = _yOrigin + _yRange;
  int i;
  if ( ( x > xLast ) || ( x < _xOrigin ) )
  {

    gkg::FloatCoordinate xOrig[ bufferSize ];
    for ( i = 0; i < _count; i++ )
    {

      xOrig[ i ] = _x[ i ] * _xRange + _xOrigin;

    }
    if ( x < _xOrigin )
    {

      _xOrigin -= ( xLast - x ) * 1.333;
      _xRange = xLast - _xOrigin;

    } 
    else
    {

      _xRange += ( x - _xOrigin ) * .1333;

    }

    for ( i = 0; i < _count; i++ )
    {

      _x[ i ] = ( xOrig[ i ] - _xOrigin ) / _xRange;

    }

    _xAxis->setRange( _xOrigin, _xOrigin + _xRange );
    _xPatch->reallocate();
    _xPatch->redraw();

  }                
  if ( ( y > yLast ) || ( y < _yOrigin ) )
  {

    gkg::FloatCoordinate yOrig[ bufferSize ];
    for ( i = 0; i < _count; i++ )
    {

      yOrig[ i ] = _y[ i ] * _yRange + _yOrigin;

    }

    if ( y < _yOrigin )
    {
 
      _yOrigin -= ( yLast - y ) * 1.333;
      _yRange = yLast - _yOrigin;

    }
    else
    {

      _yRange += ( y - _yOrigin ) * .1333;

    }

    for ( i = 0; i < _count; i++ )
    {

      _y[ i ] = ( yOrig[ i ] - _yOrigin ) / _yRange;

    }

    _yAxis->setRange( _yOrigin, _yOrigin + _yRange );
    _yPatch->reallocate();
    _yPatch->redraw();

  }                
  _x[ _count ] = ( x - _xOrigin ) / _xRange;
  _y[ _count ] = ( y - _yOrigin ) / _yRange;
  _count ++;
  _pPatch->redraw();

}


void Graph::mark( int32_t i )
{

  _current = i;
  if ( _current >= 0 )
  {

    const gkg::GeometryAllocation&
      geometryAllocation = _pPatch->getGeometryAllocation();
    gkg::FloatCoordinate l = geometryAllocation.getLeft();
    gkg::FloatCoordinate r = geometryAllocation.getRight();
    gkg::FloatCoordinate b = geometryAllocation.getBottom();
    gkg::FloatCoordinate t = geometryAllocation.getTop();
    gkg::FloatCoordinate x = l * ( 1 - _x[ _current ] ) + r * _x[ _current ];
    gkg::FloatCoordinate y = b * ( 1 - _y[ _current ] ) + t * _y[ _current ];
    _marker->mark( x - 4, y - 4, x + 4, y + 4 );

  }
  else
  {

    _marker->unmark();

  }

}


void Graph::allocate( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation,
                      gkg::GeometryExtension& geometryExtension )
{

  this->gkg::InputHandler::allocate( canvas,
                                     geometryAllocation,
                                     geometryExtension );
  mark( _current );

}


void Graph::press( const gkg::Event& event )
{

  if ( event.getType() == gkg::Event::Down )
  {

    gkg::Hit hit( &event );
    _pPatch->repick( 0, hit );
    if ( hit.hasAny() )
    {

      mark( ( int32_t )hit.getIndex( 0 ) );

    }
    else
    {

      mark( -1 );

    }

  }

}
