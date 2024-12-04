#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy3D.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Cursor.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>
#include <gkg-processing-container/Volume_i.h>

#include <QApplication>

#include <GL/glu.h>

#include <cmath>


gkg::Voi3DStrategy::Voi3DStrategy() 
                  : gkg::VoiStrategy(),
                    _data( 0 ),
                    mouseDown2( false ),
                    _fov( 35.0f ),
                    _panX( 0.0f ),
                    _panY( 0.0f ),
                    _panX2( 0.0f ),
                    _panY2( 0.0f ),
                    _panZ2( 0.0f )
{

  trackball.setQuaternion( 
                      gkg::Quaternion< float >( -0.2f, -0.76f, 0.62f, 0.13f ) );

}


void gkg::Voi3DStrategy::setDrawA( gkg::VoiDrawA *d )
{ 

  int32_t pixelRatio = d->devicePixelRatio();
  gkg::VoiStrategy::setDrawA( d );
  trackball.setViewport( pixelRatio * da->width(), pixelRatio * da->height() );
  trackball2.setViewport( pixelRatio * da->width(), pixelRatio * da->height() );

  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  GLfloat lightKa[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // ambient
  GLfloat lightKd[] = { 0.7f, 0.7f, 0.7f, 1.0f }; // diffuse
  GLfloat lightKs[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // specular
  glLightfv( GL_LIGHT0, GL_AMBIENT, lightKa );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, lightKd );
  glLightfv( GL_LIGHT0, GL_SPECULAR, lightKs );
  float lightPos[] = { 0.0f, 0.0f, 20.0f, 1.0f };
  glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

}


void gkg::Voi3DStrategy::paint( QPainter* /* painter */,
                                const QRectF& /* rect */ )
{

  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

  if ( _data )
  {

    int32_t pixelRatio = da->devicePixelRatio();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport( 0, 0, pixelRatio * da->width(), pixelRatio * da->height() );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    // gluPerspective() : fovy, aspect, zNear, zFar
    gluPerspective( _fov,
                    (GLfloat)da->width() / (GLfloat)da->height(),
                    0.01f, 15.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // gluLookAt() : eye(x3), center(x3), up(x3)
    gluLookAt( 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

    glEnable( GL_MULTISAMPLE );

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_SMOOTH );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
 
    glTranslatef( _panX, _panY, 0.0f );
    glMultMatrixf( trackball.getRotation() );
    glTranslatef( -0.5f, -0.5f, -0.5f );

    // 3D cube
    drawCube();

    // Draw slices
    glEnable( GL_TEXTURE_2D );
    drawAxial();
    drawSagittal();
    drawCoronal();
    glDisable( GL_TEXTURE_2D );

    // labels
    glEnable( GL_LIGHTING );
    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_LIGHT0 );
    _voxelEngine.render( *_data );
    glDisable( GL_LIGHT0 );
    glDisable( GL_COLOR_MATERIAL );
    glDisable( GL_LIGHTING );

// WIP
#if 0
    // cylinder
    if ( modeStrategy->id() == gkg::VoiModeStrategy::CylinderMode )
    {

      //gkg::Quaternion< float > q = trackball2.getQuaternion() *
      //                             trackball.getQuaternion();
      glPushMatrix();
      glTranslatef( 0.5f + _panX2, 0.5f + _panY2, 0.5f + _panZ2 );
      glMultMatrixf( trackball2.getRotation() );
      glTranslatef( -0.5f, -0.5f, -0.5f );
      drawCylinder();
      glPopMatrix();

    }
#endif

    glDisable( GL_SMOOTH );
    glDisable( GL_DEPTH_TEST );

    glDisable( GL_MULTISAMPLE );

  }

}


void gkg::Voi3DStrategy::mousePressEvent( gkg::VoiData& d, QMouseEvent *e )
{

  switch ( e->button() )
  {

    case Qt::LeftButton:
    {

      mouseDown = true;
      clickPos = e->pos();

      if ( QApplication::keyboardModifiers() & Qt::ControlModifier )
      {

        linkedCursor( d );

      }
      else
      {

        trackball.mousePress( clickPos.x(), clickPos.y() );

      }

      e->accept();
      break;

    }
    case Qt::RightButton:
    {

      if ( modeStrategy->id() == gkg::VoiModeStrategy::CylinderMode )
      {

        mouseDown2 = true;
        clickPos2 = e->pos();
        trackball2.mousePress( clickPos2.x(), clickPos2.y() );
        e->accept();
        break;

      }

    }
    default:
      break;

  }
    
}


void gkg::Voi3DStrategy::linkedCursor( gkg::VoiData& d )
{

  float winX = float( clickPos.x() );
  float winY = float( da->height() - clickPos.y() - 1 );
  float winZ = 2.0f;

  glReadPixels( int32_t( winX ), int32_t( winY ), 1, 1, GL_DEPTH_COMPONENT,
                GL_FLOAT, &winZ );

  if ( winZ < 1.0f )
  {

    GLint viewport[ 4 ];
    GLdouble projection[ 16 ];
    GLdouble modelview[ 16 ];
    GLdouble px, py, pz;
    gkg::Volume< int16_t >& label = d.label();
    int32_t sizeX = label.getSizeX();
    int32_t sizeY = label.getSizeY();
    int32_t sizeZ = label.getSizeZ();
    int32_t posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    gluUnProject( winX, winY, winZ, modelview, projection, viewport,
                  &px, &py, &pz );
    d.getSlicer().getVolumeCoordinate( px, py, pz, posX, posY, posZ );

    if ( ( posX >= 0 ) && ( posX < sizeX ) &&
         ( posY >= 0 ) && ( posY < sizeY ) &&
         ( posZ >= 0 ) && ( posZ < sizeZ ) )
    {

      gkg::Vector3d< int32_t > pt( posX, posY, posZ );
      d.notifySlice( pt, t );

    }

  }

}


void gkg::Voi3DStrategy::mouseReleaseEvent( gkg::VoiData&, QMouseEvent *e )
{

  switch ( e->button() )
  {

    case Qt::LeftButton:
    {

      mouseDown = false;
      e->accept();
      break;

    }
    case Qt::RightButton:
    {

      mouseDown2 = false;
      e->accept();
      break;

    }
    default:
      break;

  }
  
}


void gkg::Voi3DStrategy::mouseMoveEvent( gkg::VoiData&, QMouseEvent *e )
{

  if ( mouseDown )
  {
  
    QPoint pos = e->pos();

    if ( QApplication::keyboardModifiers() & Qt::ShiftModifier )
    {

      _panX -= float( clickPos.x() - pos.x() ) / da->width();
      _panY += float( clickPos.y() - pos.y() ) / da->height();
      clickPos = pos;

      if ( _panX < -0.5f ) 
      {

        _panX = -0.5f;

      }
      else if ( _panX > 0.5f ) 
      {

        _panX = 0.5f;

      }
      if ( _panY < -0.5f ) 
      {

        _panY = -0.5f;

      }
      else if ( _panY > 0.5f ) 
      {

        _panY = 0.5f;

      }

    }
    else
    {

      trackball.mouseMove( pos.x(), pos.y() );
      clickPos = pos;

    }

    da->update();
    e->accept();
    
  }
  else if ( mouseDown2 )
  {

    QPoint pos = e->pos();

    if ( QApplication::keyboardModifiers() & Qt::ShiftModifier )
    {

      float oldX = float( clickPos2.x() );
      float oldY = float( da->height() - clickPos2.y() - 1 );
      float oldZ = 2.0f;
      float newX = float( pos.x() );
      float newY = float( da->height() - pos.y() - 1 );
      GLint viewport[ 4 ];
      GLdouble projection[ 16 ];
      GLdouble modelview[ 16 ];
      GLdouble ox, oy, oz, nx, ny, nz;

      glReadPixels( int32_t( oldX ), int32_t( oldY ), 1, 1, GL_DEPTH_COMPONENT,
                    GL_FLOAT, &oldZ );
      glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
      glGetDoublev( GL_PROJECTION_MATRIX, projection );
      glGetIntegerv( GL_VIEWPORT, viewport );
      gluUnProject( oldX, oldY, oldZ, modelview, projection, viewport,
                    &ox, &oy, &oz );
      gluUnProject( newX, newY, oldZ, modelview, projection, viewport,
                    &nx, &ny, &nz );

      _panX2 += nx - ox;
      _panY2 += ny - oy;
      _panZ2 += nz - oz;
      clickPos2 = pos;

      if ( _panX2 < -0.5f ) 
      {

        _panX2 = -0.5f;

      }
      else if ( _panX2 > 0.5f ) 
      {

        _panX2 = 0.5f;

      }
      if ( _panY2 < -0.5f ) 
      {

        _panY2 = -0.5f;

      }
      else if ( _panY2 > 0.5f ) 
      {

        _panY2 = 0.5f;

      }
      if ( _panZ2 < -0.5f ) 
      {

        _panZ2 = -0.5f;

      }
      else if ( _panZ2 > 0.5f ) 
      {

        _panZ2 = 0.5f;

      }

    }
    else
    {

      trackball2.mouseMove( pos.x(), pos.y() );
      clickPos2 = pos;

    }

    e->accept();

  }
  
}


void gkg::Voi3DStrategy::wheelEvent( gkg::VoiData&, QWheelEvent* e )
{

  _fov *= ( e->angleDelta().y() > 0 ) ? ( 1.0f / 1.1f ) : 1.1f;

  if ( _fov < 1.0f )
  {

    _fov = 1.0f;

  }
  else if ( _fov > 150.0f )
  {

    _fov = 150.0f;

  }

  da->update();
  e->accept();

}


void gkg::Voi3DStrategy::centerObjects()
{

  _panX = 0.0f;
  _panY = 0.0f;
  da->update();

}


void gkg::Voi3DStrategy::leave()
{

  _voxelEngine.leave();

}


void gkg::Voi3DStrategy::drawCube()
{

  gkg::Slicer& slicer = _data->getSlicer();
  double slabX = slicer.getSlabX();
  double sX = slicer.getStartX();
  double eX = slicer.getStopX();
  double slabY = slicer.getSlabY();
  double sY = slicer.getStartY();
  double eY = slicer.getStopY();
  double slabZ = slicer.getSlabZ();
  double sZ = slicer.getStartZ();
  double eZ = slicer.getStopZ();

  // cube
  glBegin( GL_LINE_LOOP );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 0.0f );
    glVertex3f( 0.0f, 1.0f, 0.0f );
    glVertex3f( 0.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 1.0f );
  glEnd();

  glBegin( GL_LINE_LOOP );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 0.0f, 1.0f );
    glVertex3f( 0.0f, 0.0f, 1.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 1.0f, 0.0f, 0.0f );
  glEnd();

  glBegin( GL_LINE_LOOP );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 0.0f, 1.0f, 1.0f );
    glVertex3f( 0.0f, 0.0f, 1.0f );
    glVertex3f( 1.0f, 0.0f, 1.0f );
  glEnd();

  glBegin( GL_LINE_LOOP );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 1.0f, 0.0f );
    glVertex3f( 1.0f, 1.0f, 0.0f );
  glEnd();

  glBegin( GL_LINE_LOOP );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 0.0f, 1.0f, 1.0f );
    glVertex3f( 0.0f, 1.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 1.0f );
  glEnd();

  glBegin( GL_LINE_LOOP );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 1.0f, 0.0f );
    glVertex3f( 1.0f, 1.0f, 1.0f );
    glVertex3f( 1.0f, 0.0f, 1.0f );
    glVertex3f( 1.0f, 0.0f, 0.0f );
  glEnd();

  // dataset limits in x
  if ( slabX < 1.0 )
  {

    glBegin( GL_LINE_LOOP );
      glColor3f( 0.1f, 0.1f, 0.7f );
      glVertex3d( eX, eY, eZ );
      glVertex3d( eX, sY, eZ );
      glVertex3d( eX, sY, sZ );
      glVertex3d( eX, eY, sZ );
    glEnd();

    glBegin( GL_LINE_LOOP );
      glColor3f( 0.1f, 0.1f, 0.7f );
      glVertex3d( sX, eY, sZ );
      glVertex3d( sX, sY, sZ );
      glVertex3d( sX, sY, eZ );
      glVertex3d( sX, eY, eZ );
    glEnd();

  }

  // dataset limits in y
  if ( slabY < 1.0 )
  {

    glBegin( GL_LINE_LOOP );
      glColor3f( 0.1f, 0.1f, 0.7f );
      glVertex3d( eX, eY, eZ );
      glVertex3d( sX, eY, eZ );
      glVertex3d( sX, eY, sZ );
      glVertex3d( eX, eY, sZ );
    glEnd();

    glBegin( GL_LINE_LOOP );
      glColor3f( 0.1f, 0.1f, 0.7f );
      glVertex3d( eX, sY, sZ );
      glVertex3d( sX, sY, sZ );
      glVertex3d( sX, sY, eZ );
      glVertex3d( eX, sY, eZ );
    glEnd();

  }

  // dataset limits in z
  if ( slabZ < 1.0 )
  {

    glBegin( GL_LINE_LOOP );
      glColor3f( 0.1f, 0.1f, 0.7f );
      glVertex3d( eX, eY, eZ );
      glVertex3d( sX, eY, eZ );
      glVertex3d( sX, sY, eZ );
      glVertex3d( eX, sY, eZ );
    glEnd();

    glBegin( GL_LINE_LOOP );
      glColor3f( 0.1f, 0.1f, 0.7f );
      glVertex3d( eX, sY, sZ );
      glVertex3d( sX, sY, sZ );
      glVertex3d( sX, eY, sZ );
      glVertex3d( eX, eY, sZ );
    glEnd();

  }

  glColor3f( 1.0f, 1.0f, 1.0f );

}


