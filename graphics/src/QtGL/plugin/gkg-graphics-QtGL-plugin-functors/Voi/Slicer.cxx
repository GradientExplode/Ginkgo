#include <gkg-graphics-QtGL-plugin-functors/Voi/Slicer.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>


gkg::Slicer::Slicer()
           : _sizeX( 1 ),
             _sizeY( 1 ),
             _sizeZ( 1 ),
             _deltaX( 0.1 ),
             _deltaY( 0.1 ),
             _deltaZ( 0.1 ),
             _max( 1.0 ),
             _slabX( 1.0 ),
             _startX( 0.0 ),
             _stopX( 1.0 ),
             _slabY( 1.0 ),
             _startY( 0.0 ),
             _stopY( 1.0 ),
             _slabZ( 1.0 ),
             _startZ( 0.0 ),
             _stopZ( 1.0 )
{

  _sliceTexture[ 0 ] = 0;
  _sliceTexture[ 1 ] = 0;
  _sliceTexture[ 2 ] = 0;

}


gkg::Slicer::~Slicer()
{

  glDeleteTextures( 3, _sliceTexture );

}


void gkg::Slicer::initialize( gkg::VoiData& data )
{

  gkg::Vector3d< double > res;

  _sizeX = data.label().getSizeX();
  _sizeY = data.label().getSizeY();
  _sizeZ = data.label().getSizeZ();

  double sX = double( _sizeX );
  double sY = double( _sizeY );
  double sZ = double( _sizeZ );

  data.label().getResolution( res );
  _max = sZ * res.z;

  if ( ( sY * res.y ) > _max )
  {

    _max = sY * res.y;

  }
  if ( ( sX * res.x ) > _max )
  {

    _max = sX * res.x;

  }

  _deltaX = res.x / _max;
  _deltaY = res.y / _max;
  _deltaZ = res.z / _max;
  _slabX = sX * _deltaX;
  _startX = ( 1.0 - _slabX ) / 2.0;
  _stopX = 1.0 - _startX;
  _slabY = sY * _deltaY;
  _startY = ( 1.0 - _slabY ) / 2.0;
  _stopY = 1.0 - _startY;
  _slabZ = sZ * _deltaZ;
  _startZ = ( 1.0 - _slabZ ) / 2.0;
  _stopZ = 1.0 - _startZ;

  glGenTextures( 3, _sliceTexture );

}


void gkg::Slicer::getTexelCoordinates( int32_t x, int32_t y, int32_t z,
                                       double& x0, double& y0, double& z0,
                                       double& x1, double& y1, double& z1 )
{

  x0 = _startX + x * _deltaX;
  y0 = _startY + y * _deltaY;
  z0 = _startZ + z * _deltaZ;
  x1 = x0 + _deltaX;
  y1 = y0 + _deltaY;
  z1 = z0 + _deltaZ;

}


void gkg::Slicer::getTextureCoordinate( int32_t x, int32_t y, int32_t z,
                                        double& r, double& s, double& t )
{

  r = _startX + x * _deltaX;
  s = _startY + y * _deltaY;
  t = _startZ + z * _deltaZ;

}


void gkg::Slicer::getVolumeCoordinate( double x, double y, double z,
                                       int32_t& posX, int32_t& posY,
                                       int32_t& posZ )
{

    posX = int32_t( ( x - _startX ) / _deltaX );
    posY = int32_t( ( y - _startY ) / _deltaY );
    posZ = int32_t( ( z - _startZ ) / _deltaZ );

}


void gkg::Slicer::setTexture( gkg::Slicer::SliceType type, uint8_t* buffer )
{

  if ( buffer )
  {

    int32_t dimX, dimY;

    switch ( type )
    {

      case gkg::Slicer::Axial:
        dimX = _sizeX;
        dimY = _sizeY;
        break;

      case gkg::Slicer::Sagittal:
        dimX = _sizeY;
        dimY = _sizeZ;
        break;

      case gkg::Slicer::Coronal:
        dimX = _sizeX;
        dimY = _sizeZ;
        break;
      
      default:
        return;
    }

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glBindTexture( GL_TEXTURE_2D, _sliceTexture[ type ] );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 
                  dimX, dimY, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
    glBindTexture( GL_TEXTURE_2D, 0 );

  }

}


GLuint gkg::Slicer::getTextureId( SliceType type )
{

  return _sliceTexture[ type ];

}


double gkg::Slicer::getMax()
{

  return _max;

}


double gkg::Slicer::getSlabX()
{

  return _slabX;

}


double gkg::Slicer::getStartX()
{

  return _startX;

}


double gkg::Slicer::getStopX()
{

  return _stopX;

}


double gkg::Slicer::getSlabY()
{

  return _slabY;

}


double gkg::Slicer::getStartY()
{

  return _startY;

}


double gkg::Slicer::getStopY()
{

  return _stopY;

}


double gkg::Slicer::getSlabZ()
{

  return _slabZ;

}


double gkg::Slicer::getStartZ()
{

  return _startZ;

}


double gkg::Slicer::getStopZ()
{

  return _stopZ;

}
