#include <gkg-graphics-QtGL-plugin-functors/Voi/SagittalStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadedLoop.h>

#include <QPainter>


class SagittalContext : public gkg::LoopContext< int32_t >
{

  public:
  
    SagittalContext( gkg::VoiData& d, int32_t x, int32_t t, uint8_t* bptr );
    
    void doIt( int32_t startIndex, int32_t countIndex );
    
  private:
  
    int32_t _x;
    int32_t _t;
    uint8_t* _bptr;
    gkg::Volume< int16_t >& _label;
    gkg::VoiVolumeBaseFusion& _baseFusion;
    gkg::VoiLabelInfo& _labelInfo;
    int32_t _sizeY;
    uint8_t* _bo;
    int32_t _fusL;
    int32_t _fusBV;
    
};


SagittalContext::SagittalContext( gkg::VoiData& d,
                                  int32_t x,
                                  int32_t t,
                                  uint8_t* bptr )
                : gkg::LoopContext< int32_t >(),
                  _x( x ),
                  _t( t ),
                  _bptr( bptr ),
                  _label(  d.label() ),
                  _baseFusion( d.getBaseFusion() ),
                  _labelInfo( d.labelInfo() ),
                  _sizeY( d.getBaseFusion().getT1().getSizeY() ),
                  _bo( d.labelInfo().byteOrder() ),
                  _fusL( d.fusionLabel() ),
                  _fusBV( d.fusionBaseVolume() )
{
}