void gkg::Voi3DStrategy::drawAxial()
{

  gkg::Slicer& slicer = _data->getSlicer();
  GLdouble sX = slicer.getStartX();
  GLdouble eX = slicer.getStopX();
  GLdouble sY = slicer.getStartY();
  GLdouble eY = slicer.getStopY();
  GLdouble cx, cy, cz;

  slicer.getTextureCoordinate( point.x, point.y, point.z, cx, cy, cz );
  glBindTexture( GL_TEXTURE_2D, slicer.getTextureId( gkg::Slicer::Axial ) );

  glBegin( GL_QUADS );
    glTexCoord2d( 0.0, 0.0);
    glVertex3d( sX, sY, cz );
    glTexCoord2d( 0.0, 1.0 );
    glVertex3d( sX, eY, cz );
    glTexCoord2d( 1.0, 1.0 );
    glVertex3d( eX, eY, cz );
    glTexCoord2d( 1.0, 0.0 );
    glVertex3d( eX, sY, cz );
  glEnd();

  glBindTexture( GL_TEXTURE_2D, 0 );

}


void gkg::Voi3DStrategy::drawSagittal()
{

  gkg::Slicer& slicer = _data->getSlicer();
  GLdouble sY = slicer.getStartY();
  GLdouble eY = slicer.getStopY();
  GLdouble sZ = slicer.getStartZ();
  GLdouble eZ = slicer.getStopZ();
  GLdouble cx, cy, cz;

  slicer.getTextureCoordinate( point.x, point.y, point.z, cx, cy, cz );
  glBindTexture( GL_TEXTURE_2D,
                 slicer.getTextureId( gkg::Slicer::Sagittal ) );

  glBegin( GL_QUADS );
    glTexCoord2d( 0.0, 0.0 );
    glVertex3d( cx, sY, sZ );
    glTexCoord2d( 0.0, 1.0 );
    glVertex3d( cx, sY, eZ );
    glTexCoord2d( 1.0, 1.0 );
    glVertex3d( cx, eY, eZ );
    glTexCoord2d( 1.0, 0.0 );
    glVertex3d( cx, eY, sZ );
  glEnd();

  glBindTexture( GL_TEXTURE_2D, 0 );

}


