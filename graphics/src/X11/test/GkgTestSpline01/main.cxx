// spline planar figure

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/PlanarFigure.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <iostream>


static gkg::FloatCoordinate x[] = { 100, 0, 0, 100, 100, 200, 300, 300, 200 };
static gkg::FloatCoordinate y[] = { 0, 100, 200, 300, 400, 400, 300, 200, 100 };
static int32_t cnt = 9;


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestSpline01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const gkg::Color* bkColor = widgetFactory->getBackground();
  gkg::Brush* brush = new gkg::Brush( 2.0 );
  gkg::Color* stroke = new gkg::Color( 1, 0, 0 );
  gkg::Color* fill = new gkg::Color( 1, 1, 0 );
  gkg::Glyph* spline = new gkg::PlanarFigureClosedBSpline( brush,
                                                           stroke,
                                                           fill,
                                                           x, y, cnt );

  gkg::Glyph* margin = layoutFactory.createMargin( spline, 10 );
  gkg::Glyph* flexible = layoutFactory.createFlexible( margin );
  gkg::Background* background = new gkg::Background( flexible, bkColor );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( background );

  int result = session.runWindow( aWindow );

  return result;

}
