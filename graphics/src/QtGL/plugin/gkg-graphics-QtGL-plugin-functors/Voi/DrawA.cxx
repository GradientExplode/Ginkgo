#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ToolBar.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ScrollBars.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy3D.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Workspace.h>
#include <gkg-processing-container/Volume_i.h>

#include <QGLFormat>


gkg::VoiDrawA::VoiDrawA( gkg::VoiStrategy::StgyType type,
                         gkg::VoiObservable *data, 
                         const QGLFormat& format,
                         const QGLWidget* shareWidget,
                         QWidget *parent ) 
             : QGraphicsView(),
               gkg::VoiObserver()
{

  _scene = new gkg::GraphicsScene( this, (gkg::VoiData*)data );

  setViewport( new QGLWidget( format, parent, shareWidget ) );
  setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
  setScene( _scene );
  setFrameStyle( QFrame::NoFrame );

  strategy = gkg::StrategyFactory::getInstance().getStrategy( type );
  registerData( data );
  data->registerView( this );
  setMouseTracking( true );

  _strategies.push_back( strategy );

  if ( type == gkg::VoiStrategy::Stgy3D )
  {

    _strategies.push_back( gkg::StrategyFactory::getInstance().getStrategy( 
                                            gkg::VoiStrategy::StgyHistogram ) );
    _strategies.push_back( gkg::StrategyFactory::getInstance().getStrategy( 
                                              gkg::VoiStrategy::StgyProfile ) );

  }

  std::vector< gkg::VoiStrategy* >::iterator
    s = _strategies.begin(),
    se = _strategies.end();

  while ( s != se )
  {

    (*s)->setDrawA( this );
    ++s;

  }

  _toolBar = new ToolBar( this );
  _toolBar->setFixedHeight( 40 );
  _toolBar->hide();
  connect( _toolBar, SIGNAL( changeView( int ) ), 
           this, SLOT( muteWidget( int ) ) );

  _scrollBars = new ScrollBars( this );
  _scrollBars->hide();
  connect( _scrollBars, SIGNAL( valueChanged( int ) ), 
           this, SLOT( updateSlice( int ) ) );
  connect( _scrollBars, SIGNAL( frameChanged( int ) ), 
           this, SLOT( updateOneFrame( int ) ) );

}


void gkg::VoiDrawA::paint( QPainter* painter, const QRectF& rect )
{

  strategy->paint( painter, rect );

}


QPoint gkg::VoiDrawA::mapFromScreen( QPointF p )
{

  return mapFromScreen( p.x(), p.y() );

}

QPoint gkg::VoiDrawA::mapFromScreen( float px, float py )
{

  return strategy->mapFromScreen( px, py );

}


QPointF gkg::VoiDrawA::mapToScreen( QPoint p )
{

  return mapToScreen( p.x(), p.y() );

}


QPointF gkg::VoiDrawA::mapToScreen( int32_t px, int32_t py )
{

  return strategy->mapToScreen( px, py );

}


float gkg::VoiDrawA::distance( QPointF p1, QPointF p2 )
{

  return strategy->distance( p1, p2 );

}


int32_t gkg::VoiDrawA::getCurrentSlice()
{

  return strategy->getSlice();

}


void gkg::VoiDrawA::resizeEvent( QResizeEvent* e )
{

  if ( _scene )
  {

    _scene->setSceneRect( QRect( QPoint( 0, 0 ), e->size() ) );

  }

  _scrollBars->setSizes( e->size().width(), e->size().height() );

  QGraphicsView::resizeEvent( e );

}


void gkg::VoiDrawA::mousePressEvent( QMouseEvent* e )
{

  strategy->mousePressEvent( (gkg::VoiData&)**datas.begin(), e );

  if ( !e->isAccepted() )
  {

    QGraphicsView::mousePressEvent( e );

  }

  scene()->update();

}


void gkg::VoiDrawA::mouseReleaseEvent( QMouseEvent* e )
{

  strategy->mouseReleaseEvent( (gkg::VoiData&)**datas.begin(), e );

  if ( !e->isAccepted() )
  {

    QGraphicsView::mouseReleaseEvent( e );

  }

  scene()->update();

}


void gkg::VoiDrawA::mouseMoveEvent( QMouseEvent* e )
{

  if ( e->buttons() == Qt::NoButton )
  {

    if ( rect().contains( e->pos() ) )
    {

      setFocus( Qt::MouseFocusReason );

    }

    if ( int32_t( strategy->type() ) >= int32_t( gkg::VoiStrategy::Stgy3D ) )
    {

      if ( e->pos().y() < gkg::Workspace::getInstance().getIconSize().height() )
      {

        _toolBar->show();

      }
      else
      {

        _toolBar->hide();

      }

    }
    else
    {

      if ( ( e->pos().y() > 
             gkg::Workspace::getInstance().getIconSize().height() ) && 
           ( e->pos().x() > _scrollBars->pos().x() ) )
      {

        _scrollBars->show();

      }
      else
      {

        _scrollBars->hide();

      }

    }

    QGraphicsView::mouseMoveEvent( e );

  }
  else
  {

    strategy->mouseMoveEvent( (gkg::VoiData&)**datas.begin(), e );

    if ( !e->isAccepted() )
    {

      QGraphicsView::mouseMoveEvent( e );

    }

  }

  scene()->update();

}


