// transform setter: zooming a drawing area

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/TransformSetter.h>
#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


class ZoomArea : public gkg::MonoGlyph, public gkg::Adjustable
{

  public:

    ZoomArea( gkg::Glyph* glyph );
    ~ZoomArea();

    void updateArea( gkg::Patch* patch );
    void draw( gkg::Canvas*,
               const gkg::GeometryAllocation& geometryAllocation ) const;

    gkg::FloatCoordinate getLower( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getUpper( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getLength( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getCurrentLower( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getCurrentUpper( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getCurrentLength( gkg::DimensionName d ) const;

    void scrollTo( gkg::DimensionName, gkg::FloatCoordinate lower );

  private:

    void setXForm();

    gkg::FloatCoordinate _scale;
    gkg::TransformSetter* _xform;
    gkg::Patch* _area;
    gkg::Patch* _body;

};


ZoomArea::ZoomArea( gkg::Glyph* glyph )
         : gkg::MonoGlyph( 0 ),
           gkg::Adjustable(),
           _scale( 1.0 ),
           _xform( new gkg::TransformSetter( glyph ) )
{

  setXForm();
  _area = 0;
  _body = new gkg::Patch( _xform );
  setBody( _body );

}


ZoomArea::~ZoomArea()
{
}


void ZoomArea::updateArea( gkg::Patch* patch )
{

  _area = patch;

}


void ZoomArea::draw( gkg::Canvas* canvas,
                     const gkg::GeometryAllocation& geometryAllocation) const
{

  canvas->pushClipping();
  canvas->clipRectangle( geometryAllocation.getLeft(),
                         geometryAllocation.getBottom(),
                         geometryAllocation.getRight(),
                         geometryAllocation.getTop() );
  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );
  canvas->popClipping();

}


gkg::FloatCoordinate ZoomArea::getLower( gkg::DimensionName ) const
{

  return 0.0;

}


gkg::FloatCoordinate ZoomArea::getUpper( gkg::DimensionName ) const 
{

   return 2.0;

}


gkg::FloatCoordinate ZoomArea::getLength( gkg::DimensionName d ) const
{

  return getUpper( d );

}


gkg::FloatCoordinate ZoomArea::getCurrentLower( gkg::DimensionName ) const
{

  return _scale;

}


gkg::FloatCoordinate ZoomArea::getCurrentUpper( gkg::DimensionName ) const
{

  return _scale;

}


gkg::FloatCoordinate ZoomArea::getCurrentLength( gkg::DimensionName ) const 
{

  return 0;

}


void ZoomArea::scrollTo( gkg::DimensionName d, gkg::FloatCoordinate lower )
{

  gkg::FloatCoordinate p = lower;
  getConstrain( d, p );
  if ( p != _scale )
  {

    _scale = p;
    notify( gkg::DimensionX );
    notify( gkg::DimensionY );
    setXForm();
    _body->redraw();
    _area->reallocate();
    _body->redraw();

  }

}


void ZoomArea::setXForm()
{

  gkg::Transformer transformer;
  transformer.scale( _scale, _scale );
  transformer.rotate( 360 * _scale );
  _xform->setTransformer( transformer );

}


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestTransformSetter01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();


  gkg::Glyph* center = layoutFactory.createCenter(
                                       layoutFactory.createVBox(
                                         widgetFactory->createPushButton(
                                           "Hi mom!", 0 ),
                                         widgetFactory->createPushButton(
                                           "Look no hands!", 0 )
                                                                 ) );

  ZoomArea* zoomArea = new ZoomArea( center );

  gkg::Glyph* vCenter = layoutFactory.createVCenter(
                                       layoutFactory.createFixed( zoomArea,
                                                                  4 * 72.0,
                                                                  3 * 72.0 ),
                                       1.0 );

  gkg::Patch* patch = new gkg::Patch( vCenter );

  zoomArea->updateArea( patch );
  zoomArea->setSmallScroll( gkg::DimensionY, 0.02 );
  zoomArea->setLargeScroll( gkg::DimensionY, 0.10 );

  gkg::Glyph* vSlider = widgetFactory->createVSlider( zoomArea );
  gkg::Glyph* vCenterBis = layoutFactory.createVCenter( vSlider, 1.0 );
  gkg::Glyph* hBox = layoutFactory.createHBox( patch, vCenterBis );
  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( hBox ); 

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
