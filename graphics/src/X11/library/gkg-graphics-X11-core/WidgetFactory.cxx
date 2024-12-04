#include <gkg-graphics-X11-core/WidgetFactory.h>
#include <gkg-graphics-X11-core/WidgetFactoryImplementation.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::WidgetFactory::WidgetFactory()
{

  _widgetFactoryImplementation = new gkg::WidgetFactoryImplementation( this );

}


gkg::WidgetFactory::~WidgetFactory()
{

  delete _widgetFactoryImplementation;

}


std::string gkg::WidgetFactory::getName() const
{

  return "unknown";

}


void gkg::WidgetFactory::setColoringMode( gkg::ColoringMode coloringMode )
{

  _widgetFactoryImplementation->setColoringMode( coloringMode );

}


gkg::ColoringMode gkg::WidgetFactory::getColoringMode() const
{

  return _widgetFactoryImplementation->getColoringMode();

}


void gkg::WidgetFactory::setSavedText( const std::string& text )
{

  _widgetFactoryImplementation->setSavedText( text );

}


const std::string& gkg::WidgetFactory::getSavedText() const
{

  return _widgetFactoryImplementation->getSavedText();

}


void gkg::WidgetFactory::setStyle( gkg::Style* style )
{

  _widgetFactoryImplementation->setStyle( style );
  styleChanged( style );

}


gkg::Style* gkg::WidgetFactory::getStyle() const
{

  return _widgetFactoryImplementation->getStyle();

}


const gkg::Font* gkg::WidgetFactory::getFont() const
{

  return _widgetFactoryImplementation->getFont();

}


const gkg::Color* gkg::WidgetFactory::getForeground() const
{

  return _widgetFactoryImplementation->getForeground();

}


const gkg::Color* gkg::WidgetFactory::getBackground() const
{

  return _widgetFactoryImplementation->getBackground();

}


void gkg::WidgetFactory::beginStyle( const std::string& name )
{

  pushStyle( new gkg::Style( name, getStyle() ) );

}


void gkg::WidgetFactory::beginStyle( const std::string& name,
                                        const std::string& alias )
{

  gkg::Style* style = new gkg::Style( name, getStyle() );
  style->setAlias( alias );
  pushStyle( style );

}


void gkg::WidgetFactory::setAlias( const std::string& alias )
{

  _widgetFactoryImplementation->setAlias( alias );

}


void gkg::WidgetFactory::endStyle()
{

  popStyle();

}


void gkg::WidgetFactory::pushStyle()
{

  _widgetFactoryImplementation->pushStyle();

}


void gkg::WidgetFactory::pushStyle( gkg::Style* style )
{

  pushStyle();
  setStyle( style );

}


void gkg::WidgetFactory::popStyle()
{

  _widgetFactoryImplementation->popStyle();

}


void gkg::WidgetFactory::styleChanged( gkg::Style* )
{
}


gkg::Cursor* gkg::WidgetFactory::getHandCursor() const
{

  return _widgetFactoryImplementation->getHandCursor();

}