void gkg::VoiDrawA::wheelEvent( QWheelEvent* e )
{

  if ( ( int32_t( strategy->type() ) >
         int32_t( gkg::VoiStrategy::StgyUndefined ) ) &&
       ( int32_t( strategy->type() ) < int32_t( gkg::VoiStrategy::Stgy3D ) ) )
  {

    _scrollBars->deltaSlice( ( e->angleDelta().y() > 0 ) ? -1 : 1 );
    e->accept();

  }
  else if ( int32_t( strategy->type() ) == int32_t( gkg::VoiStrategy::Stgy3D ) )
  {

    strategy->wheelEvent( (gkg::VoiData&)**datas.begin(), e );

  }

  scene()->update();

}


void gkg::VoiDrawA::keyPressEvent( QKeyEvent* event )
{

  strategy->keyPressEvent( (gkg::VoiData&)**datas.begin(), event );

  if ( !event->isAccepted() )
  {

    QGraphicsView::keyPressEvent( event );

  }

}


void gkg::VoiDrawA::leaveEvent( QEvent* e )
{

  _toolBar->hide();
  _scrollBars->hide();
  
  QGraphicsView::leaveEvent( e );

}


void gkg::VoiDrawA::updateDataLoaded()
{

  if ( !datas.empty() )
  {

    std::vector< gkg::VoiStrategy* >::iterator
      s = _strategies.begin(),
      se = _strategies.end();

    while ( s != se )
    {

      (*s)->updateDataLoaded( (gkg::VoiData&)**datas.begin() );
      ++s;

    }

    scene()->update();

  }

}


void gkg::VoiDrawA::updateLabelLoaded()
{

  if ( !datas.empty() )
  {

    strategy->updateLabelLoaded( (gkg::VoiData&)**datas.begin() );
    scene()->update();

  }

}


void gkg::VoiDrawA::updateData()
{

  if ( !datas.empty() )
  {

    strategy->updateData( (gkg::VoiData&)**datas.begin() );
    scene()->update();

  }

}


void gkg::VoiDrawA::updatePalette()
{

  if ( !datas.empty() )
  {

    strategy->updatePalette( (gkg::VoiData&)**datas.begin() );
    scene()->update();

  }

}


void gkg::VoiDrawA::updateLabel( std::list< gkg::Vector3d< int32_t > >& bck )
{

  if ( !datas.empty() )
  {

    strategy->updateLabel( (gkg::VoiData&)**datas.begin(), bck );

    if ( strategy != _strategies[ 0 ] )
    {

      _strategies[ 0 ]->updateLabel( (gkg::VoiData&)**datas.begin(), bck );

    }

    scene()->update();

  }

}


void gkg::VoiDrawA::updateScrollBar()
{

  if ( !datas.empty() )
  {

    _scrollBars->initialize( (gkg::VoiData&)**datas.begin(), 
                             _strategies[ 0 ]->type() );

  }

}


void gkg::VoiDrawA::updateZoom()
{

  if ( !datas.empty() )
  {

    strategy->updateZoom();
    scene()->update();

  }

}


void gkg::VoiDrawA::updateSlice( gkg::Vector3d< int32_t >& pt, int32_t t )
{

  if ( !datas.empty() )
  {

    std::vector< gkg::VoiStrategy* >::iterator
      s = _strategies.begin(),
      se = _strategies.end();

    while ( s != se )
    {

      (*s)->updateSlice( (gkg::VoiData&)**datas.begin(), pt, t );
      ++s;

    }

    _scrollBars->updateSlice( pt, _strategies[ 0 ]->type() );
    scene()->update();

  }

}


void gkg::VoiDrawA::updateSlice( int slice )
{

  if ( !datas.empty() )
  {

    strategy->updateSlice( (gkg::VoiData&)**datas.begin(), slice );

  }

}


void gkg::VoiDrawA::updateFrame( int frame )
{

  if ( !datas.empty() )
  {

    strategy->updateFrame( (gkg::VoiData&)**datas.begin(), frame );
    scene()->update();

  }

}


void gkg::VoiDrawA::redrawView()
{

  if ( !datas.empty() )
  {

    scene()->update();

  }

}


void gkg::VoiDrawA::updateOneFrame( int frame )
{

  if ( !datas.empty() )
  {

    ((gkg::VoiData*)*datas.begin())->notifyFrame( frame );

  }

}


void gkg::VoiDrawA::increase()
{

  if ( !datas.empty() ) 
  {
  
    strategy->deltaSlice( (gkg::VoiData&)**datas.begin(), 1 );
    
  }

}


void gkg::VoiDrawA::decrease()
{

  if ( !datas.empty() ) 
  {
  
    strategy->deltaSlice( (gkg::VoiData&)**datas.begin(), -1 );
    
  }
  
}


void gkg::VoiDrawA::incFrame()
{

  if ( !datas.empty() )
  {
  
    strategy->deltaFrame( (gkg::VoiData&)**datas.begin(), 1 );
    
  }

}


void gkg::VoiDrawA::decFrame()
{

  if ( !datas.empty() )
  {
  
    strategy->deltaFrame( (gkg::VoiData&)**datas.begin(), -1 );
    
  }

}


void gkg::VoiDrawA::muteWidget( int type )
{

  if ( ( type < int32_t( _strategies.size() ) ) &&
       ( strategy != _strategies[ type ] ) )
  {

    strategy->leave();
    strategy = _strategies[ type ];
    resize( width(), height() );
    scene()->update();

  }
  else if ( type == 3 )
  {

    strategy->centerObjects();

  }

}


void gkg::VoiDrawA::updateMode( gkg::VoiModeStrategy* strgy )
{

  unsetCursor();
  strategy->updateMode( strgy );
  setCursor( strgy->cursor() );

  if ( strgy && _scene )
  {

    _scene->setMode( strgy->id() );

  }

}
