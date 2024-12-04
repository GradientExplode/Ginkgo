// check box and radio button

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Button.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <iostream>


class Application 
{

  public:

    void message();
    void one();
    void two();

};


void Application::message()
{

  std::cout << "hi mom!" << std::endl;

}


void Application::one()
{

  std::cout << "one!" << std::endl;

}


void Application::two()
{

  std::cout << "two!" << std::endl;

}


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "Demo11", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  Application* application = new Application;

  gkg::ActionCallback< Application >* actionCallback1 =
    new gkg::ActionCallback< Application >( application,
                                            &Application::message );

  gkg::ActionCallback< Application >* actionCallback2 =
    new gkg::ActionCallback< Application >( application,
                                            &Application::one );

  gkg::ActionCallback< Application >* actionCallback3 =
    new gkg::ActionCallback< Application >( application,
                                            &Application::two );

  gkg::Glyph* checkBox = widgetFactory->createCheckBox( "Toggle me",
                                                        actionCallback1 );
  gkg::Glyph* vCenter1 = layoutFactory.createVCenter( checkBox );

  gkg::Glyph* glue = layoutFactory.createHGlue( 15 );

  gkg::TellTaleGroup* group = new gkg::TellTaleGroup;
  gkg::Button* radioButton1 = widgetFactory->createRadioButton( 
                                                              group,
                                                              "One",
                                                              actionCallback2 );
  radioButton1->getState()->set( gkg::TellTaleState::Chosen , true );
  gkg::Button* radioButton2 = widgetFactory->createRadioButton(
                                                              group,
                                                              "Two",
                                                              actionCallback3 );
  gkg::Glyph* vCenter2 = layoutFactory.createVCenter( 
                      layoutFactory.createVBox( radioButton1, radioButton2 ) );

  gkg::Glyph* hBox = layoutFactory.createHBox( vCenter1,
                                                glue,
                                                vCenter2 );

  gkg::Glyph* layoutMargin = layoutFactory.createMargin( hBox, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( layoutMargin ); 

  gkg::ApplicationWindow* 
    aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
