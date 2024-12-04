#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SelectionStrategy.h>


gkg::VoiStrategy::VoiStrategy() 
                : correctH( 1.0f ), 
                  correctV( 1.0f ), 
                  zoomFact( 1.0f ),
                  mouseDown( false ),
                  zoomDown( false ),
                  deplaDown( false ),
                  eraseDown( false ),
                  t( 0 ),
                  da( 0 )
{

  modeStrategy = new gkg::VoiSelectionStrategy();

}


gkg::VoiStrategy::~VoiStrategy()
{
}


void gkg::VoiStrategy::setDrawA( gkg::VoiDrawA *d )
{ 

  da = d;
  
}


void gkg::VoiStrategy::paint( QPainter*, const QRectF& )
{
}


void gkg::VoiStrategy::mousePressEvent( gkg::VoiData&, QMouseEvent* )
{
}


void gkg::VoiStrategy::mouseReleaseEvent( gkg::VoiData&, QMouseEvent* )
{
}


void gkg::VoiStrategy::mouseMoveEvent( gkg::VoiData&, QMouseEvent* )
{
}


void gkg::VoiStrategy::wheelEvent( gkg::VoiData&, QWheelEvent* )
{
}


void gkg::VoiStrategy::keyPressEvent( VoiData&, QKeyEvent* event )
{

  event->accept();

}


void gkg::VoiStrategy::updateDataLoaded( gkg::VoiData& )
{
}


void gkg::VoiStrategy::updateLabelLoaded( gkg::VoiData& )
{
}


void gkg::VoiStrategy::updateData( gkg::VoiData& )
{
}


void gkg::VoiStrategy::updatePalette( gkg::VoiData& )
{
}


void gkg::VoiStrategy::updateLabel( gkg::VoiData&, 
                                    std::list< gkg::Vector3d< int32_t > >& )
{
}


void gkg::VoiStrategy::updateZoom()
{
}


void gkg::VoiStrategy::updateSlice( gkg::VoiData&, 
                                    gkg::Vector3d< int32_t >&, int32_t )
{
}


void gkg::VoiStrategy::updateSlice( gkg::VoiData&, int32_t )
{
}


void gkg::VoiStrategy::updateFrame( gkg::VoiData&, int32_t )
{
}


void gkg::VoiStrategy::deltaSlice( gkg::VoiData&, int32_t )
{
}


void gkg::VoiStrategy::deltaFrame( gkg::VoiData&, int32_t )
{
}


void gkg::VoiStrategy::centerObjects()
{
}


void gkg::VoiStrategy::updateMode( gkg::VoiModeStrategy* strgy )
{

  if ( strgy->id() != modeStrategy->id() )
  {

    modeStrategy = strgy;

  }

}


void gkg::VoiStrategy::leave()
{
}


gkg::Vector3d< int32_t > gkg::VoiStrategy::click2Point4D( QPoint&, int32_t* t )
{

  if ( t )
  {
  
    *t = 0;
    
  }

  return gkg::Vector3d< int32_t >( 0, 0, 0 );

}


QPoint gkg::VoiStrategy::point4D2QPoint( gkg::Vector3d< int32_t >&, int32_t )
{

  return QPoint( 0, 0 );

}


QPoint gkg::VoiStrategy::mapFromScreen( float, float )
{

  return QPoint( 0, 0 );

}


QPointF gkg::VoiStrategy::mapToScreen( int32_t, int32_t )
{

  return QPointF( 0.0, 0.0 );

}


float gkg::VoiStrategy::distance( QPointF, QPointF )
{

  return 0.0f;

}


void gkg::VoiStrategy::setIndicator( QPainter* )
{
}


gkg::VoiStrategy::StgyType gkg::VoiStrategy::type()
{

  return gkg::VoiStrategy::StgyUndefined;

}


float gkg::VoiStrategy::correctionH()
{ 

  return correctH;
  
}


float gkg::VoiStrategy::correctionV()
{ 

  return correctV;
  
}


float gkg::VoiStrategy::zoomFactor()
{

  return zoomFact;
  
}


int32_t gkg::VoiStrategy::getSlice()
{

  return 0;

}
