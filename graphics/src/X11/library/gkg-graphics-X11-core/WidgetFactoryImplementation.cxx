#include <iostream>
#include <gkg-graphics-X11-core/WidgetFactoryImplementation.h>
#include <gkg-graphics-X11-core/WidgetFactory.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/BitmapCursor.h>
#include <gkg-graphics-X11-core/Button.h>
#include <gkg-graphics-X11-core/Menu.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>


#include <gkg-graphics-X11-bitmaps/hand.bm>
#include <gkg-graphics-X11-bitmaps/handMask.bm>
#include <gkg-graphics-X11-bitmaps/lfast.bm>
#include <gkg-graphics-X11-bitmaps/lfastMask.bm>
#include <gkg-graphics-X11-bitmaps/lufast.bm>
#include <gkg-graphics-X11-bitmaps/lufastMask.bm>
#include <gkg-graphics-X11-bitmaps/ufast.bm>
#include <gkg-graphics-X11-bitmaps/ufastMask.bm>
#include <gkg-graphics-X11-bitmaps/rufast.bm>
#include <gkg-graphics-X11-bitmaps/rufastMask.bm>
#include <gkg-graphics-X11-bitmaps/rfast.bm>
#include <gkg-graphics-X11-bitmaps/rfastMask.bm>
#include <gkg-graphics-X11-bitmaps/rdfast.bm>
#include <gkg-graphics-X11-bitmaps/rdfastMask.bm>
#include <gkg-graphics-X11-bitmaps/dfast.bm>
#include <gkg-graphics-X11-bitmaps/dfastMask.bm>
#include <gkg-graphics-X11-bitmaps/ldfast.bm>
#include <gkg-graphics-X11-bitmaps/ldfastMask.bm>


//
// class LocalOverlay
//

namespace gkg
{


class LocalOverlay : public Glyph
{

  public:

    LocalOverlay( Glyph* g1, Glyph* g2, Glyph* g3 );
    virtual ~LocalOverlay();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

  private:

    Glyph* _first;
    Glyph* _second;
    Glyph* _third;

};


}


gkg::LocalOverlay::LocalOverlay( gkg::Glyph* g1,
                                 gkg::Glyph* g2,
                                 gkg::Glyph* g3 )
                  : _first( g1 ),
                    _second( g2 ),
                    _third( g3 )
{

  if ( _first )
  {

    _first->addReference();

  }
  if ( _second )
  {

    _second->addReference();

  }
  if ( _third )
  {

    _third->addReference();

  }

}


gkg::LocalOverlay::~LocalOverlay()
{

  if ( _first )
  {

    _first->removeReference();

  }
  if ( _second )
  {

    _second->removeReference();

  }
  if ( _third )
  {

    _third->removeReference();

  }

}


void gkg::LocalOverlay::request( 
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  _third->request( geometryRequisition );

}


void 
gkg::LocalOverlay::allocate( gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation,
                             gkg::GeometryExtension& geometryExtension )
{

  _first->allocate( canvas, geometryAllocation, geometryExtension );
  _second->allocate( canvas, geometryAllocation, geometryExtension );
  _third->allocate( canvas, geometryAllocation, geometryExtension );

}


void gkg::LocalOverlay::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  _first->draw( canvas, geometryAllocation );
  _second->draw( canvas, geometryAllocation );
  _third->draw( canvas, geometryAllocation );

}


void gkg::LocalOverlay::pick( gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              int32_t depth,
                              gkg::Hit& hit )
{

  _third->pick( canvas, geometryAllocation, depth, hit );

}


//
// class WidgetFactoryImplementation
//

gkg::WidgetFactoryImplementation::WidgetFactoryImplementation(
                                             gkg::WidgetFactory* widgetFactory )
                                 : _target( widgetFactory ),
                                   _coloringMode( gkg::Polychrome ),
                                   _style( 0 ),
                                   _styleChanged( false ),
                                   _font( 0 ),
                                   _foreground( 0 ),
                                   _background( 0 ),
                                   _handCursor( 0 ),
                                   _lFastCursor( 0 ),
                                   _luFastCursor( 0 ),
                                   _uFastCursor( 0 ),
                                   _ruFastCursor( 0 ),
                                   _rFastCursor( 0 ),
                                   _rdFastCursor( 0 ),
                                   _dFastCursor( 0 ),
                                   _ldFastCursor( 0 ),
                                   _initializedLabelStyles( false ),
                                   _chiseledLabelStyle( "" ),
                                   _raisedLabelStyle( "" ),
                                   _savedText( "" )
{


  setStyle( gkg::Session::getInstance().getStyle() );

}


