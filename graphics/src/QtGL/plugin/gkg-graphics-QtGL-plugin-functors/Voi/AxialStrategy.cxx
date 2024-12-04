#include <gkg-graphics-QtGL-plugin-functors/Voi/AxialStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VolumeBaseFusion.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadedLoop.h>

#include <QPainter>


class AxialContext : public gkg::LoopContext< int32_t >
{

  public:
  
    AxialContext( gkg::VoiData& d, int32_t z, int32_t t, uint8_t* bptr );
    
    void doIt( int32_t startIndex, int32_t countIndex );
    
  private:
  
    int32_t _z;
    int32_t _t;
    uint8_t* _bptr;
    gkg::Volume< int16_t >& _label;
    gkg::VoiVolumeBaseFusion& _baseFusion;
    gkg::VoiLabelInfo& _labelInfo;
    int32_t _sizeX;
    uint8_t* _bo;
    int32_t _fusL;
    int32_t _fusBV;
    
};


AxialContext::AxialContext( gkg::VoiData& d,
                            int32_t z,
                            int32_t t,
                            uint8_t* bptr )
             : gkg::LoopContext< int32_t >(),
               _z( z ),
               _t( t ),
               _bptr( bptr ),
               _label( d.label() ),
               _baseFusion( d.getBaseFusion() ),
               _labelInfo( d.labelInfo() ),
               _sizeX( d.getBaseFusion().getT1().getSizeX() ),
               _bo( d.labelInfo().byteOrder() ),
               _fusL( d.fusionLabel() ),
               _fusBV( d.fusionBaseVolume() )
{
}


void AxialContext::doIt( int32_t startIndex, int32_t countIndex )
{

  int32_t idx = startIndex;
  uint8_t* bptr = _bptr + 4 * startIndex;
  gkg::RGBComponent c, valBV;

  while ( countIndex-- )
  {
  
    int32_t y = idx / _sizeX;
    int32_t x = idx % _sizeX;
    int32_t val2 = _label( x, y, _z );

    valBV = _baseFusion.getFusionColor( _fusBV, x, y, _z, _t );
 
    if ( val2 && _labelInfo.getColor( val2, c ) )
    {

      valBV.r = (uint8_t)( ( _fusL * valBV.r + ( 100 - _fusL ) * c.r ) / 100 );
      valBV.g = (uint8_t)( ( _fusL * valBV.g + ( 100 - _fusL ) * c.g ) / 100 );
      valBV.b = (uint8_t)( ( _fusL * valBV.b + ( 100 - _fusL ) * c.b ) / 100 );

    }

    *((uint32_t *)bptr) = ( ( valBV.r << _bo[ 3 ] ) | ( valBV.g << _bo[ 2 ] ) |
                            ( valBV.b << _bo[ 1 ] ) | ( 0xff << _bo[ 0 ] ) );

    bptr += 4;  
    idx++;
  
  }

}


class AxialContourContext : public gkg::LoopContext< int32_t >
{

  public:
  
    AxialContourContext( gkg::VoiData& d, int32_t z, int32_t t, uint8_t* bptr );
    
    void doIt( int32_t startIndex, int32_t countIndex );
    
  private:
  
    int32_t _z;
    int32_t _t;
    uint8_t* _bptr;
    gkg::Volume< int16_t >& _label;
    gkg::VoiVolumeBaseFusion& _baseFusion;
    gkg::VoiLabelInfo& _labelInfo;
    int32_t _sizeX;
    int32_t _endY;
    uint8_t* _bo;
    int32_t _fusL;
    int32_t _fusBV;

};


AxialContourContext::AxialContourContext( gkg::VoiData& d,
                                          int32_t z,
                                          int32_t t,
                                          uint8_t* bptr )
                    : gkg::LoopContext< int32_t >(),
                      _z( z ),
                      _t( t ),
                      _bptr( bptr ),
                      _label( d.label() ),
                      _baseFusion( d.getBaseFusion() ),
                      _labelInfo( d.labelInfo() ),
                      _sizeX( d.getBaseFusion().getT1().getSizeX() ),
                      _endY( d.getBaseFusion().getT1().getSizeY() - 1 ),
                      _bo( d.labelInfo().byteOrder() ),
                      _fusL( d.fusionLabel() ),
                      _fusBV( d.fusionBaseVolume() )
{
}


