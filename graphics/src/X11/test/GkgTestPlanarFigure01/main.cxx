// planar figure, brush & patterns, flexible layout

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/PlanarFigure.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <iostream>


static int32_t pattern[] = { 10, 5 };


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestPlanarFigure01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const gkg::Color* bkColor = widgetFactory->getBackground();
  gkg::Brush* brush = new gkg::Brush( pattern, 2, 3.0 );
  gkg::Color* stroke = new gkg::Color( 1, 0, 0 );
  gkg::Color* fill = new gkg::Color( 0, 1, 0 );
  gkg::Glyph* circle = new gkg::PlanarFigureCircle( brush, stroke, fill,
                                                    50, 50, 50 );

  gkg::Glyph* margin = layoutFactory.createMargin( circle, 10 );
  gkg::Glyph* flexible = layoutFactory.createFlexible( margin );
  gkg::Background* background = new gkg::Background( flexible, bkColor );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( background );

  int result = session.runWindow( aWindow );

  return result;

}