gkg::WidgetFactoryImplementation::~WidgetFactoryImplementation()
{

  if ( _style )
  {

    _style->removeReference();

  }
  if ( _font )
  {

    _font->removeReference();

  }
  if ( _foreground )
  {

    _foreground->removeReference();

  }
  if ( _background )
  {

    _background->removeReference();

  }
  delete _handCursor;
  delete _lFastCursor;
  delete _luFastCursor;
  delete _uFastCursor;
  delete _ruFastCursor;
  delete _rFastCursor;
  delete _rdFastCursor;
  delete _dFastCursor;
  delete _ldFastCursor;

}


gkg::WidgetFactory* gkg::WidgetFactoryImplementation::getTarget() const
{

  return _target;

}


void gkg::WidgetFactoryImplementation::setColoringMode(
                                                gkg::ColoringMode coloringMode )
{

  _coloringMode = coloringMode;

}


gkg::ColoringMode gkg::WidgetFactoryImplementation::getColoringMode() const
{

  return _coloringMode;

}


void gkg::WidgetFactoryImplementation::setStyle( gkg::Style* style )
{

  if ( style )
  {

    style->addReference();

  }
  if ( _style )
  {

    _style->removeReference();

  }
  _style = style;
  _styleChanged = true;

}


gkg::Style* gkg::WidgetFactoryImplementation::getStyle() const
{

  return _style;

}


const gkg::Font* gkg::WidgetFactoryImplementation::getFont()
{

  update();
  return _font;

}


const gkg::Color* gkg::WidgetFactoryImplementation::getForeground()
{

  update();
  return _foreground;

}


const gkg::Color* gkg::WidgetFactoryImplementation::getBackground()
{

  update();
  return _background;

}


void gkg::WidgetFactoryImplementation::setAlias( const std::string& alias )
{

  _style->setAlias( alias );

}


void gkg::WidgetFactoryImplementation::pushStyle()
{

  _styles.push_front( _style );
  if ( _style )
  {

    _style->addReference();

  }

}


void gkg::WidgetFactoryImplementation::popStyle()
{

  if ( !_styles.empty() )
  {

    gkg::Style* style = _styles.front();
    _styles.pop_front();
    _target->setStyle( style );
    if ( style )
    {

      style->removeReference();

    }
    _target->styleChanged( style );

  }

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getHandCursor()
{

  if ( !_handCursor )
  {

    _handCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( hand_bits,
                                                  hand_width,
                                                  hand_height,
                                                  hand_x_hot,
                                                  hand_y_hot ),
                              new gkg::Bitmap( hand_mask_bits,
                                                  hand_mask_width,
                                                  hand_mask_height ) );

  }
  return _handCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getLFastCursor()
{

  if ( !_lFastCursor )
  {

    _lFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( lfast_bits,
                                                  lfast_width,
                                                  lfast_height,
                                                  lfast_x_hot,
                                                  lfast_y_hot ),
                              new gkg::Bitmap( lfast_mask_bits,
                                                  lfast_mask_width,
                                                  lfast_mask_height ) );

  }
  return _lFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getLUFastCursor()
{

  if ( !_luFastCursor )
  {

    _luFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( lufast_bits,
                                                  lufast_width,
                                                  lufast_height,
                                                  lufast_x_hot,
                                                  lufast_y_hot ),
                              new gkg::Bitmap( lufast_mask_bits,
                                                  lufast_mask_width,
                                                  lufast_mask_height ) );

  }
  return _luFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getUFastCursor()
{

  if ( !_uFastCursor )
  {

    _uFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( ufast_bits,
                                                  ufast_width,
                                                  ufast_height,
                                                  ufast_x_hot,
                                                  ufast_y_hot ),
                              new gkg::Bitmap( ufast_mask_bits,
                                                  ufast_mask_width,
                                                  ufast_mask_height ) );

  }
  return _uFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getRUFastCursor()
{

  if ( !_ruFastCursor )
  {

    _ruFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( rufast_bits,
                                                  rufast_width,
                                                  rufast_height,
                                                  rufast_x_hot,
                                                  rufast_y_hot ),
                              new gkg::Bitmap( rufast_mask_bits,
                                                  rufast_mask_width,
                                                  rufast_mask_height ) );

  }
  return _ruFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getRFastCursor()
{

  if ( !_rFastCursor )
  {

    _rFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( rfast_bits,
                                                  rfast_width,
                                                  rfast_height,
                                                  rfast_x_hot,
                                                  rfast_y_hot ),
                              new gkg::Bitmap( rfast_mask_bits,
                                                  rfast_mask_width,
                                                  rfast_mask_height ) );

  }
  return _rFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getRDFastCursor()
{

  if ( !_rdFastCursor )
  {

    _rdFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( rdfast_bits,
                                                  rdfast_width,
                                                  rdfast_height,
                                                  rdfast_x_hot,
                                                  rdfast_y_hot ),
                              new gkg::Bitmap( rdfast_mask_bits,
                                                  rdfast_mask_width,
                                                  rdfast_mask_height ) );

  }
  return _rdFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getDFastCursor()
{

  if ( !_dFastCursor )
  {

    _dFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( dfast_bits,
                                                  dfast_width,
                                                  dfast_height,
                                                  dfast_x_hot,
                                                  dfast_y_hot ),
                              new gkg::Bitmap( dfast_mask_bits,
                                                  dfast_mask_width,
                                                  dfast_mask_height ) );

  }
  return _dFastCursor;

}


