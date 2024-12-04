#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Label.h>
#include <gkg-graphics-X11-core/Layout.h>
#include <gkg-graphics-X11-core/Page.h>
#include <gkg-graphics-X11-core/VRule.h>
#include <gkg-graphics-X11-core/HRule.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <string>
#include <cstdio>
#include "Axis.h"


//
// static variables and functions
//

static float xinc = 40.0;
static float yinc = 20.0;
extern const gkg::Color* getColor( const std::string& name );


static gkg::Glyph** makeYTicks( int32_t count,
                                float first, float last,
                                const std::string& format,
                                const gkg::Font* font,
                                const gkg::Color* color )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  char label[ 100 ];
  gkg::Glyph** ticks = new gkg::Glyph*[ count ];
  float step = ( count > 1 ) ? ( last - first ) / ( count - 1 ) : 0;
  for ( int32_t i = 0; i < count; i++ ) 
  {

    sprintf( label, format.c_str(), first + i * step );
    ticks[ i ] = layoutFactory.createVCenter(
                   layoutFactory.createHCenter(
                     layoutFactory.createRMargin(
                       new gkg::Label( ( std::string )label, font, color ),
                       3.0 ), 1.0 ), 0.5 );

  }
  return ticks;

}


static gkg::Glyph** makeXTicks( int32_t count,
                                float first, float last,
                                const std::string& format,
                                const gkg::Font* font,
                                const gkg::Color* color )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  char label[ 100 ];
  gkg::Glyph** ticks = new gkg::Glyph*[ count ];
  float step = ( count > 1 ) ? ( last - first ) / ( count - 1 ) : 0;
  for ( int32_t i = 0; i < count; i++ ) 
  {

    sprintf( label, format.c_str(), first + i * step );
    ticks[ i ] = layoutFactory.createHCenter(
                   layoutFactory.createVCenter(
                     layoutFactory.createTMargin(
                       new gkg::Label( ( std::string )label, font, color ),
                       3.0 ), 1.0 ), 0.5 );

  }
  return ticks;

}


//
// class Axis
//

Axis::Axis( float first, float last )
     : gkg::MonoGlyph( 0 ),
       _page( 0 ),
       _first( first ),
       _last( last )
{
}


Axis::~Axis()
{
}


void Axis::allocate( gkg::Canvas* canvas,
                     const gkg::GeometryAllocation& geometryAllocation,
                     gkg::GeometryExtension& geometryExtension )
{

  _geometryAllocation = geometryAllocation;
  setRange( _first, _last );
  _page->allocate( canvas,  geometryAllocation, geometryExtension );

}


//
// class YAxis
//

YAxis::YAxis( float first, float last )
      : Axis( first, last )
{

  setRange( first, last );

}


YAxis::~YAxis()
{
}


void YAxis::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  this->gkg::MonoGlyph::request( geometryRequisition );
  gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
  gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
  rx.setStretch( 0.0 );
  rx.setShrink( 0.0 );
  rx.setAlignment( 0.0 );
  ry.setStretch( fil );
  ry.setShrink( fil );
  ry.setAlignment( 1.0 );

}


void YAxis::setRange( float first, float last )
{

  gkg::FloatCoordinate bottom, top;

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  bottom = _geometryAllocation.getBottom();
  top = _geometryAllocation.getTop();
  _first = first;
  _last = last;
  _page = new gkg::Page( new gkg::VRule( getColor( "axis_color" ), 1 ) );
  int32_t count = ( int32_t )( ( top - bottom ) / yinc );
  count = ( count > 2 ) ? count : 2;
  gkg::Glyph** ticks = makeYTicks( count,
                                   _first, _last,
                                   "%3.2f",
                                   widgetFactory->getFont(),
                                   getColor( "tick_color" ) );
  for ( int32_t i = 0; i < count; i++ )
  {

    _page->insert( i, ticks[ i ] );

  }
  if ( count == 1 )
  {

    _page->move( 0, 0, 0 );

  }
  else if ( count > 1 )
  {

    gkg::FloatCoordinate diff = _geometryAllocation.getTop() - 
                                _geometryAllocation.getBottom();
    gkg::FloatCoordinate dy = diff / ( count - 1 );
    for ( int32_t i = 0; i < count; i++ )
    {

      _page->move( i, 0, i * dy - diff );

    }

  }
  setBody( _page );

}


//
// class XAxis
//
XAxis::XAxis( float first, float last )
      : Axis( first, last )
{

  setRange( first, last );

}


XAxis::~XAxis()
{
}


void XAxis::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  this->gkg::MonoGlyph::request( geometryRequisition );
  gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
  gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
  rx.setStretch( fil );
  rx.setShrink( fil );
  rx.setAlignment( 0.0 );
  ry.setStretch( 0.0 );
  ry.setShrink( 0.0 );
  ry.setAlignment( 0.0 );

}


void XAxis::setRange( float first, float last )
{

  gkg::FloatCoordinate left, right;

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  left = _geometryAllocation.getLeft();
  right = _geometryAllocation.getRight();
  _first = first;
  _last = last;
  _page = new gkg::Page( new gkg::HRule( getColor( "axis_color" ), 1 ) );
  int32_t count = ( int32_t )( ( right - left ) / xinc );
  count = ( count > 2 ) ? count : 2;
  gkg::Glyph** ticks = makeXTicks( count,
                                   _first, _last,
                                   "%3.2f",
                                   widgetFactory->getFont(),
                                   getColor( "tick_color" ) );
  for ( int32_t i = 0; i < count; i++ )
  {

    _page->insert( i, ticks[ i ] );

  }
  if ( count == 1 )
  {

    _page->move( 0, 0, 0 );

  }
  else if ( count > 1 )
  {

    gkg::FloatCoordinate diff = _geometryAllocation.getRight() - 
                              _geometryAllocation.getLeft();
    gkg::FloatCoordinate dx = diff / ( count - 1 );
    for ( int32_t i = 0; i < count; i++ )
    {

      _page->move( i, i * dx, 0 );

    }

  }
  setBody( _page );

}