void SagittalContext::doIt( int32_t startIndex, int32_t countIndex )
{

  int32_t idx = startIndex;
  uint8_t* bptr = _bptr + 4 * startIndex;
  gkg::RGBComponent c, valBV;

  while ( countIndex-- )
  {
  
    int32_t z = idx / _sizeY;
    int32_t y = idx % _sizeY;
    int32_t val2 = _label( _x, y, z );

    valBV = _baseFusion.getFusionColor( _fusBV, _x, y, z, _t );
 
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


class SagittalContourContext : public gkg::LoopContext< int32_t >
{

  public:
  
    SagittalContourContext( gkg::VoiData& d, 
                            int32_t x, int32_t t, uint8_t* bptr );
    
    void doIt( int32_t startIndex, int32_t countIndex );
    
  private:
  
    int32_t _x;
    int32_t _t;
    uint8_t* _bptr;
    gkg::Volume< int16_t >& _label;
    gkg::VoiVolumeBaseFusion& _baseFusion;
    gkg::VoiLabelInfo& _labelInfo;
    int32_t _sizeY;
    int32_t _endZ;
    uint8_t* _bo;
    int32_t _fusL;
    int32_t _fusBV;
    
};


SagittalContourContext::SagittalContourContext( gkg::VoiData& d,
                                                int32_t x,
                                                int32_t t,
                                                uint8_t* bptr )
                       : gkg::LoopContext< int32_t >(),
                         _x( x ),
                         _t( t ),
                         _bptr( bptr ),
                         _label(  d.label() ),
                         _baseFusion( d.getBaseFusion() ),
                         _labelInfo( d.labelInfo() ),
                         _sizeY( d.getBaseFusion().getT1().getSizeY() ),
                         _endZ( d.getBaseFusion().getT1().getSizeZ() - 1 ),
                         _bo( d.labelInfo().byteOrder() ),
                         _fusL( d.fusionLabel() ),
                         _fusBV( d.fusionBaseVolume() )
{
}


void SagittalContourContext::doIt( int32_t startIndex, int32_t countIndex )
{

  int32_t idx = startIndex;
  uint8_t* bptr = _bptr + 4 * startIndex;
  gkg::RGBComponent c, valBV;

  while ( countIndex-- )
  {
  
    int32_t z = idx / _sizeY;
    int32_t y = idx % _sizeY;
    int32_t val2 = _label( _x, y, z );

    valBV = _baseFusion.getFusionColor( _fusBV, _x, y, z, _t );
 
    if ( val2 )
    {

      bool ok = true;

      if ( y && ( y < ( _sizeY - 1 ) ) && z && ( z < _endZ ) )
      {

        int32_t cpt = ( _label( _x, y - 1, z ) == val2 ) ? 1 : 0;

        cpt += ( _label( _x, y + 1, z ) == val2 ) ? 1 : 0;
        cpt += ( _label( _x, y, z - 1 ) == val2 ) ? 1 : 0;
        cpt += ( _label( _x, y, z + 1 ) == val2 ) ? 1 : 0;

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


gkg::VoiSagittalStrategy::VoiSagittalStrategy() 
                         : gkg::Voi2DStrategy()
{
}


void gkg::VoiSagittalStrategy::initialize( gkg::VoiData& data )
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
    t1.getHeader().getAttribute( "resolutionY", correctH );
    t1.getHeader().getAttribute( "resolutionZ", correctV );
    
  }
  else
  {
  
    dX = data.label().getSizeX();
    dY = data.label().getSizeY();
    dZ = data.label().getSizeZ();
    correctH = 1.0;
    correctV = 1.0;
    data.label().getHeader().getAttribute( "resolutionY", correctH );
    data.label().getHeader().getAttribute( "resolutionZ", correctV );
    
  }

  float z1 = (float)da->width() / (float)dY / correctH;
  float z2 = (float)da->height() / (float)dZ / correctV;

  zoomLimit = zoomFact = ( z1 < z2 ) ? z1 : z2;

  point.x = dX / 2;
  point.y = dY / 2;
  point.z = dZ / 2;
  t = 0;
  
}


void gkg::VoiSagittalStrategy::updateData( gkg::VoiData& d )
{

  int32_t dimY = d.getBaseFusion().getT1().getSizeY();
  int32_t dimZ = d.getBaseFusion().getT1().getSizeZ();

  if ( buffer.isNull() ||
       ( ( dimY != buffer.width() ) && ( dimZ != buffer.height() ) ) )
  {
  
    buffer = QImage( dimY, dimZ, QImage::Format_ARGB32 );
    
  }

  uint8_t* bufferPtr = buffer.scanLine( 0 );

  if ( d.isContour() )
  {

    SagittalContourContext context( d, point.x, t, bufferPtr );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, dimY * dimZ );

    threadedLoop.launch();

  }
  else
  {

    SagittalContext context( d, point.x, t, bufferPtr );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, dimY * dimZ );

    threadedLoop.launch();

  }

  d.getSlicer().setTexture( gkg::Slicer::Sagittal, bufferPtr );

}


void gkg::VoiSagittalStrategy::updateLabel( 
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
    int32_t endY = d.getBaseFusion().getT1().getSizeY() - 1;
    int32_t endZ = d.getBaseFusion().getT1().getSizeZ() - 1;
    bool isContour = d.isContour();

    if ( d.labelInfo().getColor( val2, c ) )
    {
  
      int32_t fusL = d.fusionLabel();

      while ( it != ie )
      {

      	if ( it->x == point.x )
    	  {

          bool ok = true;

    	    nptr = bptr + 4 * ( it->z * buffer.width() + it->y );
    	    valBV = baseFusion.getFusionColor( fusBV, it->x, it->y, it->z, t );

          if ( isContour &&
               it->y && ( it->y < endY ) && it->z && ( it->z < endZ ) )
          {

            int32_t cpt = ( d.label()( it->x, it->y - 1, it->z ) == val2 ) ? 1 
                                                                           : 0;

            cpt += ( d.label()( it->x, it->y + 1, it->z ) == val2 ) ? 1 : 0;
            cpt += ( d.label()( it->x, it->y, it->z - 1 ) == val2 ) ? 1 : 0;
            cpt += ( d.label()( it->x, it->y, it->z + 1 ) == val2 ) ? 1 : 0;

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

    	  if ( it->x == point.x )
    	  {

    	    nptr = bptr + 4 * ( it->z * buffer.width() + it->y );
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


void gkg::VoiSagittalStrategy::updateSlice( gkg::VoiData& data, 
                                            gkg::Vector3d< int32_t >& pt, 
                                            int32_t tt )
{

  int32_t tmp = point.x;
  point = pt;

  if ( tt >= 0 )
  {

    t = tt;

  }

  if ( pt.x != tmp )
  {
  
    updateData( data );
    
  }
  
}


void gkg::VoiSagittalStrategy::updateSlice( gkg::VoiData& data, int32_t slice )
{

  gkg::Vector3d< int32_t > pt( slice, point.y, point.z );
  data.notifySlice( pt, t );
  
}


void gkg::VoiSagittalStrategy::deltaSlice( gkg::VoiData& data, int32_t d )
{

  int32_t pos = point.x + d;

  if ( ( pos >= 0 ) &&
       ( pos < data.getBaseFusion().getT1().getSizeX() ) )
  {
  
    updateSlice( data, pos );
    
  }
  
}


void gkg::VoiSagittalStrategy::setIndicator( QPainter* painter )
{

  float corrH = zoomFact * correctH;
  float corrV = zoomFact * correctV;

  int32_t x = (int32_t)( (float)( point.y - orig.x() ) * corrH + corrH / 2.0f );
  int32_t y = (int32_t)( (float)( point.z - orig.y() ) * corrV + corrV / 2.0f );

  QPoint p( x - 8, y - 7 );

  modeStrategy->setIndicator( painter, p );
  
}


gkg::VoiStrategy::StgyType gkg::VoiSagittalStrategy::type() 
{

  return gkg::VoiStrategy::StgySagittal;
  
}


void gkg::VoiSagittalStrategy::copyRoi( VoiData& d, int32_t label )
{

  gkg::Volume< int16_t >& vol = d.label();
  int32_t sizeY = vol.getSizeY();
  int32_t sizeZ = vol.getSizeZ();
  gkg::Vector3d< int32_t > pos( point.x, 0, 0 );

  for ( pos.z = 0; pos.z < sizeZ; pos.z++ )
  {

    for ( pos.y = 0; pos.y < sizeY; pos.y++ )
    {

      if ( vol( pos ) == label )
      {

        _roiCache.add( pos );

      }

    }

  }

}


void gkg::VoiSagittalStrategy::pasteRoi( VoiData& d, int32_t label )
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

      p->x = point.x;
      vol( *p ) = label;
      ++p;

    }

    updateLabel( d, roi );

  }

}


gkg::Vector3d< int32_t >
gkg::VoiSagittalStrategy::click2Point4D( QPoint& click, int32_t* tt )
{

  gkg::Vector3d< int32_t > pt;

  if ( !buffer.isNull() )
  {
  
    int32_t y = (int32_t)( (float)orig.x() + (float)click.x() / zoomFact / 
                           correctH );
    int32_t z = (int32_t)( (float)orig.y() + (float)click.y() / zoomFact / 
                           correctV );

    if ( y < 0 )
    {
    
      y = 0;
      
    }
    else if ( y >= buffer.width() )
    {
    
      y = buffer.width() - 1;
      
    }
    
    if ( z < 0 )
    {
    
      z = 0;
      
    }
    else if ( z >= buffer.height() )
    {
    
      z = buffer.height() - 1;
      
    }

    pt.x = point.x;
    pt.y = y;
    pt.z = z;
    
    if ( tt )
    {
    
      *tt = t;
      
    }
    
  }

  return pt;

}


QPoint gkg::VoiSagittalStrategy::point4D2QPoint( gkg::Vector3d< int32_t >& pt, 
                                                 int32_t )
{

  int32_t x = (int32_t)( (float)( pt.y - orig.x() ) * zoomFact * correctH );
  int32_t y = (int32_t)( (float)( pt.z - orig.y() ) * zoomFact * correctV );

  return QPoint( x, y );

}


int32_t gkg::VoiSagittalStrategy::getSlice()
{

  return point.x;

}


RegisterStrategy( VoiSagittalStrategy );
