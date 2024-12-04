#include <gkg-graphics-X11-LF-Motif/MotifWidgetFactoryImplementation.h>
#include <gkg-graphics-X11-LF-Motif/MotifWidgetFactory.h>
#include <gkg-graphics-X11-LF-Motif/MotifInfo.h>
#include <gkg-graphics-X11-LF-Motif/MotifFrame.h>
#include <gkg-graphics-X11-LF-Motif/MotifMenuItem.h>
#include <gkg-graphics-X11-LF-Motif/MotifForeground.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ChoiceItem.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Slider.h>
#include <gkg-graphics-X11-core/ChoiceItem.h>
#include <gkg-graphics-X11-core/XSlider.h>
#include <gkg-graphics-X11-core/YSlider.h>
#include <gkg-graphics-X11-core/XYSlider.h>
#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Style.h>


static const char* styleAttributes[] =
{

  "flat",
  "frameThickness",
  "moverSize",
  "sliderSize",
  0

};


gkg::MotifWidgetFactoryImplementation::MotifWidgetFactoryImplementation(
                                  gkg::MotifWidgetFactory* motifWidgetFactory )
                                      : _target( motifWidgetFactory )
{
}


gkg::MotifWidgetFactoryImplementation::~MotifWidgetFactoryImplementation()
{

  std::list< gkg::MotifInfo* >::iterator m = _motifInfos.begin(),
                                         me = _motifInfos.end();
  while ( m != me )
  {

    gkg::MotifInfo* motifInfo = *m;
    if ( motifInfo )
    {

      motifInfo->removeReference();

    }
    ++ m;

  }

}


gkg::MotifWidgetFactory* 
gkg::MotifWidgetFactoryImplementation::getTarget() const
{

  return _target;

}


void 
gkg::MotifWidgetFactoryImplementation::styleChanged( gkg::Style* style )
{

  std::list< gkg::MotifInfo* >::const_iterator m = _motifInfos.begin(),
                                               me = _motifInfos.end();
  while ( m != me )
  {

    gkg::MotifInfo* motifInfo = *m;
    if ( motifInfo->getStyle() == style )
    {

      _motifInfo = motifInfo;
      return;

    }
    ++ m;

  }

  m = _motifInfos.begin();
  while ( m != me )
  {

    gkg::MotifInfo* motifInfo = *m;
    const gkg::Style* style2 = motifInfo->getStyle();
    for ( const char** p = styleAttributes; ; p++ )
    {

      if ( *p == 0 )
      {

        _motifInfo = motifInfo;
        return;

      }
      if ( !match( *style, *style2, *p ) )
      {

        break;

      }

    }
    ++ m;

  }
  _motifInfo = new gkg::MotifInfo( style, _target->getColoringMode() );
  if ( _motifInfo )
  {

    _motifInfo->addReference();

  }
  _motifInfos.push_back( _motifInfo );

}


gkg::MonoGlyph* 
gkg::MotifWidgetFactoryImplementation::createOutsetFrame(
                                                       gkg::Glyph* glyph ) const
{

  return new gkg::Bevel( glyph,
                         &gkg::Bevel::drawRectangle,
                         _motifInfo->getLight(),
                         _motifInfo->getFlat(),
                         _motifInfo->getDark(),
                         _motifInfo->getThickness() );

}


gkg::MonoGlyph* 
gkg::MotifWidgetFactoryImplementation::createInsetFrame(
                                                       gkg::Glyph* glyph ) const
{

  return new gkg::Bevel( glyph,
                         &gkg::Bevel::drawRectangle,
                         _motifInfo->getDark(),
                         _motifInfo->getFlat(),
                         _motifInfo->getLight(),
                         _motifInfo->getThickness() );

}