void gkg::Voi3DStrategy::drawCoronal()
{

  gkg::Slicer& slicer = _data->getSlicer();
  GLdouble sX = slicer.getStartX();
  GLdouble eX = slicer.getStopX();
  GLdouble sZ = slicer.getStartZ();
  GLdouble eZ = slicer.getStopZ();
  GLdouble cx, cy, cz;

  slicer.getTextureCoordinate( point.x, point.y, point.z, cx, cy, cz );
  glBindTexture( GL_TEXTURE_2D, slicer.getTextureId( gkg::Slicer::Coronal ) );

   glBegin( GL_QUADS );
    glTexCoord2d( 0.0, 0.0 );
    glVertex3d( sX, cy, sZ );
    glTexCoord2d( 0.0, 1.0 );
    glVertex3d( sX, cy, eZ );
    glTexCoord2d( 1.0, 1.0 );
    glVertex3d( eX, cy, eZ );
    glTexCoord2d( 1.0, 0.0 );
    glVertex3d( eX, cy, sZ );
  glEnd();
 
  glBindTexture( GL_TEXTURE_2D, 0 );

}


void gkg::Voi3DStrategy::drawCylinder()
{

  // Axial pour le moment
  gkg::Slicer& slicer = _data->getSlicer();
  double angle, angleMax = 2.0 * M_PI + 0.2;
  double radius = 0.5 * _data->getCylinderDiameter() / slicer.getMax();
  GLdouble x = 0.0, y = 0.0, z = 0.0;

  glColor3f( 1.0f, 0.4f, 0.4f );

  switch ( _data->getCylinderOrientation() )
  {

    case 1:
    default:
    {

      double sZ = slicer.getStartZ();
      double eZ = slicer.getStopZ();

      glBegin( GL_QUAD_STRIP );
        for ( angle = 0.0; angle < angleMax; angle += 0.1 )
        {

          x = 0.5 + radius * std::cos( angle );
          y = 0.5 + radius * std::sin( angle );
          glVertex3d( x, y, eZ );
          glVertex3d( x, y, sZ );
      
        }
      glEnd();
      break;

    }

    case 2:
    {

      double sY = slicer.getStartY();
      double eY = slicer.getStopY();

      glBegin( GL_QUAD_STRIP );
        for ( angle = 0.0; angle < angleMax; angle += 0.1 )
        {

          x = 0.5 + radius * std::cos( angle );
          z = 0.5 + radius * std::sin( angle );
          glVertex3d( x, eY, z );
          glVertex3d( x, sY, z );
      
        }
      glEnd();
      break;

    }

    case 3:
    {

      double sX = slicer.getStartX();
      double eX = slicer.getStopX();

      glBegin( GL_QUAD_STRIP );
        for ( angle = 0.0; angle < angleMax; angle += 0.1 )
        {

          y = 0.5 + radius * std::sin( angle );
          z = 0.5 + radius * std::cos( angle );
          glVertex3d( eX, y, z );
          glVertex3d( sX, y, z );
      
        }
      glEnd();
      break;

    }

  }

}


