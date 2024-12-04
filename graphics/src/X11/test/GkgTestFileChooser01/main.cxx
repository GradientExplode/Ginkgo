// file chooser

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/FileChooser.h>
#include <cstdlib>


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestFileChooser01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  gkg::FileChooser* fileChooser = new gkg::FileChooser( getenv( "HOME" ),
                                                        session.getStyle() );

  gkg::Glyph* layoutMargin = layoutFactory.createMargin( fileChooser, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( layoutMargin ); 

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
