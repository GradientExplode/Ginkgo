#include <list>
#include <string>
#include <gkg-graphics-X11-LF-Motif/MotifWidgetFactory.h>
#include <gkg-graphics-X11-LF-Motif/MotifWidgetFactoryImplementation.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>


static gkg::PropertyData motifDefaultMonochromeProperties[] =
{

  { "*frameThickness", "2.0" },
  { "*moverSize", "20.0" },
  { "*radioScale", "0.7" },
  { "*sliderSize", "20.0" },
  { "*toggleScale", "0.5" },
  { "*FileChooser*filter", "on" },
  { "*FieldEditor*beveled", "on" },
  { "*FieldEditor*background", "#e0e0e0" },
  { "*Panner*minimumThumbSize", "18.0" },
  { "*ScrollBar*minimumThumbSize", "28.0" },
  { "*double_buffered", "on" },
  { 0, 0 }

};


static gkg::PropertyData motifDefaultPolychromeProperties[] =
{

  { "*flat", "#aaaaaa" },
  { "*frameThickness", "2.0" },
  { "*moverSize", "20.0" },
  { "*radioScale", "0.7" },
  { "*sliderSize", "20.0" },
  { "*toggleScale", "0.5" },
  { "*FileChooser*filter", "off" },
  { "*FieldEditor*beveled", "on" },
  { "*FieldEditor*background", "#e0e0e0" },
  { "*FieldEditor*flat", "#e0e0e0" },
  { "*Panner*minimumThumbSize", "18.0" },
  { "*ScrollBar*minimumThumbSize", "28.0" },
  { 0, 0 }

};


gkg::MotifWidgetFactory::MotifWidgetFactory()
                        : gkg::WidgetFactory()
{

  int32_t priority;
  gkg::PropertyData* defaultProperties = 0;
  if ( getColoringMode() == gkg::Monochrome )
  {

    defaultProperties = motifDefaultMonochromeProperties;
    priority = -5;

  }
  else
  {

    defaultProperties = motifDefaultPolychromeProperties;
    priority = -10;

  }

  _motifWidgetFactoryImplementation = 
                              new gkg::MotifWidgetFactoryImplementation( this );
  gkg::Style* style = gkg::Session::getInstance().getStyle();
  for ( gkg::PropertyData* p = defaultProperties; p->path; p++ )
  {

    style->addAttribute( p->path, p->value, priority );

  }

}


gkg::MotifWidgetFactory::~MotifWidgetFactory()
{

  delete _motifWidgetFactoryImplementation;

}


std::string gkg::MotifWidgetFactory::getName() const
{

  return getStaticName();

}


void gkg::MotifWidgetFactory::styleChanged( gkg::Style* style )
{

  _motifWidgetFactoryImplementation->styleChanged( style );

}


gkg::MonoGlyph* 
gkg::MotifWidgetFactory::createOutsetFrame( gkg::Glyph* glyph ) const
{

  return _motifWidgetFactoryImplementation->createOutsetFrame( glyph );

}


gkg::MonoGlyph* 
gkg::MotifWidgetFactory::createInsetFrame( gkg::Glyph* glyph ) const
{

  return _motifWidgetFactoryImplementation->createInsetFrame( glyph );

}


gkg::MonoGlyph* 
gkg::MotifWidgetFactory::createBrightInsetFrame( gkg::Glyph* glyph ) const
{

  return _motifWidgetFactoryImplementation->createBrightInsetFrame( glyph );

}


gkg::Glyph* gkg::MotifWidgetFactory::createMenubarLook() const
{

  return _motifWidgetFactoryImplementation->createMenubarLook();

}


gkg::Glyph* gkg::MotifWidgetFactory::createPulldownLook() const
{

  return _motifWidgetFactoryImplementation->createPulldownLook();

}


gkg::Glyph* gkg::MotifWidgetFactory::createMenubarItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createMenubarItemLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createMenuItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createMenuItemLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createCheckMenuItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createCheckMenuItemLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createRadioMenuItemLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createRadioMenuItemLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createMenuItemSeparatorLook() const
{

  return _motifWidgetFactoryImplementation->createMenuItemSeparatorLook();

}


gkg::Glyph* gkg::MotifWidgetFactory::createPushButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createPushButtonLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createDefaultButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createDefaultButtonLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createCheckBoxLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createCheckBoxLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createPaletteButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createPaletteButtonLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createRadioButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createRadioButtonLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createNoMarginPushButtonLook(
                                       gkg::Glyph* glyph,
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createNoMarginPushButtonLook(
                                                         glyph, tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createSliderLook(
                                             gkg::DimensionName dimensionName,
                                             gkg::Adjustable* adjustable ) const
{

  return _motifWidgetFactoryImplementation->createSliderLook(
                                                    dimensionName, adjustable );

}


gkg::Glyph* gkg::MotifWidgetFactory::createScrollBarLook(
                                             gkg::DimensionName dimensionName,
                                             gkg::Adjustable* adjustable ) const
{

  return _motifWidgetFactoryImplementation->createScrollBarLook(
                                                    dimensionName, adjustable );

}


gkg::Glyph* gkg::MotifWidgetFactory::createPannerLook(
                                            gkg::Adjustable* adjustableX,
                                            gkg::Adjustable* adjustableY ) const
{

  return _motifWidgetFactoryImplementation->createPannerLook(
                                                     adjustableX, adjustableY );

}


gkg::Glyph* gkg::MotifWidgetFactory::createEnlargerLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createEnlargerLook( tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createReducerLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createReducerLook( tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createUpMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createUpMoverLook( tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createDownMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createDownMoverLook(
                                                                tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createLeftMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createLeftMoverLook(
                                                                tellTaleState );

}


gkg::Glyph* gkg::MotifWidgetFactory::createRightMoverLook(
                                       gkg::TellTaleState* tellTaleState ) const
{

  return _motifWidgetFactoryImplementation->createRightMoverLook(
                                                                tellTaleState );

}


std::string gkg::MotifWidgetFactory::getStaticName()
{

  return "Motif";

}

