// menu and sub-menus

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/Menu.h>
#include <iostream>


struct CommandInfo;

//
// class Application
//
class Application
{

  public:

    void open();
    void save();
    void quit();

    void cut();
    void copy();
    void paste();

    void black();
    void red();
    void green();
    void blue();

    gkg::Menu* createMenubar( CommandInfo* commandInfos );

  private:

    gkg::Menu* createPulldown( CommandInfo* commandInfos, int32_t opt );

};


//
// struct CommandInfo
//

struct CommandInfo
{

  const char* label;
  gkg::ActionCallback< Application >::MemberFunction callback;
  CommandInfo* subMenu;
  int32_t options;

};


//
// menu desciption
//

CommandInfo fileMenu[] =
{

  { "Open", &Application::open, 0, 0 },
  { "Save", &Application::save, 0, 0 },
  { "", 0, 0, 0 },
  { "Quit", &Application::quit, 0, 0 },
  { 0, 0, 0, 0 }

};


CommandInfo editMenu[] =
{

  { "Cut", &Application::cut, 0, 0 },
  { "Copy", &Application::copy, 0, 0 },
  { "Paste", &Application::paste, 0, 0 },
  { 0, 0, 0, 0 }

};


CommandInfo colorMenu[] =
{

  { "Black", &Application::black, 0, 0 },
  { "Red", &Application::red, 0, 0 },
  { "Green", &Application::green, 0, 0 },
  { "Blue", &Application::blue, 0, 0 },
  { 0, 0, 0, 0 }

};


CommandInfo menuBar[] =
{

  { "File", 0, fileMenu, 0 },
  { "Edit", 0, editMenu, 1 },
  { "Color", 0, colorMenu, 2 },
  { 0, 0, 0, 0 }

};


//
// Application methods
//

void Application::open()
{

  std::cout << "open" << std::endl;

}


void Application::save()
{

  std::cout << "save" << std::endl;

}


void Application::quit()
{

  gkg::Session::getInstance().quit();

}


void Application::cut()
{

  std::cout << "cut" << std::endl;

}



void Application::copy()
{

  std::cout << "copy" << std::endl;

}


void Application::paste()
{

  std::cout << "paste" << std::endl;

}


void Application::black()
{

  std::cout << "black" << std::endl;

}


void Application::red()
{

  std::cout << "red" << std::endl;

}


void Application::green()
{

  std::cout << "green" << std::endl;

}


void Application::blue()
{

  std::cout << "blue" << std::endl;

}


gkg::Menu* Application::createMenubar( CommandInfo* commandInfos )
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::Menu* menu = widgetFactory->createMenubar();
  for ( CommandInfo* i = commandInfos; i->label; i++ )
  {

    gkg::MenuItem* menuItem = widgetFactory->createMenubarItem(
                                 widgetFactory->createFancyLabel( i->label ) );
    menuItem->setMenu( createPulldown( i->subMenu, i->options ) );
    menu->appendItem( menuItem );

  }
  menu->getItem(0)->getMenu()->getItem( 1 )->getState()->set(
                                           gkg::TellTaleState::Enabled, false );
  return menu;

}


gkg::Menu* Application::createPulldown( CommandInfo* commandInfos, int opt )
{

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  gkg::Menu* menu = widgetFactory->createPulldown();
  gkg::TellTaleGroup* group = 0;
  for ( CommandInfo* i = commandInfos; i->label; i++ )
  {

    if ( i->label[ 0 ] == '\0' )
    {

      menu->appendItem( widgetFactory->createMenuItemSeparator() );

    }
    else
    {

      gkg::Glyph* glyph = layoutFactory.createRMargin(
                                    widgetFactory->createFancyLabel( i->label ),
                                    0.0, fil, 0.0 );
      gkg::MenuItem* menuItem;
      switch ( opt )
      {

        case 1:

          menuItem = widgetFactory->createCheckMenuItem( glyph );
          break;

        case 2:

          if ( !group )
          {

            group = new gkg::TellTaleGroup;

          }
          menuItem = widgetFactory->createRadioMenuItem( group, glyph );
          break;

        default:

          menuItem = widgetFactory->createMenuItem( glyph );
          break;

      }
      if ( !i->callback && i->subMenu )
      {

        menuItem->setMenu( createPulldown( i->subMenu, i->options ) );

      }
      else
      {

        menuItem->setAction( new gkg::ActionCallback< Application >( this,
                                                                i->callback ) );

      }
      menu->appendItem( menuItem );

    }

  }
  return menu;

}


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestMenu01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  Application* application = new Application;

  gkg::Glyph* bar = application->createMenubar( menuBar );
  gkg::Glyph* margin = layoutFactory.createMargin( bar, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( margin );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
