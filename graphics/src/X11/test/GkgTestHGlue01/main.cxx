// HBox, Label & HGlue example

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestHGlue01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const gkg::Color* bkColor = widgetFactory->getBackground();

  gkg::Glyph* label1 = widgetFactory->createLabel( "good" );
  gkg::Glyph* glue = layoutFactory.createHGlue();
  gkg::Glyph* label2 = widgetFactory->createLabel( "bye" );

  gkg::Glyph* hBox = layoutFactory.createHBox( label1, glue, label2 );

  gkg::Background* background = new gkg::Background( hBox, bkColor );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( background );

  int result = session.runWindow( aWindow );

  return result;

}
