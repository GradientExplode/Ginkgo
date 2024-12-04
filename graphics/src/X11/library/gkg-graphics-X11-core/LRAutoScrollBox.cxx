#include <gkg-graphics-X11-core/LRAutoScrollBox.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/LRScrollBox.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


gkg::LRAutoScrollBox::LRAutoScrollBox( bool shrinkable )
                     : gkg::Patch( 0 ),
                       _sliderOn( false )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  _lrScrollBox = static_cast< gkg::LRScrollBox* >(
                                layoutFactory.createHScrollBox( shrinkable ) );

  _vBox = layoutFactory.createVBox();
  _vBox->append( _lrScrollBox );

  setBody( _vBox );

}


gkg::LRAutoScrollBox::~LRAutoScrollBox()
{
}


void 
gkg::LRAutoScrollBox::request( 
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( !_sliderOn )
  {

    gkg::WidgetFactory* 
      widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

    Glyph* insetFrame = widgetFactory->createInsetFrame( _lrScrollBox );
    gkg::GeometryRequisition gr1;
    insetFrame->request( gr1 );
    delete insetFrame;

    Glyph* scrollBar = widgetFactory->createHScrollBar( _lrScrollBox );
    gkg::GeometryRequisition gr2;
    scrollBar->request( gr2 );
    delete scrollBar;


    this->gkg::Patch::request( geometryRequisition );

    geometryRequisition.getRequirementX().setNatural(
                            gr1.getRequirementX().getNatural() );
    geometryRequisition.getRequirementY().setNatural(
                            gr1.getRequirementY().getNatural() + 
                            gr2.getRequirementY().getNatural() );

  }
  else
  {

    this->gkg::Patch::request( geometryRequisition );

  }

}

void gkg::LRAutoScrollBox::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  gkg::GeometryRequisition geometryRequisition;
  _lrScrollBox->request( geometryRequisition );
  gkg::FloatCoordinate
    naturalWidth = geometryRequisition.getRequirementX().getNatural();
  activateSlider( ( naturalWidth >
                    geometryAllocation.getAllotmentX().getSpan() ) );

  this->gkg::Patch::allocate( canvas, geometryAllocation, geometryExtension );

}


bool gkg::LRAutoScrollBox::isShown( gkg::Glyph::Index index ) const
{

  return _lrScrollBox->isShown( index );

}


gkg::Glyph::Index gkg::LRAutoScrollBox::getFirstShown() const
{

  return _lrScrollBox->getFirstShown();

}


gkg::Glyph::Index gkg::LRAutoScrollBox::getLastShown() const
{

  return _lrScrollBox->getLastShown();

}


void gkg::LRAutoScrollBox::append( gkg::Glyph* other )
{

  _lrScrollBox->append( other );
  if ( getGeometryAllocation().getAllotmentX().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _lrScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalWidth = geometryRequisition.getRequirementX().getNatural();
    activateSlider( 
      naturalWidth > getGeometryAllocation().getAllotmentX().getSpan() ) ;

  }

}


void gkg::LRAutoScrollBox::prepend( gkg::Glyph* other )
{

  _lrScrollBox->prepend( other );
  if ( getGeometryAllocation().getAllotmentX().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _lrScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalWidth = geometryRequisition.getRequirementX().getNatural();
    activateSlider( 
      naturalWidth > getGeometryAllocation().getAllotmentX().getSpan() ) ;

  }

}


void gkg::LRAutoScrollBox::insert( gkg::Glyph::Index index,
                                      gkg::Glyph* other )
{

  _lrScrollBox->insert( index, other );
  if ( getGeometryAllocation().getAllotmentX().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _lrScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalWidth = geometryRequisition.getRequirementX().getNatural();
    activateSlider( 
      naturalWidth > getGeometryAllocation().getAllotmentX().getSpan() ) ;

  }

}


void gkg::LRAutoScrollBox::remove( gkg::Glyph::Index index )
{

  _lrScrollBox->remove( index );
  if ( getGeometryAllocation().getAllotmentX().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _lrScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalWidth = geometryRequisition.getRequirementX().getNatural();
    activateSlider( 
      naturalWidth > getGeometryAllocation().getAllotmentX().getSpan() ) ;

  }

}


void gkg::LRAutoScrollBox::replace( gkg::Glyph::Index index,
                                       gkg::Glyph* other )
{

  _lrScrollBox->replace( index, other );
  if ( getGeometryAllocation().getAllotmentX().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _lrScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalWidth = geometryRequisition.getRequirementX().getNatural();
    activateSlider( 
      naturalWidth > getGeometryAllocation().getAllotmentX().getSpan() ) ;

  }

}


gkg::Glyph::Index gkg::LRAutoScrollBox::getCount() const
{

  return _lrScrollBox->getCount();

}


gkg::Glyph* 
gkg::LRAutoScrollBox::getComponent( gkg::Glyph::Index index ) const
{

  return _lrScrollBox->getComponent( index );

}


void 
gkg::LRAutoScrollBox::setSmallScroll( gkg::FloatCoordinate floatCoordinate )
{

  _lrScrollBox->setSmallScroll( gkg::DimensionX, floatCoordinate );

}


gkg::FloatCoordinate gkg::LRAutoScrollBox::getSmallScroll() const
{

  return _lrScrollBox->getSmallScroll( gkg::DimensionX );

}


void 
gkg::LRAutoScrollBox::setLargeScroll( gkg::FloatCoordinate floatCoordinate )
{

  _lrScrollBox->setLargeScroll( gkg::DimensionX, floatCoordinate );

}


gkg::FloatCoordinate gkg::LRAutoScrollBox::getLargeScroll() const
{

  return _lrScrollBox->getLargeScroll( gkg::DimensionX );

}


void gkg::LRAutoScrollBox::scrollForward()
{

  _lrScrollBox->scrollForward( gkg::DimensionX );

}


void gkg::LRAutoScrollBox::scrollBackward()
{

  _lrScrollBox->scrollBackward( gkg::DimensionX );

}


void gkg::LRAutoScrollBox::pageForward()
{

  _lrScrollBox->pageForward( gkg::DimensionX );

}


void gkg::LRAutoScrollBox::pageBackward()
{

  _lrScrollBox->pageBackward( gkg::DimensionX );

}


void gkg::LRAutoScrollBox::scrollTo( gkg::FloatCoordinate lower )
{

  _lrScrollBox->scrollTo( gkg::DimensionX, lower );

}


void gkg::LRAutoScrollBox::activateSlider( bool value )
{

  _lrScrollBox->modified( 0 );
  if ( ( _sliderOn == false ) && ( value == true ) )
  {

    _sliderOn = true;

    gkg::WidgetFactory* 
      widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

    PolyGlyph* vBox = layoutFactory.createVBox();
    vBox->append( widgetFactory->createInsetFrame( _lrScrollBox ) );
    vBox->append( widgetFactory->createHScrollBar( _lrScrollBox ) );


    setBody( vBox );
    _vBox = vBox;

    reallocate();
    if ( getCanvas() )
    {

      redraw();

    }

  }

  if ( ( _sliderOn == true ) && ( value == false ) )
  {

    _sliderOn = false;

    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
    PolyGlyph* vBox = layoutFactory.createVBox();
    vBox->append( _lrScrollBox );


    setBody( vBox );
    _vBox = vBox;

    reallocate();
    if ( getCanvas() )
    {

      redraw();

    }

  }

}
