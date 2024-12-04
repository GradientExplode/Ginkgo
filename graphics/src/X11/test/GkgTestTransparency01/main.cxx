// label with transparency

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Label.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestTransparency01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::Glyph* label = new gkg::Label( "hello world!",
                                      widgetFactory->getFont(),
                                      new gkg::Color( 0, 1, 0, 0.7 ) );

  gkg::Background* background = new gkg::Background(
                                               label, 
                                               new gkg::Color( 1, 0, 0, 0.3 ) );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( background );

  int result = session.runWindow( aWindow );

  return result;

}
