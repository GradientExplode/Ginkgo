#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/CreateState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ValidateState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StartDistanceState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineItem.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Node.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <QGraphicsSceneMouseEvent>
#include <QApplication>

#include <list>
#include <cmath>


gkg::GraphicsScene::GraphicsScene( gkg::VoiDrawA* da, gkg::VoiData* data )
                  : QGraphicsScene(),
                    _state( 0 ),
                    _da( da ),
                    _data( data ),
                    _splineSlice( 0 ),
                    _splineLabel( -1 ),
                    _distanceMeasure( false ),
                    _distanceLine( 0 ),
                    _distanceText( 0 )
{

  _splineItem = new gkg::SplineItem();
  addItem( _splineItem );
  setMode( gkg::VoiModeStrategy::SplineMode );

  connect( &data->labelInfo(), SIGNAL( currentChanged( int ) ), 
           this, SLOT( setItemColor( int ) ) );

}


gkg::GraphicsScene::~GraphicsScene()
{

  delete _state;

}


void gkg::GraphicsScene::drawBackground( QPainter* painter, const QRectF& rect )
{

  if ( ( painter->paintEngine()->type() != QPaintEngine::OpenGL ) &&
       ( painter->paintEngine()->type() != QPaintEngine::OpenGL2 ) )
  {

    return;

  }

  if ( _da )
  {

    _da->paint( painter, rect );

  }

}


void gkg::GraphicsScene::clearScene()
{

  if ( _distanceMeasure )
  {

    std::map< QGraphicsLineItem*, QGraphicsSimpleTextItem* >::iterator
      d = _distances.find( _distanceLine ),
      de = _distances.end();

    if ( d == de )
    {

      if ( _distanceLine )
      {

        removeItem( _distanceLine );

      }

      if ( _distanceText )
      {

        removeItem( _distanceText );

      }

    }

    d = _distances.begin();

    while ( d != de )
    {

      removeItem( d->first );
      removeItem( d->second );
      ++d;

    }

    _distances.clear();
    _distanceLine = 0;
    _distanceText = 0;

  }
  else
  {
  
    foreach ( QGraphicsItem* item, items() )
    {

      gkg::Node* node = qgraphicsitem_cast< gkg::Node* >( item );

      if ( node )
      {

        removeItem( node );

      }

    }

    _splineItem->clear();

  }

}


void gkg::GraphicsScene::setMode( gkg::VoiModeStrategy::ModeId id )
{

  clearScene();

  if ( id == gkg::VoiModeStrategy::DistanceMode )
  {

    _distanceMeasure = true;
    setState( new gkg::StartDistanceState( this ) );

  }
  else
  {

    _distanceMeasure = false;
    setState( new gkg::CreateState( this ) );

  }

}



void gkg::GraphicsScene::setState( gkg::EventState* state )
{

  if ( state )
  {

    delete _state;

    _state = state;

  }

}


void gkg::GraphicsScene::setItemColor( int label )
{

  gkg::RGBComponent color;
  _data->labelInfo().getColor( label, color );
  _splineItem->setColor( color );
  _splineLabel = label;

}


void gkg::GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{

  if ( ( _splineLabel > -1 ) || _distanceMeasure )
  {

    _state->mousePressEvent( event );

  }

}


void gkg::GraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

  _state->mouseMoveEvent( event );
  QGraphicsScene::mouseMoveEvent( event );

}


void gkg::GraphicsScene::keyPressEvent( QKeyEvent* event )
{

  if ( _splineLabel > -1 )
  {

    _state->keyPressEvent( event );

  }

}


void gkg::GraphicsScene::addNode( QPointF pos )
{

  if ( !_splineItem->size() )
  {

    _splineSlice = _da->getCurrentSlice();

  }

  gkg::Node* node = new gkg::Node( _splineItem, pos );

  addItem( node );
  _splineItem->addKnot( node );

}


void gkg::GraphicsScene::insertNode( QPointF pos )
{

  gkg::Node* node = new gkg::Node( _splineItem, pos );

  addItem( node );
  _splineItem->insertKnot( node );

}