void AxialContourContext::doIt( int32_t startIndex, int32_t countIndex )
{

  int32_t idx = startIndex;
  uint8_t* bptr = _bptr + 4 * startIndex;
  gkg::RGBComponent c, valBV;

  while ( countIndex-- )
  {
  
    int32_t y = idx / _sizeX;
    int32_t x = idx % _sizeX;
    int32_t val2 = _label( x, y, _z );

    valBV = _baseFusion.getFusionColor( _fusBV, x, y, _z, _t );
 
    if ( val2  )
    {

      bool ok = true;

      if ( x && ( x < ( _sizeX - 1 ) ) && y && ( y < _endY ) )
      {

        int32_t cpt = ( _label( x - 1, y, _z ) == val2 ) ? 1 : 0;

        cpt += ( _label( x + 1, y, _z ) == val2 ) ? 1 : 0;
        cpt += ( _label( x, y - 1, _z ) == val2 ) ? 1 : 0;
        cpt += ( _label( x, y + 1, _z ) == val2 ) ? 1 : 0;

        if ( cpt == 4 )
        {

          ok = false;

        }

      }

      if ( ok && _labelInfo.getColor( val2, c ) )
      {

        valBV.r = (uint8_t)( ( _fusL * valBV.r + 
                               ( 100 - _fusL ) * c.r ) / 100 );
        valBV.g = (uint8_t)( ( _fusL * valBV.g + 
                               ( 100 - _fusL ) * c.g ) / 100 );
        valBV.b = (uint8_t)( ( _fusL * valBV.b + 
                               ( 100 - _fusL ) * c.b ) / 100 );

      }

    }

    *((uint32_t *)bptr) = ( ( valBV.r << _bo[ 3 ] ) | ( valBV.g << _bo[ 2 ] ) |
                            ( valBV.b << _bo[ 1 ] ) | ( 0xff << _bo[ 0 ] ) );

    bptr += 4;  
    idx++;
  
  }

}


gkg::VoiAxialStrategy::VoiAxialStrategy() 
                      : gkg::Voi2DStrategy()
{
}


void gkg::VoiAxialStrategy::initialize( gkg::VoiData& data )
{

  int32_t dX, dY, dZ;

  if ( data.dataLoaded() )
  {
       
    gkg::Volume< float >& t1 = data.getBaseFusion().getT1();
    dX = t1.getSizeX();
    dY = t1.getSizeY();
    dZ = t1.getSizeZ();
    correctH = 1.0;
    correctV = 1.0;
    t1.getHeader().getAttribute( "resolutionX", correctH );
    t1.getHeader().getAttribute( "resolutionY", correctV );

  }
  else
  {

    dX = data.label().getSizeX();
    dY = data.label().getSizeY();
    dZ = data.label().getSizeZ();
    correctH = 1.0;
    correctV = 1.0;
    data.label().getHeader().getAttribute( "resolutionX", correctH );
    data.label().getHeader().getAttribute( "resolutionY", correctV );

  }

  float z1 = (float)da->width() / (float)dX / correctH;
  float z2 = (float)da->height() / (float)dY / correctV;

  zoomLimit = zoomFact = ( z1 < z2 ) ? z1 : z2;

  point.x = dX / 2;
  point.y = dY / 2;
  point.z = dZ / 2;
  t = 0;
  
}


