#include <gkg-graphics-QtGL-plugin-functors/Voi/LutInfo.h>


gkg::VoiLutInfo::VoiLutInfo()
               : lutMin( 0.0f ),
                 lutMax( 255.0f ),
                 origMin( 0.0f ),
                 origMax( 255.0f ),
                 scaling( 257.0f )
{

  lut16.resize( gkg::VoiLutInfo::nLut );
  computeLut( lutMin, lutMax );

}


gkg::VoiLutInfo::~VoiLutInfo()
{
}


std::vector< uint8_t >& gkg::VoiLutInfo::lut()
{ 

  return lut16;
  
}


const std::vector< uint8_t >& gkg::VoiLutInfo::lut() const 
{ 

  return lut16;
  
}


float gkg::VoiLutInfo::minLut()
{

  return lutMin;
  
}


float gkg::VoiLutInfo::maxLut()
{

  return lutMax;

}


float gkg::VoiLutInfo::getScaling()
{

  return scaling;

}


void gkg::VoiLutInfo::setOrig( float omin, float omax )
{ 

  origMin = omin;
  origMax = omax;
  scaling = 65535.0f / ( omax - omin );
  
}


float gkg::VoiLutInfo::getOrig()
{ 

  return origMin;

}


void gkg::VoiLutInfo::computeLut( float lmin, float lmax )
{

  lutMin = lmin - origMin;
  lutMax = lmax - origMin;

  int32_t i;
  int32_t l1 = (int32_t)( scaling * lutMin );
  int32_t l2 = (int32_t)( scaling * lutMax );

  for ( i = 0; i < l1; i++ )
  {
  
    lut16[ i ] = 0;
    
  }
  
  for ( i = l2; i < gkg::VoiLutInfo::nLut; i++ )
  {
  
    lut16[ i ] = 255;
    
  }
  
  for ( i = l1; i < l2; i++ ) 
  {
  
    lut16[ i ] = (uint8_t)( 255.0 * (float)( i - l1 ) / (float)( l2 - l1 ) );
    
  }

}


uint8_t gkg::VoiLutInfo::operator [] ( float n ) 
{

  return lut16[ (long)( (float)( scaling * ( n - origMin ) ) ) ];
  
}
