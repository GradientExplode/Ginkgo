#include <gkg-graphics-QtGL-plugin-functors/Voi/VolumeBaseFusion.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ReadProcess.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadedLoop.h>

#include <fstream>


static uint8_t redTemperature[ 256 ][ 3 ] =
{

  { 0, 0, 0 },
  { 1, 0, 0 },
  { 2, 0, 0 },
  { 4, 0, 0 },
  { 5, 0, 0 },
  { 7, 0, 0 },
  { 8, 0, 0 },
  { 10, 0, 0 },
  { 11, 0, 0 },
  { 13, 0, 0 },
  { 14, 0, 0 },
  { 15, 0, 0 },
  { 17, 0, 0 },
  { 18, 0, 0 },
  { 20, 0, 0 },
  { 21, 0, 0 }, 
  { 23, 0, 0 },
  { 24, 0, 0 },
  { 26, 0, 0 },
  { 27, 0, 0 },
  { 28, 0, 0 },
  { 30, 0, 0 },
  { 31, 0, 0 },
  { 33, 0, 0 },
  { 34, 0, 0 },
  { 36, 0, 0 },
  { 37, 0, 0 },
  { 39, 0, 0 },
  { 40, 0, 0 },
  { 42, 0, 0 },
  { 43, 0, 0 },
  { 44, 0, 0 },
  { 46, 0, 0 },
  { 47, 0, 0 },
  { 49, 0, 0 },
  { 50, 0, 0 },
  { 52, 0, 0 },
  { 53, 0, 0 },
  { 55, 0, 0 },
  { 56, 0, 0 },
  { 57, 0, 0 },
  { 59, 0, 0 },
  { 60, 0, 0 },
  { 62, 0, 0 },
  { 63, 0, 0 },
  { 65, 0, 0 },
  { 66, 0, 0 },
  { 68, 0, 0 },
  { 69, 0, 0 },
  { 70, 0, 0 },
  { 72, 0, 0 },
  { 73, 0, 0 },
  { 75, 0, 0 },
  { 76, 0, 0 },
  { 78, 0, 0 },
  { 79, 0, 0 },
  { 81, 0, 0 },
  { 82, 0, 0 },
  { 84, 0, 0 },
  { 85, 0, 0 },
  { 86, 0, 0 },
  { 88, 0, 0 },
  { 89, 0, 0 },
  { 91, 0, 0 },
  { 92, 0, 0 },
  { 94, 0, 0 },
  { 95, 0, 0 },
  { 97, 0, 0 },
  { 98, 0, 0 },
  { 99, 0, 0 },
  { 101, 0, 0 },
  { 102, 0, 0 },
  { 104, 0, 0 },
  { 105, 0, 0 },
  { 107, 0, 0 },
  { 108, 0, 0 },
  { 110, 0, 0 },
  { 111, 0, 0 },
  { 113, 0, 0 },
  { 114, 0, 0 },
  { 115, 0, 0 },
  { 117, 0, 0 },
  { 118, 0, 0 },
  { 120, 0, 0 },
  { 121, 0, 0 },
  { 123, 0, 0 },
  { 124, 0, 0 },
  { 126, 0, 0 },
  { 127, 0, 0 },
  { 128, 0, 0 },
  { 130, 0, 0 },
  { 131, 0, 0 },
  { 133, 0, 0 },
  { 134, 0, 0 },
  { 136, 0, 0 },
  { 137, 0, 0 },
  { 139, 0, 0 },
  { 140, 0, 0 },
  { 141, 0, 0 },
  { 143, 0, 0 },
  { 144, 0, 0 },
  { 146, 0, 0 },
  { 147, 0, 0 },
  { 149, 0, 0 },
  { 150, 0, 0 },
  { 152, 0, 0 },
  { 153, 0, 0 },
  { 155, 0, 0 },
  { 156, 0, 0 },
  { 157, 0, 0 },
  { 159, 0, 0 },
  { 160, 0, 0 },
  { 162, 0, 0 },
  { 163, 0, 0 },
  { 165, 0, 0 },
  { 166, 0, 0 },
  { 168, 0, 0 },
  { 169, 0, 0 },
  { 170, 0, 0 },
  { 172, 0, 0 },
  { 173, 0, 0 },
  { 175, 1, 0 },
  { 176, 3, 0 },
  { 178, 5, 0 },
  { 179, 7, 0 },
  { 181, 9, 0 },
  { 182, 11, 0 },
  { 184, 13, 0 },
  { 185, 15, 0 },
  { 186, 17, 0 },
  { 188, 18, 0 },
  { 189, 20, 0 },
  { 191, 22, 0 },
  { 192, 24, 0 },
  { 194, 26, 0 },
  { 195, 28, 0 },
  { 197, 30, 0 },
  { 198, 32, 0 },
  { 199, 34, 0 },
  { 201, 35, 0 },
  { 202, 37, 0 },
  { 204, 39, 0 },
  { 205, 41, 0 },
  { 207, 43, 0 },
  { 208, 45, 0 },
  { 210, 47, 0 },
  { 211, 49, 0 },
  { 212, 51, 0 },
  { 214, 52, 0 },
  { 215, 54, 0 },
  { 217, 56, 0 },
  { 218, 58, 0 },
  { 220, 60, 0 },
  { 221, 62, 0 },
  { 223, 64, 0 },
  { 224, 66, 0 },
  { 226, 68, 0 },
  { 227, 69, 0 },
  { 228, 71, 0 },
  { 230, 73, 0 },
  { 231, 75, 0 },
  { 233, 77, 0 },
  { 234, 79, 0 },
  { 236, 81, 0 },
  { 237, 83, 0 },
  { 239, 85, 0 },
  { 240, 86, 0 },
  { 241, 88, 0 },
  { 243, 90, 0 },
  { 244, 92, 0 },
  { 246, 94, 0 },
  { 247, 96, 0 },
  { 249, 98, 0 },
  { 250, 100, 0 },
  { 252, 102, 0 },
  { 253, 103, 0 },
  { 255, 105, 0 },
  { 255, 107, 0 },
  { 255, 109, 0 },
  { 255, 111, 0 },
  { 255, 113, 0 },
  { 255, 115, 0 },
  { 255, 117, 0 },
  { 255, 119, 0 },
  { 255, 120, 0 },
  { 255, 122, 0 },
  { 255, 124, 0 },
  { 255, 126, 0 },
  { 255, 128, 0 },
  { 255, 130, 0 },
  { 255, 132, 0 },
  { 255, 134, 3 },
  { 255, 136, 7 },
  { 255, 137, 11 },
  { 255, 139, 15 },
  { 255, 141, 19 },
  { 255, 143, 23 },
  { 255, 145, 27 },
  { 255, 147, 31 },
  { 255, 149, 35 },
  { 255, 151, 39 },
  { 255, 153, 43 },
  { 255, 154, 47 },
  { 255, 156, 51 },
  { 255, 158, 54 },
  { 255, 160, 58 },
  { 255, 162, 62 },
  { 255, 164, 66 },
  { 255, 166, 70 },
  { 255, 168, 74 },
  { 255, 170, 78 },
  { 255, 171, 82 },
  { 255, 173, 86 },
  { 255, 175, 90 },
  { 255, 177, 94 },
  { 255, 179, 98 },
  { 255, 181, 102 },
  { 255, 183, 105 },
  { 255, 185, 109 },
  { 255, 187, 113 },
  { 255, 188, 117 },
  { 255, 190, 121 },
  { 255, 192, 125 },
  { 255, 194, 129 },
  { 255, 196, 133 },
  { 255, 198, 137 },
  { 255, 200, 141 },
  { 255, 202, 145 },
  { 255, 204, 149 },
  { 255, 205, 153 },
  { 255, 207, 156 },
  { 255, 209, 160 },
  { 255, 211, 164 },
  { 255, 213, 168 },
  { 255, 215, 172 },
  { 255, 217, 176 },
  { 255, 219, 180 },
  { 255, 221, 184 },
  { 255, 222, 188 },
  { 255, 224, 192 },
  { 255, 226, 196 },
  { 255, 228, 200 },
  { 255, 230, 204 },
  { 255, 232, 207 },
  { 255, 234, 211 },
  { 255, 236, 215 },
  { 255, 238, 219 },
  { 255, 239, 223 },
  { 255, 241, 227 },
  { 255, 243, 231 },
  { 255, 245, 235 },
  { 255, 247, 239 },
  { 255, 249, 243 },
  { 255, 251, 247 },
  { 255, 253, 251 },
  { 255, 255, 255 }

};


