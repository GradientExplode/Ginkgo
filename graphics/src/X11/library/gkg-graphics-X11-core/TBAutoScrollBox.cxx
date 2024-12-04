#include <gkg-graphics-X11-core/TBAutoScrollBox.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/TBScrollBox.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#if 0

#include <iostream> // a enlever

#endif


gkg::TBAutoScrollBox::TBAutoScrollBox( bool shrinkable )
                     : gkg::Patch( 0 ),
                       _sliderOn( false )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  _tbScrollBox = static_cast< gkg::TBScrollBox* >(
                                layoutFactory.createVScrollBox( shrinkable ) );

  _hBox = layoutFactory.createHBox();
  _hBox->append( _tbScrollBox );

  setBody( _hBox );

}


gkg::TBAutoScrollBox::~TBAutoScrollBox()
{
}


void gkg::TBAutoScrollBox::request( 
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( !_sliderOn )
  {

    gkg::WidgetFactory* 
      widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

    Glyph* insetFrame = widgetFactory->createInsetFrame( _tbScrollBox );
    gkg::GeometryRequisition gr1;
    insetFrame->request( gr1 );
    delete insetFrame;

    Glyph* scrollBar = widgetFactory->createVScrollBar( _tbScrollBox );
    gkg::GeometryRequisition gr2;
    scrollBar->request( gr2 );
    delete scrollBar;

    this->gkg::Patch::request( geometryRequisition );

    geometryRequisition.getRequirementX().setNatural(
                                           gr1.getRequirementX().getNatural() +
                                           gr2.getRequirementX().getNatural() );
    geometryRequisition.getRequirementY().setNatural(
                                           gr1.getRequirementY().getNatural() );

  }
  else
  {

    this->gkg::Patch::request( geometryRequisition );

  }

}


void gkg::TBAutoScrollBox::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  gkg::GeometryRequisition geometryRequisition;
  _tbScrollBox->request( geometryRequisition );
  gkg::FloatCoordinate 
    naturalHeight = geometryRequisition.getRequirementY().getNatural();
  activateSlider( ( naturalHeight >
                    geometryAllocation.getAllotmentY().getSpan() ) );

#if 0

  std::cerr << "TBAutoScrollBox::allocate (before) : extX="
            << geometryExtension.getRight() - 
               geometryExtension.getLeft() << "  extY="
            << geometryExtension.getTop() - 
               geometryExtension.getBottom() << std::endl;
  std::cerr << "sliderOn = " << _sliderOn << std::endl;

#endif

  this->gkg::Patch::allocate( canvas, geometryAllocation, geometryExtension );

#if 0

  std::cerr << "TBAutoScrollBox::allocate : spanX="
            << geometryAllocation.getAllotmentX().getSpan() << "  spanY="
            << geometryAllocation.getAllotmentY().getSpan() << std::endl;
  std::cerr << "TBAutoScrollBox::allocate (after) : extX="
            << geometryExtension.getRight() - 
               geometryExtension.getLeft() << "  extY="
            << geometryExtension.getTop() - 
               geometryExtension.getBottom() << std::endl;

#endif

}


bool gkg::TBAutoScrollBox::isShown( gkg::Glyph::Index index ) const
{

  return _tbScrollBox->isShown( index );

}


gkg::Glyph::Index gkg::TBAutoScrollBox::getFirstShown() const
{

  return _tbScrollBox->getFirstShown();

}


gkg::Glyph::Index gkg::TBAutoScrollBox::getLastShown() const
{

  return _tbScrollBox->getLastShown();

}


void gkg::TBAutoScrollBox::append( gkg::Glyph* other )
{

  _tbScrollBox->append( other );
  if ( getGeometryAllocation().getAllotmentY().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _tbScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalHeight = geometryRequisition.getRequirementY().getNatural();
    activateSlider( 
            naturalHeight > getGeometryAllocation().getAllotmentY().getSpan() );

  }

}


void gkg::TBAutoScrollBox::prepend( gkg::Glyph* other )
{

  _tbScrollBox->prepend( other );
  if ( getGeometryAllocation().getAllotmentY().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _tbScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate 
      naturalHeight = geometryRequisition.getRequirementY().getNatural();
    activateSlider( 
            naturalHeight > getGeometryAllocation().getAllotmentY().getSpan() );

  }

}