void gkg::Voi3DStrategy::updateDataLoaded( gkg::VoiData& data )
{

  _voxelEngine.initialize( data );
  _data = &data;

}


void gkg::Voi3DStrategy::updateLabelLoaded( gkg::VoiData& data )
{

  _voxelEngine.initialize( data );
  _data = &data;

}


void gkg::Voi3DStrategy::updateLabel( VoiData& data, 
                                      std::list< Vector3d< int32_t > >& bck )
{

  _voxelEngine.update( data, bck );

}


void gkg::Voi3DStrategy::updatePalette( VoiData& data )
{

  std::map< int32_t, gkg::LabelInformation >& infos = data.labelInfo().info();
  std::map< int32_t, gkg::LabelInformation >::iterator
    i = infos.begin(),
    ie = infos.end();

  while ( i != ie )
  {

    _voxelEngine.updateVisible( i->first, i->second.show );
    ++i;

  }

}


void gkg::Voi3DStrategy::updateSlice( gkg::VoiData& /* data */, 
                                      gkg::Vector3d< int32_t >& pt, 
                                      int32_t tt )
{

  point = pt;
  t = tt;

}


gkg::VoiStrategy::StgyType gkg::Voi3DStrategy::type()
{ 

  return gkg::VoiStrategy::Stgy3D;
  
}


RegisterStrategy( Voi3DStrategy );