class MinMaxRgbContext : public gkg::LoopContext< int32_t >
{

  public:

    MinMaxRgbContext( uint8_t* ptr, float& min, float& max );
    
    void doIt( int32_t startIndex, int32_t countIndex );
    
  private:

    uint8_t* _ptr;
    float& _min;
    float& _max;

};


MinMaxRgbContext::MinMaxRgbContext( uint8_t* ptr, float& min, float& max )
                : gkg::LoopContext< int32_t >(),
                  _ptr( ptr ),
                  _min( min ),
                  _max( max )
{
}


void MinMaxRgbContext::doIt( int32_t startIndex, int32_t countIndex )
{

  float min = _min;
  float max = _max;
  uint8_t* p = _ptr + startIndex;

  while ( countIndex-- )
  {

    float val = float( *p++ );
  	
    if ( val < min )
    {
	
      min = val;

    }
    else if ( val > max )
    {
	
      max = val;

    }

  }

  lock();

  if ( min < _min )
  {

    _min = min;

  }
  if ( max > _max )
  {

    _max = max;

  }

  unlock();

}


gkg::VoiVolumeBaseFusion::VoiVolumeBaseFusion()
                        : _minT1( 0.0f ),
                          _maxT1( 0.0f ),
                          _fusion( 0 ),
                          _dimXB( 0 ),
                          _dimYB( 0 ),
                          _dimZB( 0 )
{
}


