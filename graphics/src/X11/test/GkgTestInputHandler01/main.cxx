// Rubber band


#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Image.h>
#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/TransformFitter.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>


//
// the rubber band itself
//

class Selector : public gkg::InputHandler
{

  public:

    Selector( gkg::Glyph* glyph, gkg::Style* style );
    virtual ~Selector();

    void draw( gkg::Canvas* canvas,
               const gkg::GeometryAllocation& geometryAllocation ) const;

    void press( const gkg::Event& event );
    void drag( const gkg::Event& event );
    void release( const gkg::Event& event );

  private:

    gkg::Brush* _brush;
    gkg::Color* _color;
    gkg::FloatCoordinate _x0;
    gkg::FloatCoordinate _y0;
    gkg::FloatCoordinate _x1;
    gkg::FloatCoordinate _y1;
    gkg::FloatCoordinate _x2;
    gkg::FloatCoordinate _y2;
    bool _drawn;
    bool _overlayDamage;
    bool _overlayDone;
};


Selector::Selector( gkg::Glyph* glyph, gkg::Style* style )
         : gkg::InputHandler( glyph, style ),
           _drawn( false ),
           _overlayDamage( false ),
           _overlayDone( false )
{

  gkg::FloatCoordinate width = 0;
  style->findAttribute( "brushWidth", width );

  _brush = new gkg::Brush( width );
  if ( _brush )
  {

    _brush->addReference();

  }

  _color = new gkg::Color(
       *gkg::LookAndFeel::getInstance().getWidgetFactory()->getBackground(),
       1.0f, gkg::Color::Xor );
  if ( _color )
  {

    _color->addReference();

  }

}


Selector::~Selector()
{

  if ( _brush )
  {

    _brush->removeReference();

  }
  if ( _color )
  {

    _color->removeReference();

  }

}


void Selector::draw( gkg::Canvas* canvas,
                    const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _overlayDamage )
  {

    canvas->pushTransform();
    gkg::Transformer identity;
    canvas->setTransformer( identity );
    if ( _drawn )
    {

      canvas->rectangle( _x0, _y0, _x1, _y1, _color, _brush );

    }

    Selector* selector = ( Selector* )this;
    if ( _overlayDone )
    {

      selector->_drawn = false;
      selector->_overlayDamage = false;
      selector->_overlayDone = false;

    }
    else
    {

      canvas->rectangle( _x0, _y0, _x2, _y2, _color, _brush );
      selector->_x1 = selector->_x2;
      selector->_y1 = selector->_y2;
      selector->_drawn = true;

    }
    canvas->popTransform();

  }
  else
  {

    this->gkg::InputHandler::draw( canvas, geometryAllocation );

  }

}


void Selector::press( const gkg::Event& event )
{

  _x0 = event.getPointerX();
  _y0 = event.getPointerY();
  this->gkg::InputHandler::press( event );

}


void Selector::drag( const gkg::Event& event )
{

  _x2 = event.getPointerX();
  _y2 = event.getPointerY();
  _overlayDamage = true;
  redraw();

}


void Selector::release( const gkg::Event& event )
{

  _overlayDamage = true;
  _overlayDone = true;
  redraw();
  this->gkg::InputHandler::release( event );

}


//
// main procedure
//


static gkg::OptionDescription options[] =
{

  { "-brush", "*brushWidth", gkg::OptionDescription::OptionValueNext, 0 },
  { 0, 0, gkg::OptionDescription::OptionNone, 0 }

};


static gkg::PropertyData properties[] =
{

  { "Rubband*brushWidth", "0" },
  { 0, 0 }

};


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestInputHandler01",
                      argc, argv, options, properties );

  gkg::Raster* raster = new gkg::Raster( 256, 256 );

  for ( gkg::PixelCoordinate y = 0; y < 256; y++ )
  {

    for ( gkg::PixelCoordinate x = 0; x < 256; x++ )
    {

      raster->poke( x, y, ( x + 1 ) / 256.0, ( y + 1 ) / 256.0, 0.0, 1.0 );

    }

  }

  gkg::Style* style = session.getDefaultDisplay()->getStyle();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  gkg::ApplicationWindow* 
    aWindow = new gkg::ApplicationWindow( 
               layoutFactory.createFlexible( 
                 new Selector( 
                   new gkg::TransformFitter( new gkg::Image( raster ) ),
                   style ) ) );

  int result = session.runWindow( aWindow );

  return result;

}
