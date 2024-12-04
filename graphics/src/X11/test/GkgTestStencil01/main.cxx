// Stencil and Character example

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Character.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/Stencil.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-bitmaps/hand.bm>


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestStencil01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  const gkg::Font* font = widgetFactory->getFont();
  const gkg::Color* fgColor = widgetFactory->getForeground();
  const gkg::Color* bkColor = widgetFactory->getBackground();

  gkg::Bitmap* bitmap1 = new gkg::Bitmap( hand_bits, 
                                          hand_width,
                                          hand_height );
  gkg::Bitmap* bitmap2 = new gkg::Bitmap( hand_bits, 
                                          hand_width,
                                          hand_height,
                                          hand_x_hot,
                                          hand_y_hot );

  gkg::Stencil* stencil1 = new gkg::Stencil( bitmap1, fgColor );
  gkg::Stencil* stencil2 = new gkg::Stencil( bitmap2, fgColor );

  gkg::Glyph* hBox = layoutFactory.createHBox(
                                stencil1,
                                stencil2,
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