gkg::Cursor* gkg::WidgetFactoryImplementation::getLDFastCursor()
{

  if ( !_ldFastCursor )
  {

    _ldFastCursor = new gkg::BitmapCursor(
                              new gkg::Bitmap( ldfast_bits,
                                                  ldfast_width,
                                                  ldfast_height,
                                                  ldfast_x_hot,
                                                  ldfast_y_hot ),
                              new gkg::Bitmap( ldfast_mask_bits,
                                                  ldfast_mask_width,
                                                  ldfast_mask_height ) );

  }
  return _ldFastCursor;

}


void gkg::WidgetFactoryImplementation::setSavedText( const std::string& text )
{

  _savedText = text;

}


const std::string& gkg::WidgetFactoryImplementation::getSavedText() const
{

  return _savedText;

}


gkg::Glyph* 
gkg::WidgetFactoryImplementation::createFancyLabel( const std::string& label )
{

  std::string value;
  if ( _style->findAttribute( "labelStyle", value ) )
  {

    if ( !_initializedLabelStyles )
    {

      _chiseledLabelStyle = "chiseled";
      _raisedLabelStyle = "raised";
      _initializedLabelStyles = true;

    }
    if ( value == _chiseledLabelStyle )
    {

      return _target->createChiseledLabel( label );

    }
    if ( value == _raisedLabelStyle )
    {

      return _target->createRaisedLabel( label );

    }

  }
  return _target->createLabel( label );

}


gkg::Glyph* 
gkg::WidgetFactoryImplementation::buildFancyLabel( gkg::Glyph* above,
                                                   gkg::Glyph* below,
                                                   gkg::Glyph* glyph ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate offset = 1.0;
  gkg::Glyph* g1;
  gkg::Glyph* g2;
  gkg::Glyph* g3;

  if ( above )
  {

    g1 = layoutFactory.createMargin( above, 0.0, offset, offset, 0.0 );
    g2 = layoutFactory.createMargin( above, 0.0, offset, 0.0, 0.0 );
    g3 = layoutFactory.createMargin( glyph, offset, 0.0, 0.0, offset );

  }
  else
  {

    g1 = layoutFactory.createMargin( below, offset, 0.0, 0.0, offset );
    g2 = layoutFactory.createMargin( below, 0.0, 0.0, 0.0, offset );
    g3 = layoutFactory.createMargin( glyph, 0.0, offset, offset, 0.0 );

  }
  return new gkg::LocalOverlay( g1, g2, g3 );

}


gkg::FloatColorIntensity 
gkg::WidgetFactoryImplementation::getLabelShading() const
{

  gkg::FloatColorIntensity shading = 0.9;
  _style->findAttribute( "labelShading", shading );
  return shading;

}