void gkg::VoiAxialStrategy::updateData( gkg::VoiData& d )
{

  int32_t dimX = d.getBaseFusion().getT1().getSizeX();
  int32_t dimY = d.getBaseFusion().getT1().getSizeY();

  if ( buffer.isNull() || 
       ( ( dimX != buffer.width() ) && ( dimY != buffer.height() ) ) )
  {
  
    buffer = QImage( dimX, dimY, QImage::Format_ARGB32 );
    
  }

  uint8_t* bufferPtr = buffer.scanLine( 0 );

  if ( d.isContour() )
  {

    AxialContourContext context( d, point.z, t, bufferPtr );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, dimX * dimY );

    threadedLoop.launch();

  }
  else
  {

    AxialContext context( d, point.z, t, bufferPtr );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, dimX * dimY );

    threadedLoop.launch();

  }

  d.getSlicer().setTexture( gkg::Slicer::Axial, bufferPtr );

}


void gkg::VoiAxialStrategy::updateLabel( 
                                    gkg::VoiData& d, 
                                    std::list< gkg::Vector3d< int32_t > >& bck )
{
  
  if ( bck.size() )
  {

    gkg::RGBComponent c, valBV;
    uint8_t *nptr, *bptr = buffer.scanLine( 0 );
    int32_t fusBV = d.fusionBaseVolume();
    gkg::VoiVolumeBaseFusion& baseFusion = d.getBaseFusion();
    uint8_t* bo = d.labelInfo().byteOrder();
    std::list< gkg::Vector3d< int32_t > >::iterator 
      it = bck.begin(),
      ie = bck.end();
    int32_t val2 = d.label()( *it );
    int32_t endX = d.getBaseFusion().getT1().getSizeX() - 1;
    int32_t endY = d.getBaseFusion().getT1().getSizeY() - 1;
    bool isContour = d.isContour();

    if ( d.labelInfo().getColor( val2, c ) )
    {

      int32_t fusL = d.fusionLabel();

      while ( it != ie )
      {
 
        if ( it->z == point.z )
        {

          bool ok = true;

          nptr = bptr + 4 * ( it->y * buffer.width() + it->x );
          valBV = baseFusion.getFusionColor( fusBV, it->x, it->y, it->z, t );

          if ( isContour &&
               it->x && ( it->x < endX ) && it->y && ( it->y < endY ) )
          {

            int32_t cpt = ( d.label()( it->x - 1, it->y, it->z ) == val2 ) ? 1 
                                                                           : 0;

            cpt += ( d.label()( it->x + 1, it->y, it->z ) == val2 ) ? 1 : 0;
            cpt += ( d.label()( it->x, it->y - 1, it->z ) == val2 ) ? 1 : 0;
            cpt += ( d.label()( it->x, it->y + 1, it->z ) == val2 ) ? 1 : 0;

            if ( cpt == 4 )
            {

              ok = false;

            }

          }

          if ( ok )
          {

            valBV.r = (uint8_t)( ( fusL * valBV.r + 
                                   ( 100 - fusL ) * c.r ) / 100 );
            valBV.g = (uint8_t)( ( fusL * valBV.g + 
                                   ( 100 - fusL ) * c.g ) / 100 );
            valBV.b = (uint8_t)( ( fusL * valBV.b + 
                                   ( 100 - fusL ) * c.b ) / 100 );

          }

          *((uint32_t *)nptr) = ( ( valBV.r << bo[ 3 ] ) | 
                                  ( valBV.g << bo[ 2 ] ) | 
    	    		                    ( valBV.b << bo[ 1 ] ) | 
                                  ( 0xff << bo[ 0 ] ) );

        }

        ++it;

      }
        			  
    }
    else
    {

      while ( it != ie )
      {

        if ( it->z == point.z )
        {

          nptr = bptr + 4 * ( it->y * buffer.width() + it->x );
          valBV = baseFusion.getFusionColor( fusBV, it->x, it->y, it->z, t );
          *((uint32_t *)nptr) = ( ( valBV.r << bo[ 3 ] ) | 
                                  ( valBV.g << bo[ 2 ] ) | 
                                  ( valBV.b << bo[ 1 ] ) |
                                  ( 0xff << bo[ 0 ] ) );

        }

        ++it;

      }

    }
      
  }
  
}