bool gkg::GraphicsScene::removeNode( QPointF pos )
{

  QTransform transform;
  gkg::Node* node = dynamic_cast< gkg::Node* >( itemAt( pos, transform ) );

  if ( node )
  {

    removeItem( node );
    _splineItem->removeKnot( node );
    return true;

  }

  return false;

}


void gkg::GraphicsScene::removeLastNode()
{

  if ( _splineItem->size() )
  {

    gkg::Node* node = _splineItem->getKnots().back();
    removeItem( node );
    _splineItem->removeKnot( node );

  }

}


bool gkg::GraphicsScene::closeCurve()
{

  if ( _splineItem->size() )
  {

    _splineItem->closeSpline();

    return true;

  }

  return false;  

}


void gkg::GraphicsScene::move( QGraphicsSceneMouseEvent* event )
{

  QGraphicsScene::mousePressEvent( event );

}


void gkg::GraphicsScene::validateSpline()
{

  QPolygonF& polygon = _splineItem->getPolygon();
  QRectF bbox = polygon.boundingRect();
  QPoint tl = _da->mapFromScreen( bbox.topLeft() );
  QPoint br = _da->mapFromScreen( bbox.bottomRight() );
  int32_t x, x0 = tl.x(), x1 = br.x();
  int32_t y, y0 = tl.y(), y1 = br.y();
  std::list< QPoint > spline;

  polygon.push_back( polygon.front() );

  // Non-zero winding fill
  for ( y = y0; y <= y1; y++ )
  {

    for ( x = x0; x <= x1; x++ )
    {

      if ( polygon.containsPoint( _da->mapToScreen( x, y ), Qt::WindingFill ) )
      {

        spline.push_back( QPoint( x, y ) );

      }

    }

  }

  _data->splineToLabel( spline, _splineSlice, _splineLabel );
  _splineCache.save( _splineItem->getKnots() );
  clearScene();

}


void gkg::GraphicsScene::undo()
{

  if ( !_splineItem->size() )
  {

    _data->undoEvolution();

  }

}


void gkg::GraphicsScene::copySpline()
{

  if ( !_splineItem->size() )
  {

    _splineCache.initializeScene( this );

  }

}


void gkg::GraphicsScene::addFirstPoint( QPointF pos )
{

  _startDistance = pos;
  _distanceLine = addLine( QLineF( pos, pos ), 
                           QPen( Qt::red, 1, Qt::SolidLine ) );
  _distanceLine->setPos( pos );
  _distanceText = addSimpleText( "", QFont( "helvetica", 12 ) );
  _distanceText->setBrush( QBrush( Qt::red ) );

}


void gkg::GraphicsScene::drawLineTo( QPointF pos )
{

  float x1 = _startDistance.x();
  float y1 = _startDistance.y();
  float x2 = pos.x();
  float y2 = pos.y();
  float vx = x2 - x1;
  float vy = y2 - y1;
  float scal = std::fabs( vx ) / std::sqrt( vx * vx + vy * vy );
  float midX = 0.5f * ( x1 + x2 );
  float midY = 0.5f * ( y1 + y2 );
  float quarterH = 0.25f * _da->height();
  float quarterW = 0.75f * _da->width();
  float distance = _da->distance( _startDistance, pos );
  QString text = QString( "%1 mm" ).arg( distance );
  float len = 10.0f * text.length();

  if ( scal > 0.5f )
  {

    midX -= 0.5f * len;
    midY += ( midY < quarterH ) ? 16.0f : -32.0f;

  }
  else
  {

    midX += ( midX < quarterW ) ? 16.0f : -16.0f - len;

  }

  _distanceLine->setLine( 0, 0, 
                          pos.x() - _startDistance.x(),
                          pos.y() - _startDistance.y() );
  _distanceText->setPos( midX, midY );
  _distanceText->setText( text );

}


void gkg::GraphicsScene::acceptDistance( bool accept )
{

  if ( accept )
  {

    _distances.insert( std::make_pair( _distanceLine, _distanceText ) );

  }
  else
  {

    removeItem( _distanceLine );
    removeItem( _distanceText );

    _distanceLine = 0;
    _distanceText = 0;

  }

}
