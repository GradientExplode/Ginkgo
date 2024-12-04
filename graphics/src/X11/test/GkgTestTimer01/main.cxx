// timer(s)

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-dispatcher/IOCallback.h>
#include <iostream>


class Application 
{

  public:

    Application();

    void button();
    void tick( int64_t, int64_t );

};


Application::Application()
{
}


void Application::button()
{

  std::cerr << "button pressed" << std::endl;

}


static gkg::IOHandler* handler = 0;


void Application::tick( int64_t, int64_t )
{

  std::cerr << "tick" << std::endl;
  gkg::Dispatcher::getInstance().startTimer( 1, 0, handler );

}



int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestTimer01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  Application* application = new Application;

  gkg::ActionCallback< Application >* actionCallback =
    new gkg::ActionCallback< Application >( application, &Application::button );

  gkg::Glyph* pushButton = widgetFactory->createPushButton( "Press the button",
                                                            actionCallback );
  gkg::Glyph* layoutMargin = layoutFactory.createMargin( pushButton, 10 );

  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( layoutMargin ); 

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  handler = new gkg::IOCallback< Application >( application,
                                                &Application::tick );
  gkg::Dispatcher::getInstance().startTimer( 1, 0, handler );

  int result = session.runWindow( aWindow );

  return result;

}