gkg::Cursor* gkg::WidgetFactory::getLFastCursor() const
{

  return _widgetFactoryImplementation->getLFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getLUFastCursor() const
{

  return _widgetFactoryImplementation->getLUFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getUFastCursor() const
{

  return _widgetFactoryImplementation->getUFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getRUFastCursor() const
{

  return _widgetFactoryImplementation->getRUFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getRFastCursor() const
{

  return _widgetFactoryImplementation->getRFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getRDFastCursor() const
{

  return _widgetFactoryImplementation->getRDFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getDFastCursor() const
{

  return _widgetFactoryImplementation->getDFastCursor();

}


gkg::Cursor* gkg::WidgetFactory::getLDFastCursor() const
{

  return _widgetFactoryImplementation->getLDFastCursor();

}


gkg::Glyph* gkg::WidgetFactory::createLabel( const std::string& label ) const
{

  if ( getColoringMode() == gkg::Monochrome )
  {

    _widgetFactoryImplementation->setSavedText( label );

  }
  return new gkg::Label( label, getFont(), getForeground() );

}


gkg::Glyph*
gkg::WidgetFactory::createChiseledLabel( const std::string& label ) const
{

  const gkg::Font* font = getFont();
  gkg::FloatColorIntensity 
    shading = _widgetFactoryImplementation->getLabelShading();
  return _widgetFactoryImplementation->buildFancyLabel(
                               0,
                               new gkg::Label( label, font,
                                               new gkg::Color( shading,
                                                               shading,
                                                               shading,
                                                               1.0 ) ),
                               new gkg::Label( label, font, getForeground() ) );

}


gkg::Glyph*
gkg::WidgetFactory::createRaisedLabel( const std::string& label ) const
{

  const gkg::Font* font = getFont();
  gkg::FloatColorIntensity 
    shading = _widgetFactoryImplementation->getLabelShading();
  return _widgetFactoryImplementation->buildFancyLabel(
                               new gkg::Label( label, font,
                                               new gkg::Color( shading,
                                                               shading,
                                                               shading,
                                                               1.0 ) ),
                               0,
                               new gkg::Label( label, font, getForeground() ) );

}


gkg::Glyph*
gkg::WidgetFactory::createFancyLabel( const std::string& label ) const
{

  return _widgetFactoryImplementation->createFancyLabel( label );

}


gkg::Menu* gkg::WidgetFactory::createMenubar()
{

  beginStyle( "MenuBar", "Menu" );
  gkg::Menu* menu = new gkg::Menu( createMenubarLook(),
                                   getStyle(),
                                   0.0, 0.0, 0.0, 1.0 );
  endStyle();
  return menu;

}


gkg::Menu* gkg::WidgetFactory::createPulldown()
{

  beginStyle( "Pulldown", "Menu" );
  gkg::Menu* menu = new gkg::Menu( createPulldownLook(),
                                   getStyle(),
                                   1.0, 1.0, 0.0, 1.0 );
  endStyle();
  return menu;

}


gkg::Menu* gkg::WidgetFactory::createPullright()
{

  beginStyle( "Pullright", "Menu" );
  gkg::Menu* menu = new gkg::Menu( createPullrightLook(),
                                   getStyle(),
                                   1.0, 1.0, 0.0, 1.0 );
  endStyle();
  return menu;

}


gkg::MenuItem*
gkg::WidgetFactory::createMenubarItem( const std::string& label  ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginMenubarItemStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                   createMenubarItemLook( createLabel( label ), tellTaleState ),
                   tellTaleState );

}


gkg::MenuItem*
gkg::WidgetFactory::createMenubarItem( gkg::Glyph* glyph ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginMenubarItemStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                   createMenubarItemLook( glyph, tellTaleState ),
                   tellTaleState );

}


gkg::MenuItem*
gkg::WidgetFactory::createMenuItem( const std::string& label ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginMenuItemStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                   createMenuItemLook( createLabel( label ), tellTaleState ),
                   tellTaleState );

}


gkg::MenuItem*
gkg::WidgetFactory::createMenuItem( gkg::Glyph* glyph ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginMenuItemStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                   createMenuItemLook( glyph, tellTaleState ),
                   tellTaleState );

}


gkg::MenuItem*
gkg::WidgetFactory::createCheckMenuItem( const std::string& label ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginCheckMenuItemStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                 createCheckMenuItemLook( createLabel( label ), tellTaleState ),
                 tellTaleState );

}


gkg::MenuItem*
gkg::WidgetFactory::createCheckMenuItem( gkg::Glyph* glyph ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginCheckMenuItemStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                 createCheckMenuItemLook( glyph, tellTaleState ),
                 tellTaleState );

}


gkg::MenuItem* 
gkg::WidgetFactory::createRadioMenuItem( gkg::TellTaleGroup* group,
                                         const std::string& label ) const
{

  gkg::TellTaleState* tellTaleState = 
    _widgetFactoryImplementation->beginRadioMenuItemStyle( group );
  return _widgetFactoryImplementation->endMenuItemStyle( 
                 createRadioMenuItemLook( createLabel( label ), tellTaleState ),
                 tellTaleState );

}


