// Drag and drop
//
// This program creates a box with one object that can be dragged and
// one place to drag an object to.  You could run this program in parallel
// and drag from one window to the other.
//
// There are two options to control the visual part of dragging:
//    -useCursor true   (set the cursor to a cross to indicate dragging)
//    -useGlyph  true   (drag a glyph in a window to indicate dragging)
//

#include <iostream>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/Box.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-graphics-X11-core/Drag.h>
#include <gkg-graphics-X11-core/DragZone.h>
#include <gkg-graphics-X11-core/DragZoneSink.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>


//
// Glyph that can be dragged to a drag zone
//

class DragDemo : public gkg::Drag
{

  public:

    DragDemo( bool useCursor, bool useGlyph );

    gkg::Glyph* getDragGlyph();
    gkg::Cursor* getDragCursor();
    void getDragData( const char*& value, int32_t& length );

    gkg::Glyph* getGlyph();

  protected:

    bool _useGlyph;
    bool _useCursor;

};


DragDemo::DragDemo( bool useGlyph, bool useCursor )
         : gkg::Drag( 0 ),
           _useGlyph( useGlyph ),
           _useCursor( useCursor )
{

  setBody( getGlyph() );

}


gkg::Glyph* DragDemo::getDragGlyph()
{

  return _useGlyph ? getGlyph() : 0;

}


gkg::Cursor* DragDemo::getDragCursor()
{

  return _useCursor ? gkg::Cursor::getCrossHairs() : 0;

}


void DragDemo::getDragData( const char*& value, int32_t& length ) 
{

  value = "a drag example";
  length = 14;

}


gkg::Glyph* DragDemo::getGlyph()
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
                                                  
  return widgetFactory->createOutsetFrame( widgetFactory->createLabel("drag") );

}


//
// Glyph that can have things dropped in it
//

class DragZoneDemo : public gkg::DragZone
{

  public:

    DragZoneDemo();

    void enter( gkg::Event& event, const char* type, int32_t length );
    void leave( gkg::Event& event );

    void drop( gkg::Event& event, const char* data, int32_t length );

    void allocate( gkg::Canvas* canvas,
                   const gkg::GeometryAllocation& geometryAllocation,
                   gkg::GeometryExtension& geometryExtension );
    virtual void reallocate();

  private:

    gkg::Canvas* _canvas;
    gkg::GeometryAllocation _geometryAllocation;
    gkg::GeometryExtension _geometryExtension;
};


DragZoneDemo::DragZoneDemo()
             : gkg::DragZone( 0 )
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  setBody(widgetFactory->createInsetFrame(widgetFactory->createLabel("zone")) );

}


void DragZoneDemo::enter( gkg::Event&, const char*, int32_t )
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  setBody(widgetFactory->createOutsetFrame(widgetFactory->createLabel("zone")));
  reallocate();

}


void DragZoneDemo::leave( gkg::Event& )
{

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  setBody(widgetFactory->createInsetFrame(widgetFactory->createLabel("zone")));
  reallocate();

}


void DragZoneDemo::drop( gkg::Event& event, const char* data, int32_t )
{

  std::cout << data << std::endl;
  leave( event );

}


void DragZoneDemo::allocate(
                           gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  gkg::DragZone::allocate( canvas, geometryAllocation, geometryExtension );
  _canvas = canvas;
  _geometryAllocation = geometryAllocation;
  _geometryExtension = geometryExtension;

}


void DragZoneDemo::reallocate()
{

  _geometryExtension.clear();
  allocate( _canvas, _geometryAllocation, _geometryExtension );
  _canvas->setDamage( _geometryExtension );

}



//
// main procedure
//

static gkg::OptionDescription options[] =
{

  { "-useGlyph",  "*useGlyph",   gkg::OptionDescription::OptionValueNext, 0 },
  { "-useCursor", "*useCursor",  gkg::OptionDescription::OptionValueNext, 0 },
  { 0, 0, gkg::OptionDescription::OptionNone, 0 }

};


static gkg::PropertyData properties[] =
{

  { "*useGlyph", "false" },
  { "*useCursor", "true" },
  { 0, 0 }

};


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestDragAndDrop01",
                      argc, argv, options, properties );

  gkg::Style* style = session.getStyle();

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  bool useGlyph = style->getValueIsOn( "useGlyph" );
  bool useCursor = style->getValueIsOn( "useCursor" );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow(
           widgetFactory->createInsetFrame(
             layoutFactory.createMargin(
               new gkg::DragZoneSink(
                 new gkg::Background(
                   layoutFactory.createHBox(
                     new DragDemo( useGlyph, useCursor ),
                     new DragZoneDemo()	), widgetFactory->getBackground() ) ),
               10 ) ) );
  int result = session.runWindow( aWindow );

  return result;

}