void gkg::TBAutoScrollBox::insert( gkg::Glyph::Index index,
                                   gkg::Glyph* other )
{

  _tbScrollBox->insert( index, other );
  if ( getGeometryAllocation().getAllotmentY().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _tbScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalHeight = geometryRequisition.getRequirementY().getNatural();
    activateSlider( 
            naturalHeight > getGeometryAllocation().getAllotmentY().getSpan() );

  }

}


void gkg::TBAutoScrollBox::remove( gkg::Glyph::Index index )
{

  _tbScrollBox->remove( index );
  if ( getGeometryAllocation().getAllotmentY().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _tbScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalHeight = geometryRequisition.getRequirementY().getNatural();
    activateSlider( 
            naturalHeight > getGeometryAllocation().getAllotmentY().getSpan() );

  }

}


void gkg::TBAutoScrollBox::replace( gkg::Glyph::Index index,
                                    gkg::Glyph* other )
{

  _tbScrollBox->replace( index, other );
  if ( getGeometryAllocation().getAllotmentY().getSpan() > 0.0f )
  {

    gkg::GeometryRequisition geometryRequisition;
    _tbScrollBox->request( geometryRequisition );
    gkg::FloatCoordinate
      naturalHeight = geometryRequisition.getRequirementY().getNatural();
    activateSlider( 
            naturalHeight > getGeometryAllocation().getAllotmentY().getSpan() );

  }

}


gkg::Glyph::Index gkg::TBAutoScrollBox::getCount() const
{

  return _tbScrollBox->getCount();

}


gkg::Glyph* gkg::TBAutoScrollBox::getComponent( gkg::Glyph::Index index ) const
{

  return _tbScrollBox->getComponent( index );

}


void
gkg::TBAutoScrollBox::setSmallScroll( gkg::FloatCoordinate floatCoordinate )
{

  _tbScrollBox->setSmallScroll( gkg::DimensionY, floatCoordinate );

}


gkg::FloatCoordinate gkg::TBAutoScrollBox::getSmallScroll() const
{

  return _tbScrollBox->getSmallScroll( gkg::DimensionY );

}


void
gkg::TBAutoScrollBox::setLargeScroll( gkg::FloatCoordinate floatCoordinate )
{

  _tbScrollBox->setLargeScroll( gkg::DimensionY, floatCoordinate );

}


gkg::FloatCoordinate gkg::TBAutoScrollBox::getLargeScroll() const
{

  return _tbScrollBox->getLargeScroll( gkg::DimensionY );

}


void gkg::TBAutoScrollBox::scrollForward()
{

  _tbScrollBox->scrollForward( gkg::DimensionY );

}


void gkg::TBAutoScrollBox::scrollBackward()
{

  _tbScrollBox->scrollBackward( gkg::DimensionY );

}


void gkg::TBAutoScrollBox::pageForward()
{

  _tbScrollBox->pageForward( gkg::DimensionY );

}


void gkg::TBAutoScrollBox::pageBackward()
{

  _tbScrollBox->pageBackward( gkg::DimensionY );

}


void gkg::TBAutoScrollBox::scrollTo( gkg::FloatCoordinate lower )
{

  _tbScrollBox->scrollTo( gkg::DimensionY, lower );

}


void gkg::TBAutoScrollBox::activateSlider( bool value )
{

  _tbScrollBox->modified( 0 );
  if ( ( _sliderOn == false ) && ( value == true ) )
  {

    _sliderOn = true;

    gkg::WidgetFactory* 
      widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

    PolyGlyph* hBox = layoutFactory.createHBox();
    hBox->append( widgetFactory->createInsetFrame( _tbScrollBox ) );
    hBox->append( widgetFactory->createVScrollBar( _tbScrollBox ) );

    setBody( hBox );
    _hBox = hBox;

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
    PolyGlyph* hBox = layoutFactory.createHBox();
    hBox->append( _tbScrollBox );

    setBody( hBox );
    _hBox = hBox;

    reallocate();
    if ( getCanvas() )
    {

      redraw();

    }

  }

}