gkg::VoiVolumeBaseFusion::~VoiVolumeBaseFusion()
{
}


// load

bool gkg::VoiVolumeBaseFusion::load( const std::string& t1Name,
                                     const std::string& volBName,
                                     const std::string& transfoName )
{

  if ( !t1Name.empty() )
  {

    loadT1( t1Name );

    if ( !volBName.empty() )
    {

      _trmVolBToT1.setIdentity();
      loadVolB( volBName );

      if ( !transfoName.empty() )
      {

        std::ifstream ifs( transfoName.c_str() );

        _trmVolBToT1.readTrm( ifs );
        ifs.close();

      }

    }

    return true;

  }

  return false;

}


void gkg::VoiVolumeBaseFusion::loadT1( const std::string& fname )
{

  gkg::ReadProcess process( gkg::TypeOf< float >::getName(), 0, &_t1 );

  process.execute( fname );
  initializeT1();
  _adc.reallocate();
  _rgb.reallocate();
  _trmVolBToT1.setIdentity();
  _fusion = 0;
  _dimXB = 0;
  _dimYB = 0;
  _dimZB = 0;

}


void gkg::VoiVolumeBaseFusion::loadVolB( const std::string& fname )
{

  gkg::ReadProcess process( gkg::TypeOf< float >::getName(), 0, &_adc, &_rgb );

  process.execute( fname );
    
  if ( process.isRgb() )
  {

    initializeRgb();
    _fusion = 2;

  }
  else
  {

    initializeAdc();
    _fusion = 1;

  }

}


void gkg::VoiVolumeBaseFusion::setFusion( const int8_t& nb )
{

  _fusion = nb;

}


gkg::Volume<float>& gkg::VoiVolumeBaseFusion::getT1()
{

  return _t1;

}


gkg::VoiLutInfo& gkg::VoiVolumeBaseFusion::getLut( bool shift )
{

  if ( shift && _fusion )
  {

    return ( _fusion > 1 ) ? _lutRgb : _lutAdc;


  }

  return _lutT1;

}


int8_t gkg::VoiVolumeBaseFusion::getFusion()
{

  return _fusion;

}


void gkg::VoiVolumeBaseFusion::initializeT1()
{

  float lowerVal, upperVal;

  gkg::HistogramAnalyzer::getInstance().getPercentileBoundariesAndMinMax(
         _t1, gkg::VoiLutInfo::nLut, 98.0, lowerVal, upperVal, _minT1, _maxT1 );

  _lutT1.setOrig( _minT1, _maxT1 );
  _lutT1.computeLut( lowerVal, upperVal );
  _t1.getResolution( _res );
  
}


void gkg::VoiVolumeBaseFusion::initializeAdc()
{

  float lowerVal, upperVal, minAdc, maxAdc;

  gkg::HistogramAnalyzer::getInstance().getPercentileBoundariesAndMinMax(
        _adc, gkg::VoiLutInfo::nLut, 98.0, lowerVal, upperVal, minAdc, maxAdc );

  _lutAdc.setOrig( minAdc, maxAdc );
  _lutAdc.computeLut( lowerVal, upperVal );
  _adc.getResolution( _res2 );
  _dimXB = _adc.getSizeX();
  _dimYB = _adc.getSizeY();
  _dimZB = _adc.getSizeZ();
  
}


