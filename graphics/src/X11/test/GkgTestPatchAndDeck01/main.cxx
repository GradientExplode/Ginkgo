// patch and deck
// patch are used to redraw sub-part of a canvas without requiring to
// refresh the entire canvas

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Deck.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <fstream>
#include <string>


class Application
{

  public:

    gkg::Glyph* make();
    void next();
    void previous();

  private:

    void flip( gkg::Glyph::Index index );

    gkg::Deck* _deck;
    gkg::Patch* _patch;

};


gkg::Glyph* Application::make()
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  _deck = layoutFactory.createDeck();
  _deck->append( widgetFactory->createLabel( "Hi mom!" ) );
  _deck->append( widgetFactory->createLabel( "Big Bird") );
  _deck->append( widgetFactory->createLabel( "Oscar") );
  _deck->flipTo( 0 );
  _patch = new gkg::Patch( _deck );
  return _patch;

}


void Application::next()
{

  gkg::Glyph::Index currentIndex = _deck->getCard() + 1;
  flip( currentIndex == _deck->getCount() ? 0 : currentIndex );

}


void Application::previous()
{

  gkg::Glyph::Index currentIndex = _deck->getCard() - 1;
  flip( currentIndex == -1 ? _deck->getCount() - 1 : currentIndex );

}


void Application::flip( gkg::Glyph::Index index )
{

  _deck->flipTo( index );
  _patch->redraw();

}


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestPatchAndDeck01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  Application* application = new Application;

  gkg::Glyph* buttonNext = widgetFactory->createPushButton( "Next",
                 new gkg::ActionCallback< Application >( application,
                                                         &Application::next ) );
  gkg::Glyph* glue = layoutFactory.createHGlue( 10 );
  gkg::Glyph* buttonPrev = widgetFactory->createPushButton( "Previous",
             new gkg::ActionCallback< Application >( application,
                                                     &Application::previous ) );

  gkg::Glyph* hBox = layoutFactory.createHBox( buttonNext,
                                               glue,
                                               buttonPrev );

  gkg::Glyph* vBox = layoutFactory.createVBox( application->make(),
                                               layoutFactory.createVGlue( 5 ),
                                               hBox );

  gkg::Glyph* margin = layoutFactory.createMargin( vBox, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( margin );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
