// top->bottom scrollbox with automatic scrollbar

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/TBAutoScrollBox.h>
#include <iostream>
#include <cstdio>


class Application 
{

  public:

    Application( gkg::TBAutoScrollBox* tbAutoScrollBox,
                 int32_t& count );

    void add();
    void remove();

  private:

    gkg::TBAutoScrollBox* _tbAutoScrollBox;
    int32_t& _count;

};


Application::Application( gkg::TBAutoScrollBox* tbAutoScrollBox, 
                          int32_t& count )
            : _tbAutoScrollBox( tbAutoScrollBox ),
              _count( count )
{
}


void Application::add()
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  std::cout << "add item " << _count + 1 << std::endl;
  
  char tmp[ 512 ];
  sprintf( tmp, "item %d", _count + 1 );
  _tbAutoScrollBox->append( widgetFactory->createLabel( tmp ) );
  _count ++;

}


void Application::remove()
{

  if ( _count - 1 >= 0 )
  {

    std::cout << "remove item " << _count << std::endl;
    _tbAutoScrollBox->remove( _count - 1 );
    _count --;

  }

}


int main( int argc, char* argv[] )
{

  int32_t count;

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestTBAutoScrollBox01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  // allocating scrollbox and associated slider
  gkg::TBAutoScrollBox*
    tbAutoScrollBox = layoutFactory.createVAutoScrollBox( false );

  for ( count = 0; count < 5; count++ )
  {

    char tmp[ 512 ];
    sprintf( tmp, "item %d", count + 1 );
    tbAutoScrollBox->append( widgetFactory->createLabel( tmp ) );

  }

  // allocating application with callbacks
  Application* application = new Application( tbAutoScrollBox, count );

  gkg::Glyph* pushButtonAdd = widgetFactory->createPushButton( 
                  "Add item",
                  new gkg::ActionCallback< Application >( application,
                                                          &Application::add ) );
  gkg::Glyph* pushButtonRemove = widgetFactory->createPushButton(
               "Remove item",
               new gkg::ActionCallback< Application >( application,
                                                       &Application::remove ) );

  // putting alltogether
  gkg::ApplicationWindow* aWindow =
    new gkg::ApplicationWindow(
      widgetFactory->createInsetFrame( 
        layoutFactory.createMargin(
          layoutFactory.createHBox( tbAutoScrollBox,
                                    layoutFactory.createHGlue( 15 ),
                                    layoutFactory.createVBox(
                                      layoutFactory.createVGlue( 15 ),
                                      pushButtonAdd,
                                      pushButtonRemove,
                                      layoutFactory.createVGlue( 15 ) ) ),
          10 ) ) );

  int result = session.runWindow( aWindow );

  delete aWindow;

  return result;

}