void gkg::VoiVolumeBaseFusion::initializeRgb()
{

  gkg::RGBComponent c = _rgb( 0 );
  float minRgb = float( c.r );
  float maxRgb = minRgb;

  _dimXB = _rgb.getSizeX();
  _dimYB = _rgb.getSizeY();
  _dimZB = _rgb.getSizeZ();

  if ( _rgb.isMemoryMapped() )
  {

    int32_t sizeT = _rgb.getSizeT();
    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < _dimZB; z++ )
      {

        for ( y = 0; y < _dimYB; y++ )
        {

          for ( x = 0; x < _dimXB; x++ )
          {

            c = _rgb( x, y, z, t );

            if ( float( c.r ) < minRgb )
            {

              minRgb = float( c.r );

            }
            else if ( float( c.r ) > maxRgb )
            {

              maxRgb = float( c.r );

            }
            if ( float( c.g ) < minRgb )
            {

              minRgb = float( c.g );

            }
            else if ( float( c.g ) > maxRgb )
            {

              maxRgb = float( c.g );

            }
            if ( float( c.b ) < minRgb )
            {

              minRgb = float( c.b );

            }
            else if ( float( c.b ) > maxRgb )
            {

              maxRgb = float( c.b );

            }

          }

        }

      }

    }

  }
  else
  {

    MinMaxRgbContext context( (uint8_t*)&_rgb( 0 ), minRgb, maxRgb );
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context, 0, 3 * _rgb.getSizeXYZT() );

    threadedLoop.launch();

  }

  _lutRgb.setOrig( minRgb, maxRgb );
  _lutRgb.computeLut( minRgb, maxRgb );
  _rgb.getResolution( _res2 );
  
}


float gkg::VoiVolumeBaseFusion::getMinT1()
{

  return _minT1;

}


float gkg::VoiVolumeBaseFusion::getMaxT1()
{

  return _maxT1;

}


gkg::RGBComponent gkg::VoiVolumeBaseFusion::getFusionColor( int32_t fusion,
                                                            int32_t x,
                                                            int32_t y,
                                                            int32_t z,
                                                            int32_t t )
{

  uint8_t val = _lutT1[ _t1( x, y, z, t ) ];
  uint8_t r = val, g = val, b = val;

  if ( _fusion == 1 )
  {

    gkg::Vector3d< float > p2;
    gkg::Vector3d< float > p1( x * _res.x, y * _res.y, z * _res.z );
    
    _trmVolBToT1.getInverse( p1, p2 );
   
    int32_t x2 = int32_t( p2.x / _res2.x + 0.5f );
    int32_t y2 = int32_t( p2.y / _res2.y + 0.5f );
    int32_t z2 = int32_t( p2.z / _res2.z + 0.5f );

    if ( ( x2 >= 0 ) && ( x2 < _dimXB ) && 
         ( y2 >= 0 ) && ( y2 < _dimYB ) &&
         ( z2 >= 0 ) && ( z2 < _dimZB ) )
    {

      uint8_t val2 = _lutAdc[ _adc( x2, y2, z2 ) ];

      r = (uint8_t)( ( fusion * val + 
                       ( 100 - fusion ) * redTemperature[ val2 ][ 0 ] ) / 100 );
      g = (uint8_t)( ( fusion * val + 
                       ( 100 - fusion ) * redTemperature[ val2 ][ 1 ] ) / 100 );
      b = (uint8_t)( ( fusion * val + 
                       ( 100 - fusion ) * redTemperature[ val2 ][ 2 ] ) / 100 );

    }

  }
  else if ( _fusion == 2 )
  {

    gkg::Vector3d< float > p2;
    gkg::Vector3d< float > p1( x * _res.x, y * _res.y, z * _res.z );
    
    _trmVolBToT1.getInverse( p1, p2 );
   
    int32_t x2 = int32_t( p2.x / _res2.x + 0.5f );
    int32_t y2 = int32_t( p2.y / _res2.y + 0.5f );
    int32_t z2 = int32_t( p2.z / _res2.z + 0.5f );

    if ( ( x2 >= 0 ) && ( x2 < _dimXB ) && 
         ( y2 >= 0 ) && ( y2 < _dimYB ) &&
         ( z2 >= 0 ) && ( z2 < _dimZB ) )
    {

      gkg::RGBComponent c = _rgb( x2, y2, z2 );
      uint8_t cr = _lutRgb[ c.r ];
      uint8_t cg = _lutRgb[ c.g ];
      uint8_t cb = _lutRgb[ c.b ];
      
      r = (uint8_t)( ( fusion * val + ( 100 - fusion ) * cr ) / 100 );
      g = (uint8_t)( ( fusion * val + ( 100 - fusion ) * cg ) / 100 );
      b = (uint8_t)( ( fusion * val + ( 100 - fusion ) * cb ) / 100 );
      
    }
  
  }

  return gkg::RGBComponent( r, g, b );

}
