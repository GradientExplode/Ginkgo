// field editor

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/FieldEditor.h>


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestFieldEditor01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();


  gkg::FieldEditor*  fieldEditor = new gkg::FieldEditor( "hello world",
                                                         session.getStyle(),
                                                         0,
                                                         80.0f );

  gkg::Glyph* layoutMargin = layoutFactory.createMargin( fieldEditor, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( layoutMargin ); 

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