gkg::MenuItem* 
gkg::WidgetFactory::createRadioMenuItem( gkg::TellTaleGroup* group,
                                         gkg::Glyph* glyph ) const
{

  gkg::TellTaleState* tellTaleState = 
    _widgetFactoryImplementation->beginRadioMenuItemStyle( group );
  return _widgetFactoryImplementation->endMenuItemStyle( 
                 createRadioMenuItemLook( glyph, tellTaleState ),
                 tellTaleState );

}


gkg::MenuItem* gkg::WidgetFactory::createMenuItemSeparator() const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginMenuItemSeparatorStyle();
  return _widgetFactoryImplementation->endMenuItemStyle( 
                            createMenuItemSeparatorLook(), tellTaleState );

}


gkg::Button* 
gkg::WidgetFactory::createPushButton( const std::string& label,
                                      gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginPushButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createPushButtonLook( createLabel( label ), tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createPushButton( gkg::Glyph* glyph,
                                      gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginPushButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createPushButtonLook( glyph, tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createDefaultButton( const std::string& label,
                                         gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginDefaultButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createDefaultButtonLook( createLabel( label ), tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createDefaultButton( gkg::Glyph* glyph,
                                         gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginDefaultButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createDefaultButtonLook( glyph, tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createCheckBox( const std::string& label,
                                    gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginCheckBoxStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createCheckBoxLook( createLabel( label ), tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createCheckBox( gkg::Glyph* glyph,
                                    gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginCheckBoxStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createCheckBoxLook( glyph, tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createPaletteButton( const std::string& label,
                                         gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginPaletteButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createPaletteButtonLook( createLabel( label ), tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createPaletteButton( gkg::Glyph* glyph,
                                         gkg::Action* action ) const
{

  gkg::TellTaleState*
    tellTaleState = _widgetFactoryImplementation->beginPaletteButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createPaletteButtonLook( glyph, tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createRadioButton( gkg::TellTaleGroup* group,
                                       const std::string& label,
                                       gkg::Action* action ) const
{

  gkg::TellTaleState*
   tellTaleState = _widgetFactoryImplementation->beginRadioButtonStyle( group );
  return _widgetFactoryImplementation->endButtonStyle( 
                 createRadioButtonLook( createLabel( label ), tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createRadioButton( gkg::TellTaleGroup* group,
                                       gkg::Glyph* glyph,
                                       gkg::Action* action ) const
{

  gkg::TellTaleState*
   tellTaleState = _widgetFactoryImplementation->beginRadioButtonStyle( group );
  return _widgetFactoryImplementation->endButtonStyle( 
                 createRadioButtonLook( glyph, tellTaleState ),
                 tellTaleState, action );

}


gkg::Button* 
gkg::WidgetFactory::createNoMarginPushButton( gkg::Glyph* glyph,
                                              gkg::Action* action ) const
{

  gkg::TellTaleState*
   tellTaleState = _widgetFactoryImplementation->beginNoMarginPushButtonStyle();
  return _widgetFactoryImplementation->endButtonStyle( 
                 createNoMarginPushButtonLook( glyph, tellTaleState ),
                 tellTaleState, action );

}


gkg::ComboBox* gkg::WidgetFactory::createComboBox() const
{

  return new gkg::ComboBox();

}


gkg::SpinBox* gkg::WidgetFactory::createSpinBox() const
{

  return new gkg::SpinBox();

}


gkg::Action* gkg::WidgetFactory::createQuitAction() const
{

  return new gkg::ActionCallback< gkg::Session >( &gkg::Session::getInstance(),
                                                  &gkg::Session::quit );

}


gkg::Glyph* gkg::WidgetFactory::createHSlider( gkg::Adjustable* adjustable )
{

  beginStyle( "HSlider", "Slider" );
  Glyph* glyph = createSliderLook( gkg::DimensionX, adjustable );
  endStyle();
  return glyph;

}


gkg::Glyph* gkg::WidgetFactory::createHScrollBar( gkg::Adjustable* adjustable )
{

  beginStyle( "HScrollBar", "ScrollBar" );
  Glyph* glyph = createScrollBarLook( gkg::DimensionX, adjustable );
  endStyle();
  return glyph;

}


gkg::Glyph* gkg::WidgetFactory::createVSlider( gkg::Adjustable* adjustable )
{

  beginStyle( "VSlider", "Slider" );
  Glyph* glyph = createSliderLook( gkg::DimensionY, adjustable );
  endStyle();
  return glyph;

}


gkg::Glyph* gkg::WidgetFactory::createVScrollBar( gkg::Adjustable* adjustable )
{

  beginStyle( "VScrollBar", "ScrollBar" );
  Glyph* glyph = createScrollBarLook( gkg::DimensionY, adjustable );
  endStyle();
  return glyph;

}


gkg::Glyph* gkg::WidgetFactory::createPanner( gkg::Adjustable* adjustableX,
                                              gkg::Adjustable* adjustableY )
{

  beginStyle( "Panner" );
  Glyph* glyph = createPannerLook( adjustableX, adjustableY );
  endStyle();
  return glyph;

}


gkg::Stepper* gkg::WidgetFactory::createEnlarger( gkg::Adjustable* )
{

  // unimplemented
  return 0;

}


gkg::Stepper* gkg::WidgetFactory::createReducer( gkg::Adjustable* )
{

  // unimplemented
  return 0;

}


gkg::Stepper* gkg::WidgetFactory::createUpMover( gkg::Adjustable* adjustable )
{

  gkg::TellTaleState* 
    tellTaleState = _widgetFactoryImplementation->beginStyle(
                    "UpMover", "Button", gkg::TellTaleState::Enabled );
  gkg::Stepper*
  stepper = new gkg::ForwardScroller( createUpMoverLook( tellTaleState ),
                                      getStyle(),
                                      tellTaleState,
                                      adjustable,
                                      gkg::DimensionY );
  endStyle();
  return stepper;

}


gkg::Stepper* gkg::WidgetFactory::createDownMover( gkg::Adjustable* adjustable )
{

  gkg::TellTaleState* 
    tellTaleState = _widgetFactoryImplementation->beginStyle(
                    "DownMover", "Button", gkg::TellTaleState::Enabled );
  gkg::Stepper*
  stepper = new gkg::BackwardScroller( createDownMoverLook( tellTaleState ),
                                       getStyle(),
                                       tellTaleState,
                                       adjustable,
                                       gkg::DimensionY );
  endStyle();
  return stepper;

}


gkg::Stepper* gkg::WidgetFactory::createLeftMover( gkg::Adjustable* adjustable )
{

  gkg::TellTaleState* 
    tellTaleState = _widgetFactoryImplementation->beginStyle(
                    "LeftMover", "Button", gkg::TellTaleState::Enabled );
  gkg::Stepper*
  stepper = new gkg::BackwardScroller( createLeftMoverLook( tellTaleState ),
                                       getStyle(),
                                       tellTaleState,
                                       adjustable,
                                       gkg::DimensionX );
  endStyle();
  return stepper;

}


gkg::Stepper* 
gkg::WidgetFactory::createRightMover( gkg::Adjustable* adjustable )
{

  gkg::TellTaleState* 
    tellTaleState = _widgetFactoryImplementation->beginStyle(
                    "RightMover", "Button", gkg::TellTaleState::Enabled );
  gkg::Stepper*
  stepper = new gkg::ForwardScroller( createRightMoverLook( tellTaleState ),
                                      getStyle(),
                                      tellTaleState,
                                      adjustable,
                                      gkg::DimensionX );
  endStyle();
  return stepper;

}


gkg::Glyph* gkg::WidgetFactory::createPullrightLook() const
{

  return createPulldownLook();

}


gkg::WidgetFactoryImplementation* gkg::WidgetFactory::getImplementation() const
{

  return _widgetFactoryImplementation;

}
