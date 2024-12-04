#include <gkg-graphics-QtGL-plugin-functors/Voi/ContrastStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <cmath>


gkg::VoiContrastStrategy::VoiContrastStrategy() 
                        : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiContrastStrategy::id()
{ 

  return gkg::VoiModeStrategy::ContrastMode;
  
}


void gkg::VoiContrastStrategy::pressed( gkg::VoiDrawA&, gkg::VoiData& d, 
                                        gkg::Vector3d< int32_t >&, int32_t, 
			    	        QPoint& pt,
                                        Qt::KeyboardModifiers modifiers )
{

  if ( d.dataLoaded() )
  {

    VoiLutInfo& lut = d.lutInfo( modifiers & Qt::ShiftModifier );

    posClick = pt;
    scaling = lut.getScaling();
    minLut = scaling * lut.minLut();
    maxLut = scaling * lut.maxLut();
    origin = lut.getOrig();
    startW = (int32_t)( maxLut - minLut + 1 );
    startL = (int32_t)( ( minLut + maxLut ) / 2.0 );

  }

}


void gkg::VoiContrastStrategy::moved( gkg::VoiDrawA&, gkg::VoiData &d, 
                                      gkg::Vector3d< int32_t >&, int32_t, 
			              QPoint& pt, 
                                      Qt::KeyboardModifiers modifiers )
{

  if ( pt != posClick )
  {

    VoiLutInfo& lut = d.lutInfo( modifiers & Qt::ShiftModifier );
    double w = progress( startW, double( pt.x() - posClick.x() ) );
    double l = progress( startL, double( pt.y() - posClick.y() ) );

    if ( w < 0.0 )
    {

      w = 0.0;

    }

    if ( w > 65535.0 )
    {

      w = 65535.0;

    }

    if ( l < 0.0 )
    {

      l = 0.0;

    }

    if ( l > 65535.0 )
    {

      l = 65535.0;

    }

    float min = float( l - w / 2.0 );

    if ( min < 0.0f )
    {

      min = 0.0f;

    }

    float max = float( l + w / 2.0 );

    if ( max > 65535.0f )
    {

      max = 65535.0f;

    }

    lut.computeLut( min / scaling + origin, max / scaling + origin );
    d.notifyData();

  }

}


double gkg::VoiContrastStrategy::progress( double v, double d )
{

  double a = std::log( 3.0 ) / 150.0;

  if ( ( d <= 100.0 && d >= 0.0 ) || ( d < 0.0 && d >= -100.0 ) )
  {

    return v + 0.2 * d;

  }

  if ( d < -100.0 )
  {

    if ( v <= 20.0 )
    {

      return v - 20.0 + 10.0 * ( d + 100.0 );

    }

    return std::exp( a * ( d + 100.0 ) + std::log( v - 20.0 ) );

  }

  if ( d > 100.0 )
  {

    if ( v <= -20.0 )
    {

      return v + 20.0 + 10.0 * ( d - 100.0 );

    }

    return std::exp( a * ( d - 100.0 ) + std::log( v + 20.0 ) );

  }

  return v;

}
