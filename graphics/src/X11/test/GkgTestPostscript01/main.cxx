// postscript printing

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <fstream>
#include <string>


class Printable : public gkg::Patch 
{

  public:

    Printable( gkg::Glyph* glyph );
    virtual ~Printable();

    virtual void doPrint();

};


Printable::Printable( gkg::Glyph* glyph )
          : gkg::Patch( glyph )
{
}


Printable::~Printable() 
{ 
}


void Printable::doPrint()
{

  std::ofstream out( "out.ps" );
  gkg::Printer* printer = new gkg::Printer( &out );
  printer->prolog( "tmp" );
  const gkg::GeometryAllocation& 
    geometryAllocation = getGeometryAllocation();
  printer->resize( geometryAllocation.getLeft(),
                   geometryAllocation.getBottom(),
                   geometryAllocation.getRight(),
                   geometryAllocation.getTop() );
  printer->setPage( "1" );
  gkg::Glyph* glyph = getBody();
  glyph->print( printer, geometryAllocation );
  printer->epilog();
  glyph->undraw();
  reallocate();
  redraw();
  out.flush();
  out.close();
  delete printer;

}



int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestPostscript01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  Printable* printable = new Printable( 0 );

  gkg::Glyph* button1 = widgetFactory->createPushButton( "Print",
                  new gkg::ActionCallback< Printable >( printable,
                                                        &Printable::doPrint ) );
  gkg::Glyph* glue = layoutFactory.createHGlue( 5 );
  gkg::Glyph* button2 = widgetFactory->createPushButton( "Quit",
                                            widgetFactory->createQuitAction() );

  gkg::Glyph* hBox = layoutFactory.createHBox( button1, glue, button2 );

  gkg::Glyph* margin = layoutFactory.createMargin( hBox, 10 );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( margin );

  printable->setBody( insetFrame );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( printable );

  int result = session.runWindow( aWindow );

  return result;

}