gkg::TellTaleState* 
gkg::WidgetFactoryImplementation::beginStyle( const std::string& s1,
                                              const std::string& s2,
                                              gkg::TellTaleState::Flag flag )
{

  if ( s2.empty() )
  {

    _target->beginStyle( s1 );

  }
  else
  {

    _target->beginStyle( s1, s2 );

  }
  return new gkg::TellTaleState( flag );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginMenubarItemStyle()
{

  return beginStyle( "MenuBar", "Menu", gkg::TellTaleState::Enabled );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginMenuItemStyle()
{

  return beginStyle( "MenuItem", "", gkg::TellTaleState::Enabled );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginCheckMenuItemStyle()
{

  return beginStyle( "ToggleButton", "MenuItem", ( gkg::TellTaleState::Flag )
                     ( gkg::TellTaleState::Enabled |
                       gkg::TellTaleState::Toggle ) );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginRadioMenuItemStyle(
                                                     gkg::TellTaleGroup* group )
{

  gkg::TellTaleState*
    tellTaleState = beginStyle( "RadioButton", "MenuItem",
                                ( gkg::TellTaleState::Flag )
                                ( gkg::TellTaleState::Enabled |
                                  gkg::TellTaleState::Choosable ) );
  tellTaleState->join( group );
  return tellTaleState;

}


gkg::TellTaleState* 
gkg::WidgetFactoryImplementation::beginMenuItemSeparatorStyle()
{

  return beginStyle( "MenuSeparator", "MenuItem",
                     gkg::TellTaleState::FlagNone );

}


gkg::MenuItem* gkg::WidgetFactoryImplementation::endMenuItemStyle(
                                             gkg::Glyph* glyph,
                                             gkg::TellTaleState* tellTaleState )
{

  gkg::MenuItem* menuItem = new gkg::MenuItem( glyph, tellTaleState );
  _target->endStyle();
  return menuItem;

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginPushButtonStyle()
{

  return beginStyle( "PushButton", "Button", gkg::TellTaleState::Enabled );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginDefaultButtonStyle()
{

  gkg::Style* style = new gkg::Style( "DefaultButton", _style );
  style->setAlias( "PushButton" );
  style->setAlias( "Button" );
  _target->pushStyle( style );
  return new gkg::TellTaleState( gkg::TellTaleState::Enabled );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginCheckBoxStyle()
{

  return beginStyle( "ToggleButton", "Button",
                     ( gkg::TellTaleState::Flag )
                     ( gkg::TellTaleState::Enabled |
                       gkg::TellTaleState::Toggle ) );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginPaletteButtonStyle()
{

  return beginStyle( "PaletteButton", "Button",
                     ( gkg::TellTaleState::Flag )
                     ( gkg::TellTaleState::Enabled |
                       gkg::TellTaleState::Toggle ) );

}


gkg::TellTaleState* gkg::WidgetFactoryImplementation::beginRadioButtonStyle(
                                                     gkg::TellTaleGroup* group )
{

  gkg::TellTaleState*
    tellTaleState = beginStyle(	"RadioButton", "Button",
                                ( gkg::TellTaleState::Flag )
                                ( gkg::TellTaleState::Enabled |
                                  gkg::TellTaleState::Choosable ) );
  tellTaleState->join( group );
  return tellTaleState;

}


gkg::TellTaleState* 
gkg::WidgetFactoryImplementation::beginNoMarginPushButtonStyle()
{

  return beginStyle( "NoMarginPushButton", "Button",
                     gkg::TellTaleState::Enabled );

}


gkg::Button* gkg::WidgetFactoryImplementation::endButtonStyle( 
                                              gkg::Glyph* glyph,
                                              gkg::TellTaleState* tellTaleState,
                                              gkg::Action* action )
{

  gkg::Button* button = new gkg::Button( glyph, _style, tellTaleState, action );
  _target->endStyle();
  return button;

}


void gkg::WidgetFactoryImplementation::update()
{

  if ( _styleChanged )
  {

    updateStyleInfo();

  }

}


void gkg::WidgetFactoryImplementation::updateStyleInfo()
{

  gkg::Display* 
    display = gkg::Session::getInstance().getDefaultDisplay();
  std::string value;
  if ( _style->findAttribute( "font", value ) ||
       _style->findAttribute( "Font", value ) )
  {

    const Font* font = gkg::Font::lookup( value );
    if ( !font )
    {

      if ( !_font )
      {

        std::string defaultFont = "fixed";
        reportError( "open font", value, defaultFont );
        _font = gkg::Font::lookup( defaultFont );
        if ( _font )
        {

          _font->addReference();

        }

      }

    }
    else
    {

      if ( font )
      {

        font->addReference();

      }
      if ( _font )
      {

        _font->removeReference();

      }
      _font = font;

    }

  }
  if ( _style->findAttribute( "foreground", value ) ||
       _style->findAttribute( "Foreground", value ) )
  {

    const Color* color = gkg::Color::lookup( display, value );
    if ( !color )
    {

      if ( !_foreground )
      {

        std::string defaultForeground = "#000000";
        reportError( "find color", value, defaultForeground );
        _foreground = gkg::Color::lookup( display, defaultForeground );
        if ( _foreground )
        {

          _foreground->addReference();

        }

      }

    }
    else
    {

      if ( color )
      {

        color->addReference();

      }
      if ( _foreground )
      {

        _foreground->removeReference();

      }
      _foreground = color;

    }

  }
  if ( _style->findAttribute( "background", value ) ||
       _style->findAttribute( "Background", value ) )
  {

    const Color* color = gkg::Color::lookup( display, value );
    if ( !color )
    {

      if ( !_foreground )
      {

        std::string defaultBackground = "#ffffff";
        reportError( "find color", value, defaultBackground );
        _background = gkg::Color::lookup( display, defaultBackground );
        if ( _background )
        {

          _background->addReference();

        }

      }

    }
    else
    {

      if ( color )
      {

        color->addReference();

      }
      if ( _background )
      {

        _background->removeReference();

      }
      _background = color;

    }

  }
  _styleChanged = false;

}


void
gkg::WidgetFactoryImplementation::reportError( const std::string& operation,
                                               const std::string& name,
                                               const std::string& value )
{

  std::cerr << gkg::Session::getInstance().getName()
            << ": unable to "
            << operation
            << " "
            << name
            << ", using "
            << value
            << std::endl;

}