gkg::MonoGlyph* 
gkg::MotifWidgetFactoryImplementation::createBrightInsetFrame(
                                                       gkg::Glyph* glyph ) const
{

  return new gkg::Bevel( glyph,
                         &gkg::Bevel::drawRectangle,
                         _motifInfo->getDark(),
                         _motifInfo->getLight(),
                         _motifInfo->getLight(),
                         1.0, 0.0, 0.0, false, false );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createMenubarLook() const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  return createOutsetFrame( layoutFactory.createRMargin(
                            layoutFactory.createHBox(), 0.0, fil, 0.0 ) );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createPulldownLook() const
{

  return createOutsetFrame( gkg::LayoutFactory::getInstance().createVBox() );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createMenubarItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::Glyph* item = layoutFactory.createHMargin( glyph, 2.0 );
  return new gkg::MotifFrame( item,
                              tellTaleState,
                              _motifInfo,
                              _motifInfo->getThickness(),
                              true,
                              true );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createMenuItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return new gkg::MotifFrame( glyph,
                              tellTaleState,
                              _motifInfo,
                              _motifInfo->getThickness(),
                              true,
                              true );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createCheckMenuItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate boxSize = _target->getFont()->getWidth( 'm' ) *
                                 _motifInfo->getToggleScale();
  gkg::Glyph* dot = new gkg::Bevel(
                            layoutFactory.createCenter(
                              layoutFactory.createFixed( 0, boxSize, boxSize ),
                              0.0,
                              0.0 ),
                            &gkg::Bevel::drawRectangle,
                            _motifInfo->getDark(),
                            _motifInfo->getDull(),
                            _motifInfo->getLight(),
                            _motifInfo->getThickness(),
                            false,
                            false );
  return makeMenuItem( tellTaleState, dot, glyph );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createRadioMenuItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate boxSize = _target->getFont()->getWidth( 'm' ) *
                                 _motifInfo->getRadioScale();
  gkg::Glyph* diamond = new gkg::Bevel(
                            layoutFactory.createCenter(
                              layoutFactory.createFixed( 0, boxSize, boxSize ),
                              0.0,
                              0.0 ),
                            &gkg::Bevel::drawDiamond,
                            _motifInfo->getDark(),
                            _motifInfo->getDull(),
                            _motifInfo->getLight(),
                            _motifInfo->getThickness(),
                            false,
                            false );
  return makeMenuItem( tellTaleState, diamond, glyph );

}


gkg::Glyph* 
gkg::MotifWidgetFactoryImplementation::createMenuItemSeparatorLook() const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  return layoutFactory.createVMargin(
                   layoutFactory.createVFixed(
                     layoutFactory.createHMargin( createInsetFrame( 0 ), 3.0 ),
                     3.0 ),
                   2.0 );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createPushButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate margin = 5.0;
  gkg::FloatCoordinate offset = 0.5 * _motifInfo->getThickness();

  gkg::Glyph* pressed = 0;
  gkg::Glyph* disabled = 0;
  if ( _target->getColoringMode() == gkg::Monochrome )
  {

    if ( !_target->getSavedText().empty() )
    {

      pressed = new gkg::Label( _target->getSavedText(),
                                _target->getFont(),
                                _target->getBackground() );
      disabled = new gkg::Label( _target->getSavedText(),
                                 _target->getFont(),
                                 _motifInfo->getLight() );
      _target->setSavedText( "" );

    }
    else
    {

      pressed = disabled = glyph;

    }

  }
  else
  {

    pressed = glyph;
    disabled = 0;

  }
  return makeButton(
               tellTaleState,
               layoutFactory.createMargin( glyph, margin ),
               layoutFactory.createMargin( pressed,
                                           margin + offset,
                                           margin - offset ),
               disabled ? layoutFactory.createMargin( disabled, margin ) : 0 );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createDefaultButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  return createInsetFrame( layoutFactory.createMargin( 
                             createPushButtonLook( glyph, tellTaleState ),
                             3.0 ) );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createCheckBoxLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  return layoutFactory.createHBox(
           layoutFactory.createVCenter(
             makeToggle( tellTaleState,
                         &gkg::Bevel::drawRectangle,
                         _motifInfo->getToggleScale() ) ),
           layoutFactory.createHSpace( 5.0 ),
           layoutFactory.createVCenter( glyph ) );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createPaletteButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return createPushButtonLook( glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createRadioButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  return layoutFactory.createHBox(
           layoutFactory.createVCenter(
             makeToggle( tellTaleState,
                          &gkg::Bevel::drawDiamond,
                          _motifInfo->getRadioScale() ) ),
           layoutFactory.createHSpace( 5.0 ),
           layoutFactory.createVCenter( glyph ) );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createNoMarginPushButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate margin = 0.0;
  gkg::FloatCoordinate offset = 0.5 * _motifInfo->getThickness();

  gkg::Glyph* pressed = 0;
  gkg::Glyph* disabled = 0;
  if ( _target->getColoringMode() == gkg::Monochrome )
  {

    if ( !_target->getSavedText().empty() )
    {

      pressed = new gkg::Label( _target->getSavedText(),
                                   _target->getFont(),
                                   _target->getBackground() );
      disabled = new gkg::Label( _target->getSavedText(),
                                    _target->getFont(),
                                    _motifInfo->getLight() );
      _target->setSavedText( "" );

    }
    else
    {

      pressed = disabled = glyph;

    }

  }
  else
  {

    pressed = glyph;
    disabled = 0;

  }
  return makeButton(
               tellTaleState,
               layoutFactory.createMargin( glyph, margin ),
               layoutFactory.createMargin( pressed,
                                           margin + offset,
                                           margin - offset ),
               disabled ? layoutFactory.createMargin( disabled, margin ) : 0 );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createSliderLook(
                                             gkg::DimensionName dimensionName,
                                             gkg::Adjustable* adjustable ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate sliderMargin = 2.0;
  gkg::FloatCoordinate size = _motifInfo->getSliderSize() - sliderMargin -
                              sliderMargin;
  gkg::Glyph* glyph;
  switch ( dimensionName )
  {

    case gkg::DimensionX:

      glyph = layoutFactory.createVFixed(
                makeSlider( new gkg::XSlider( _target->getStyle(),
                                                 adjustable ) ), size );
      break;

    case gkg::DimensionY:

      glyph = layoutFactory.createHFixed(
                makeSlider( new gkg::YSlider( _target->getStyle(),
                                                 adjustable ) ), size );
      break;

    default:

	return 0;

  }
  return _target->getColoringMode() == gkg::Monochrome ?
           glyph :
           new gkg::Bevel( glyph,
                           &gkg::Bevel::drawRectangle,
                           _motifInfo->getDark(),
                           _motifInfo->getDull(),
                           _motifInfo->getLight(),
                           _motifInfo->getThickness() );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createScrollBarLook(
                                             gkg::DimensionName dimensionName,
                                             gkg::Adjustable* adjustable ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::Glyph* glyph = 0;
  gkg::Glyph* box = 0;
  gkg::Glyph* sep = 0;
  gkg::Glyph* mover1 = 0;
  gkg::Glyph* slider = 0;
  gkg::Glyph* mover2 = 0;
  gkg::FloatCoordinate xSpan = 0, ySpan = 0;
  float align = 0;
  gkg::FloatCoordinate sliderMargin = 2.0;
  switch ( dimensionName )
  {

    case gkg::DimensionX:

      xSpan = _motifInfo->getMoverSize();
      ySpan = _motifInfo->getSliderSize();
      align = 0.0;
      box = layoutFactory.createHBox();
      glyph = layoutFactory.createVFixed( box, ySpan );
      sep = layoutFactory.createHSpace( 1.0 );
      mover1 = _target->createLeftMover( adjustable );
      slider = layoutFactory.createVMargin(
                 layoutFactory.createVFixed(
                              makeSlider(
                                new gkg::XSlider( _target->getStyle(),
                                                     adjustable ) ),
                              ySpan - sliderMargin - sliderMargin ),
                 sliderMargin );
      mover2 = _target->createRightMover( adjustable );
      break;

    case gkg::DimensionY:

      xSpan = _motifInfo->getSliderSize();
      ySpan = _motifInfo->getMoverSize();
      align = 1.0;
      box = layoutFactory.createVBox();
      glyph = layoutFactory.createHFixed( box, xSpan );
      sep = layoutFactory.createVSpace( 1.0 );
      mover1 = _target->createUpMover( adjustable );
      slider = layoutFactory.createHMargin(
                 layoutFactory.createHFixed(
                              makeSlider(
                                new gkg::YSlider( _target->getStyle(),
                                                      adjustable ) ),
                              xSpan - sliderMargin - sliderMargin ),
                 sliderMargin );
      mover2 = _target->createDownMover( adjustable );
      break;

  }
  box->append( layoutFactory.createCenterDimension(
                             layoutFactory.createFixed( mover1, xSpan, ySpan ),
                             dimensionName,
                             align ) );
  box->append( sep );
  box->append( slider );
  box->append( sep );
  box->append( layoutFactory.createFixed( mover2, xSpan, ySpan ) );
  return new gkg::Bevel( glyph,
                         &gkg::Bevel::drawRectangle,
                         _motifInfo->getDark(),
                         _motifInfo->getDull(),
                         _motifInfo->getLight(),
                         _motifInfo->getThickness() );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createPannerLook(
                                            gkg::Adjustable* adjustableX,
                                            gkg::Adjustable* adjustableY ) const
{

  return makeSlider( new gkg::XYSlider( _target->getStyle(),
                                        adjustableX, adjustableY ) );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createEnlargerLook(
                                   gkg::TellTaleState* /*tellTaleState*/ ) const
{

  return 0;

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createReducerLook(
                                   gkg::TellTaleState* /*tellTaleState*/ ) const
{

  return 0;

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createUpMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return makeMover( tellTaleState, &gkg::Bevel::drawUpArrow );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createDownMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return makeMover( tellTaleState, &gkg::Bevel::drawDownArrow );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createLeftMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return makeMover( tellTaleState, &gkg::Bevel::drawLeftArrow );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::createRightMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return makeMover( tellTaleState, &gkg::Bevel::drawRightArrow );

}


gkg::Glyph* gkg::MotifWidgetFactoryImplementation::makeMenuItem(
                                              gkg::TellTaleState* tellTaleState,
                                              gkg::Glyph* check,
                                              gkg::Glyph* glyph ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  return new gkg::MotifFrame( layoutFactory.createHBox(
                                new gkg::MotifMenuItem(
                                          layoutFactory.createVCenter( check ),
                                          tellTaleState ),
	                        layoutFactory.createHSpace( 5 ),
                              layoutFactory.createVCenter( glyph ) ),
                              tellTaleState,
                              _motifInfo,
                              _motifInfo->getThickness(),
                              true,
                              false );

}


gkg::ChoiceItem* gkg::MotifWidgetFactoryImplementation::makeButton(
                                              gkg::TellTaleState* tellTaleState,
                                              gkg::Glyph* normal,
                                              gkg::Glyph* pressed,
                                              gkg::Glyph* disabled ) const
{

  gkg::Glyph* enabled = new gkg::Bevel( normal,
                                        &gkg::Bevel::drawRectangle,
                                        _motifInfo->getLight(),
                                        _motifInfo->getFlat(),
                                        _motifInfo->getDark(),
                                        _motifInfo->getThickness(),
                                        false,
                                        false );

  gkg::Glyph* unenabled;
  if ( _target->getColoringMode() == gkg::Monochrome )
  {

    unenabled = new gkg::Bevel( disabled,
                                &gkg::Bevel::drawRectangle,
                                _motifInfo->getLight(),
                                _motifInfo->getFlat(),
                                _motifInfo->getDark(),
                                _motifInfo->getThickness(),
                                false,
                                false );

  }
  else
  {

    unenabled = new gkg::MotifForeground( enabled,
                                          _motifInfo->getGrayOut() );

  }

  gkg::Glyph* active;
  if ( _target->getColoringMode() == gkg::Monochrome )
  {

    active = new gkg::Bevel( pressed,
                             &gkg::Bevel::drawRectangle,
                             _motifInfo->getDark(),
                             _motifInfo->getDull(),
                             _motifInfo->getLight(),
                             _motifInfo->getThickness(),
                             false,
                             false );

  }
  else
  {

    active = new gkg::Bevel( pressed,
                             &gkg::Bevel::drawRectangle,
                             _motifInfo->getDark(),
                             _motifInfo->getFlat(),
                             _motifInfo->getLight(),
                             _motifInfo->getThickness(),
                             false,
                             false );

  }

  return new gkg::ChoiceItem( tellTaleState,
                              unenabled,
                              enabled,
                              enabled,
                              active,
                              enabled,
                              active,
                              active,
                              active,
                              active,
                              unenabled );

}


gkg::ChoiceItem* gkg::MotifWidgetFactoryImplementation::makeToggle(
                                              gkg::TellTaleState* tellTaleState,
                                              gkg::Bevel::Beveler beveler,
                                              float scale ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate boxSize = _target->getFont()->getWidth( 'm' ) * scale;
  gkg::Glyph* box = layoutFactory.createFixed( 0, boxSize, boxSize );
  gkg::Glyph* toggleOut = new gkg::Bevel( box,
                                           beveler,
                                           _motifInfo->getLight(),
                                           _motifInfo->getFlat(),
                                           _motifInfo->getDark(),
                                           _motifInfo->getThickness(),
                                           false,
                                           false );
  gkg::Glyph* toggleIn = new gkg::Bevel( box,
                                         beveler,
                                         _motifInfo->getDark(),
                                         _motifInfo->getDull(),
                                         _motifInfo->getLight(),
                                         _motifInfo->getThickness(),
                                         false,
                                         false );
  return new gkg::ChoiceItem( tellTaleState,
                              new gkg::MotifForeground(
                                                     toggleOut,
                                                     _motifInfo->getGrayOut() ),
                              toggleOut,
                              toggleOut,
                              toggleIn,
                              toggleIn,
                              toggleIn,
                              toggleIn,
                              toggleIn,
                              toggleIn,
                              new gkg::MotifForeground(
                                                   toggleIn,
                                                   _motifInfo->getGrayOut() ) );

}


gkg::ChoiceItem* gkg::MotifWidgetFactoryImplementation::makeMover(
                                             gkg::TellTaleState* tellTaleState,
                                             gkg::Bevel::Beveler beveler ) const
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::FloatCoordinate m = 2.0;
  gkg::Glyph* enabled = layoutFactory.createMargin(
                          new gkg::Bevel( 0,
                                          beveler,
                                          _motifInfo->getLight(),
                                          _motifInfo->getFlat(),
                                          _motifInfo->getDark(),
                                          _motifInfo->getThickness()),
                          m );
  gkg::Glyph* active = layoutFactory.createMargin(
                         new gkg::Bevel( 0,
                                         beveler,
                                         _motifInfo->getDark(),
                                         _motifInfo->getFlat(),
                                         _motifInfo->getLight(),
                                         _motifInfo->getThickness() ),
                         m );
  gkg::Glyph* disabled = new gkg::MotifForeground( enabled,
                                                   _motifInfo->getGrayOut() );
  return new gkg::ChoiceItem( tellTaleState,
                              disabled,
                              enabled,
                              enabled,
                              active,
                              active,
                              enabled,
                              active,
                              active,
                              active,
                              disabled );

}


gkg::Slider* gkg::MotifWidgetFactoryImplementation::makeSlider(
                                                     gkg::Slider* slider ) const
{

  slider->setNormalThumb( new gkg::Bevel( 0,
                                          &gkg::Bevel::drawRectangle,
                                          _motifInfo->getLight(),
                                          _motifInfo->getFlat(),
                                          _motifInfo->getDark(),
                                          _motifInfo->getThickness() ) );
  return slider;

}


bool gkg::MotifWidgetFactoryImplementation::match( const gkg::Style& s1,
                                                   const gkg::Style& s2,
                                                   const std::string& name )
{

  std::string value1, value2;
  bool b1 = s1.findAttribute( name, value1 );
  bool b2 = s2.findAttribute( name, value2 );
  return !( b1 || b2 ) || ( b1 && b2 && ( value1 == value2 ) );

}
