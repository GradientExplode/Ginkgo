// HBox and Character example

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Character.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestCharacter01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const gkg::Font* font = widgetFactory->getFont();
  const gkg::Color* fgColor = widgetFactory->getForeground();
  const gkg::Color* bkColor = widgetFactory->getBackground();

  gkg::Glyph* hBox = layoutFactory.createHBox(
                                new gkg::Character( 'g', font, fgColor ),
                                new gkg::Character( 'o', font, fgColor ),
                                new gkg::Character( 'o', font, fgColor ),
                                new gkg::Character( 'd', font, fgColor ),
                                new gkg::Character( 'b', font, fgColor ),
                                new gkg::Character( 'y', font, fgColor ),
                                new gkg::Character( 'e', font, fgColor ) );

  gkg::Background* background = new gkg::Background( hBox, bkColor );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( background );

  int result = session.runWindow( aWindow );

  return result;

}