void gkg::VoiAxialStrategy::updateSlice( gkg::VoiData& data, 
                                         gkg::Vector3d< int32_t >& pt, 
                                         int32_t tt )
{

  int32_t tmp = point.z;
  point = pt;

  if ( tt >= 0 )
  {

    t = tt;

  }

  if ( pt.z != tmp )
  {
  
    updateData( data );
    
  }
  
}


void gkg::VoiAxialStrategy::updateSlice( gkg::VoiData& data, int32_t slice )
{

  gkg::Vector3d< int32_t > pt( point.x, point.y, slice );
  data.notifySlice( pt, t );

}


void gkg::VoiAxialStrategy::deltaSlice( gkg::VoiData& data, int32_t d )
{

  int32_t pos = point.z + d;

  if ( ( pos >= 0 ) && 
       ( pos < data.getBaseFusion().getT1().getSizeZ() ) )
  {
  
    updateSlice( data, pos );
    
  }
  
}


void gkg::VoiAxialStrategy::setIndicator( QPainter* painter )
{

  float corrH = zoomFact * correctH;
  float corrV = zoomFact * correctV;

  int32_t x = (int32_t)( (float)( point.x - orig.x() ) * corrH + corrH / 2.0f );
  int32_t y = (int32_t)( (float)( point.y - orig.y() ) * corrV + corrV / 2.0f );

  QPoint p( x - 8, y - 7 );

  modeStrategy->setIndicator( painter, p );

}


gkg::VoiStrategy::StgyType gkg::VoiAxialStrategy::type()
{ 

  return gkg::VoiStrategy::StgyAxial;
  
}


void gkg::VoiAxialStrategy::copyRoi( VoiData& d, int32_t label )
{

  gkg::Volume< int16_t >& vol = d.label();
  int32_t sizeX = vol.getSizeX();
  int32_t sizeY = vol.getSizeY();
  gkg::Vector3d< int32_t > pos( 0, 0, point.z );

  for ( pos.y = 0; pos.y < sizeY; pos.y++ )
  {

    for ( pos.x = 0; pos.x < sizeX; pos.x++ )
    {

      if ( vol( pos ) == label )
      {

        _roiCache.add( pos );

      }

    }

  }

}


void gkg::VoiAxialStrategy::pasteRoi( VoiData& d, int32_t label )
{

  if ( !_roiCache.empty() )
  {

    gkg::Volume< int16_t >& vol = d.label();
    std::list< Vector3d< int32_t > >& roi = _roiCache.getRoi();
    std::list< Vector3d< int32_t > >::iterator
      p = roi.begin(),
      pe = roi.end();

    while ( p != pe )
    {

      p->z = point.z;
      vol( *p ) = label;
      ++p;

    }

    updateLabel( d, roi );

  }

}


gkg::Vector3d< int32_t >
gkg::VoiAxialStrategy::click2Point4D( QPoint& click, int32_t* tt )
{

  gkg::Vector3d< int32_t > pt;

  if ( !buffer.isNull() )
  {
  
    int32_t x = (int32_t)( (float)orig.x() + (float)click.x() / zoomFact / 
                           correctH );
    int32_t y = (int32_t)( (float)orig.y() + (float)click.y() / zoomFact / 
                           correctV );

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

    pt.x = x;
    pt.y = y;
    pt.z = point.z;
    
    if ( tt )
    {
    
      *tt = t;
      
    }
    
  }

  return pt;

}


QPoint gkg::VoiAxialStrategy::point4D2QPoint( gkg::Vector3d< int32_t >& pt, 
                                              int32_t )
{

  int32_t x = (int32_t)( (float)( pt.x - orig.x() ) * zoomFact * correctH );
  int32_t y = (int32_t)( (float)( pt.y - orig.y() ) * zoomFact * correctV );

  return QPoint( x, y );

}


int32_t gkg::VoiAxialStrategy::getSlice()
{

  return point.z;

}


RegisterStrategy( VoiAxialStrategy );
