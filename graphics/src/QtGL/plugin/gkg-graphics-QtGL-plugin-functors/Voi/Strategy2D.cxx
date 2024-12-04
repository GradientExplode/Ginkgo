#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy2D.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Cursor.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume_i.h>

#include <QApplication>

#include <cmath>


gkg::Voi2DStrategy::Voi2DStrategy() 
                   : gkg::VoiStrategy(), 
                     zoomLimit( 1.0f ),
                     contrastDown( false )
{
}


void gkg::Voi2DStrategy::paint( QPainter* painter, const QRectF& /* rect */ )
{

  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glClear( GL_COLOR_BUFFER_BIT );

  if ( !buffer.isNull() )
  {

    int32_t pixelRatio = painter->device()->devicePixelRatio();
    float corrH = zoomFact * correctH;
    float corrV = zoomFact * correctV;

    glShadeModel( GL_FLAT );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

    glViewport( 0, 0, pixelRatio * da->width(), pixelRatio * da->height() );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glEnable( GL_MULTISAMPLE );

    glPixelZoom( corrH * pixelRatio , -corrV * pixelRatio );

    glRasterPos2f( -1.0, 1.0 );

    float dX = ( (float)buffer.width() - (float)da->width() / corrH ) / 2.0f;
    float dY = ( (float)buffer.height() - (float)da->height() / corrV ) / 2.0f;
  
    if ( dX < 0.0 )
    {
  
      dX = 0.0;
    
    }
  
    if ( dY < 0.0 )
    {
  
      dY = 0.0;
    
    }
  
    QPoint delta( (int32_t)dX, (int32_t)dY );
    orig = depla + delta;

    glPixelStoref( GL_UNPACK_SKIP_PIXELS, orig.x() );
    glPixelStoref( GL_UNPACK_SKIP_ROWS, orig.y() );

    glDrawPixels( buffer.width(), buffer.height() - orig.y(), GL_RGBA, 
                  GL_UNSIGNED_BYTE, (const GLvoid *)buffer.scanLine( 0 ) );

    glPixelStoref( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStoref( GL_UNPACK_SKIP_ROWS, 0 );

    glDisable( GL_MULTISAMPLE );

    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    setIndicator( painter );

  }

}


void gkg::Voi2DStrategy::updatePalette( gkg::VoiData& data )
{

  updateData( data );

}


void gkg::Voi2DStrategy::updateDataLoaded( gkg::VoiData& data )
{

  initialize( data );
  updateData( data );
  
}


void gkg::Voi2DStrategy::updateZoom()
{

  zoomFact = zoomLimit;
  
}


void gkg::Voi2DStrategy::updateFrame( gkg::VoiData& data, int32_t frame )
{

  if ( frame != t )
  {
  
    t = frame;
    updateData( data );
    
  }
  
}


void gkg::Voi2DStrategy::deltaFrame( gkg::VoiData& data, int32_t d )
{

  int32_t delta = d + t;

  if ( ( delta >= 0 ) && ( delta < data.getBaseFusion().getT1().getSizeT() ) )
  {
  
    data.notifyFrame( delta );
    
  }
  
}


void gkg::Voi2DStrategy::mousePressEvent( gkg::VoiData& d, QMouseEvent *e )
{

  tmpCursor = da->cursor();

  if ( !buffer.isNull() )
  {
  
    if ( modeStrategy->id() == gkg::VoiModeStrategy::DistanceMode )
    {

      e->ignore();

    }
    else if ( modeStrategy->id() == gkg::VoiModeStrategy::SplineMode )
    {

      gkg::VoiLabelFactory factory;

      d.setLabelStrategy( factory.create( type() ) );
      e->ignore();

    }
    else
    {

      switch ( e->button() )
      {

        case Qt::LeftButton:
        {

          clickPos = e->pos();

          if ( ( QApplication::keyboardModifiers() & Qt::ShiftModifier ) && 
               ( modeStrategy->id() == gkg::VoiModeStrategy::SelectionMode ) )
          {

              deplaDown = true;
              gkg::VoiCursor curs( gkg::VoiCursor::VoiGraspingHandCursor );
              da->setCursor( curs.cursor() );

          }
          else if ( 
                 ( QApplication::keyboardModifiers() & Qt::ControlModifier ) &&
                 ( modeStrategy->id() == gkg::VoiModeStrategy::SelectionMode ) )
          {

            zoomDown = true;
            gkg::VoiCursor curs( gkg::VoiCursor::VoiMagnifyingGlassCursor );
            da->setCursor( curs.cursor() );

          }
          else
          {

            mouseDown = true;
            int32_t tt;
            gkg::Vector3d< int32_t > pt = click2Point4D( clickPos, &tt );
            gkg::VoiLabelFactory factory;
        
            d.setLabelStrategy( factory.create( type() ) );
            modeStrategy->pressed( *da, d, pt, tt, clickPos,
                                   QApplication::keyboardModifiers() );

          }

          e->accept();
          break;

        }
        case Qt::MiddleButton:
        {

          int32_t tt;

          clickPos = e->pos();
          contrastDown = true;

          gkg::Vector3d< int32_t > pt = click2Point4D( clickPos, &tt );

          contrastStrategy.pressed( *da, d, pt, tt, clickPos,
                                    QApplication::keyboardModifiers() );
          e->accept();
          break;

        }
        case Qt::RightButton:
        {
        
          eraseDown = true;
          QPoint click = e->pos();
          int32_t tt;
          gkg::Vector3d< int32_t > pt = click2Point4D( click, &tt );

          if ( modeStrategy->id() == gkg::VoiModeStrategy::PaintMode )
          {

            gkg::VoiLabelFactory factory;
            gkg::VoiCursor curs( gkg::VoiCursor::VoiGumCursor );
            tmpCursor = da->cursor();
            da->setCursor( curs.cursor() );
            d.setLabelStrategy( factory.create( type() ) );

          }

          modeStrategy->erase( *da, d, pt, tt, click,
                               QApplication::keyboardModifiers() );
          e->accept();
          break;
        
        }
        default:
          break;
        
      }

    }

  }

}


void gkg::Voi2DStrategy::mouseReleaseEvent( gkg::VoiData& d, QMouseEvent *e )
{

  if ( ( modeStrategy->id() == gkg::VoiModeStrategy::DistanceMode ) ||
       ( modeStrategy->id() == gkg::VoiModeStrategy::SplineMode ) )
  {

    e->ignore();

  }
  else
  {

    switch( e->button() )
    {

      case Qt::LeftButton:
      {

      	if ( !d.empty() && mouseDown )
    	  {
    	
    	    modeStrategy->released( d );
    	  
    	  }

     	  da->setCursor( tmpCursor );
     	  mouseDown = false;
    	  deplaDown = false;
    	  zoomDown = false;
    	  e->accept();
    	  break;

      }
      case Qt::MiddleButton:
      {

    	  contrastDown = false;
    	  e->accept();
    	  break;

      }
      case Qt::RightButton:
      {

    	  if ( !d.empty() )
    	  {
    	
    	    modeStrategy->released( d );
    	  
    	  }

    	  eraseDown = false;
    	  da->setCursor( tmpCursor );
    	  e->accept();
    	  break;

      }
      default:
    	  break;

    }

  }

}


void gkg::Voi2DStrategy::mouseMoveEvent( gkg::VoiData& d, QMouseEvent *e )
{

  if ( ( modeStrategy->id() == gkg::VoiModeStrategy::DistanceMode ) ||
       ( modeStrategy->id() == gkg::VoiModeStrategy::SplineMode ) )
  {

    e->ignore();

  }
  else
  {

    if ( mouseDown )
    {
  
      QPoint click = e->pos();
      int32_t tt;
      gkg::Vector3d< int32_t > pt = click2Point4D( click, &tt );

      modeStrategy->moved( *da, d, pt, tt, click,
                           QApplication::keyboardModifiers() );
      e->accept();
      
    }
    else if ( contrastDown )
    {
  
      QPoint click = e->pos();
      int32_t tt;
      gkg::Vector3d< int32_t > pt = click2Point4D( click, &tt );

      contrastStrategy.moved( *da, d, pt, tt, click,
                              QApplication::keyboardModifiers() );
      e->accept();
      
    }
    else if ( deplaDown )
    {
  
      depla += clickPos - e->pos();
      clickPos = e->pos();
      clipping();
      da->update();
      e->accept();
      
    }
    else if ( zoomDown )
    {
  
      if ( e->pos().y() < clickPos.y() )
      {
      
    	zoomFact *= 1.05;
    	
      }
      else
      {
      
    	zoomFact /= 1.05;
    	
      }
      
      if ( zoomFact <= zoomLimit )
      {
      
    	zoomFact = zoomLimit;
    	
      }

      clickPos = e->pos();
      clipping();
      da->update();
      e->accept();
      
    }
    else if ( eraseDown )
    {
  
      QPoint click = e->pos();
      int32_t tt;
      gkg::Vector3d< int32_t > pt = click2Point4D( click, &tt );

      modeStrategy->erase( *da, d, pt, tt, click,
                           QApplication::keyboardModifiers() );
      e->accept();
      
    }

  }

}


void gkg::Voi2DStrategy::keyPressEvent( VoiData& d, QKeyEvent* event )
{

  if ( !d.empty() )
  {

    switch ( modeStrategy->id() )
    {

      case gkg::VoiModeStrategy::SplineMode:
        event->ignore();
        break;

      case gkg::VoiModeStrategy::PaintMode:
      case gkg::VoiModeStrategy::EvolutionMode:
      case gkg::VoiModeStrategy::FillingMode:
        {

          int32_t l = d.labelInfo().currentLabel();

          if ( d.labelInfo().getShow( l ) )
          {

            if ( ( event->key() == Qt::Key_C ) &&
                 ( QApplication::keyboardModifiers() & Qt::ControlModifier ) )
            {
            
              _roiCache.setLabel( l );
              copyRoi( d, l );
              event->accept();

            }
            else if ( ( event->key() == Qt::Key_V ) &&
                   ( QApplication::keyboardModifiers() & Qt::ControlModifier ) )
            {
            
              if ( _roiCache.isValid( l ) )
              {

                pasteRoi( d, l );
                da->scene()->update();

              }

              event->accept();

            }
            else
            {

              event->ignore();

            }

          }

          break;

        }

      default:
        gkg::VoiStrategy::keyPressEvent( d, event );
        break;

    }

  }

}


QPoint gkg::Voi2DStrategy::mapFromScreen( float px, float py )
{

  int32_t x = 0, y = 0;

  if ( !buffer.isNull() )
  {

    x = (int32_t)( (float)orig.x() + px / zoomFact / correctH + 0.5);
    y = (int32_t)( (float)orig.y() + py / zoomFact / correctV + 0.5 );

    if ( x < 0 )
    {
    
      x = 0;
      
    }
    else if ( x >= buffer.width() )
    {
    
      x = buffer.width() - 1;
      
    }
    
    if ( y < 0 )
    {
    
      y = 0;
      
    }
    else if ( y >= buffer.height() )
    {
    
      y = buffer.height() - 1;
      
    }
    
  }

  return QPoint( x, y );

}


QPointF gkg::Voi2DStrategy::mapToScreen( int32_t px, int32_t py )
{

  float x = ( px - orig.x() ) * zoomFact * correctH;
  float y = ( py - orig.y() ) * zoomFact * correctV;

  return QPointF( x, y );

}


float gkg::Voi2DStrategy::distance( QPointF p1, QPointF p2 )
{

  float dx = ( p1.x() - p2.x() ) / zoomFact;
  float dy = ( p1.y() - p2.y() ) / zoomFact;

  return std::sqrt( dx * dx + dy * dy );

}


void gkg::Voi2DStrategy::clipping()
{

  float corrH = zoomFact * correctH;
  float corrV = zoomFact * correctV;

  float dX = ( (float)buffer.width() - (float)da->width() / corrH ) / 2.0f;
  float dY = ( (float)buffer.height() - (float)da->height() / corrV ) / 2.0f;
  
  if ( dX < 0.0 )
  {
  
    dX = 0.0;
    
  }
  
  if ( dY < 0.0 )
  {
  
    dY = 0.0;
    
  }

  QPoint delta( (int32_t)dX, (int32_t)dY );
  QPoint test1 = depla + delta;
  QPoint test2 = depla - delta;

  if ( test1.x() <= 0 )
  {
  
    depla.setX( -delta.x() );
    
  }
  
  if ( test1.y() <= 0 )
  {
  
    depla.setY( -delta.y() );
    
  }
  
  if ( test2.x() >= 0 )
  {
  
    depla.setX( delta.x() );
    
  }
  
  if ( test2.y() >= 0 )
  {
  
    depla.setY( delta.y() );
    
  }

}
