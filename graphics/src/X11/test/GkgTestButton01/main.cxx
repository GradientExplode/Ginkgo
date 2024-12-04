// standard labelled button with callback

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <iostream>


class Application 
{

  public:

    void message();

};


void Application::message()
{

  std::cout << "hi mom!" << std::endl;

}


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestButton01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  Application* application = new Application;

  gkg::ActionCallback< Application >* actionCallback =
    new gkg::ActionCallback< Application >( application, 
                                            &Application::message );

  gkg::Glyph* pushButton = widgetFactory->createPushButton( "Push me",
                                                            actionCallback );

  gkg::Glyph* layoutMargin = layoutFactory.createMargin( pushButton, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( layoutMargin ); 

  gkg::ApplicationWindow* 
    aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
