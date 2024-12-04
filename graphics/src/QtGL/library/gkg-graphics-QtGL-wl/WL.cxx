#include <gkg-graphics-QtGL-wl/WL.h>
#include <gkg-core-cppext/Limits.h>
#include <cmath>
#include <cstring>


template < class T >
gkg::WL< T >::WL()
             : _offset( 0.0 ),
               _slope( 1.0 )
{

  T min = std::numeric_limits< T >::lowest();
  T max = std::numeric_limits< T >::max();

  _ww = double( max - min ) + 1.0;
  _wl = double( max + min ) / 2.0;
  _lut = new uint8_t[ size_t( max - min ) + 1 ];
  computeLut();

}


template < class T >
gkg::WL< T >::~WL()
{

  delete[] _lut;

}


template < class T >
void gkg::WL< T >::setRange( T min, T max )
{

  _ww = double( max - min );
  _wl = double( max + min ) / 2.0 + _offset;
  computeLut();

}


template < class T >
void gkg::WL< T >::setWW_WL( double ww, double wl )
{

  _ww = ww;
  _wl = wl;
  computeLut();

}


template < class T >
void gkg::WL< T >::setRescale( double offset, double slope )
{

  _offset = offset;
  _slope = slope;
  computeLut();

}


template < class T >
uint8_t gkg::WL< T >::getValue( T val )
{

  return _lut[ val ];

}


template < class T >
void gkg::WL< T >::mouseDown( int32_t x, int32_t y )
{

  _startX = x;
  _startY = y;
  _startWw = _ww;
  _startWl = ( _wl - _offset ) / _slope;

}


template < class T >
void gkg::WL< T >::mouseMove( int32_t x, int32_t y )
{

  _ww = progress( _startWw, double( x - _startX ) );
  _wl = progress( _startWl, double( y - _startY ) ) * _slope + _offset;

  T min = std::numeric_limits< T >::lowest();
  T max = std::numeric_limits< T >::max();

  if ( _ww < 0.0 )
  {

    _ww = 0.0;

  }

  if ( _ww > double( max - min ) )
  {

    _ww = double( max - min );

  }

  if ( _wl < double( min ) )
  {

    _wl = double( min );

  }

  if ( _wl > double( max ) )
  {

    _wl = double( max );

  }

  computeLut();

}


template < class T >
double gkg::WL< T >::progress( double val, double delta )
{

  double a = log( 3.0 ) / 150.0;

  if ( ( delta <= 100.0 && delta >= 0.0 ) || 
       ( delta < 0.0 && delta >= -100.0 ) )
  {

    return val + 0.2 * delta;

  }

  if ( delta < -100.0 )
    {

      if ( val <= 20.0 )
      {

	return val - 20.0 + 10.0 * ( delta + 100.0 );

      }

      return exp( a * ( delta + 100.0 ) + log( val - 20.0 ) );

    }

  if ( delta > 100.0 )
    {

      if ( val <= -20.0 )
      {

	return val + 20.0 + 10.0 * ( delta - 100.0 );

      }

      return exp( a * ( delta - 100.0 ) + log( val + 20.0 ) );

    }

  return val;

}


template < class T >
void gkg::WL< T >::computeLut()
{

  double a = 0.5, b = 0.0;
  T min = std::numeric_limits< T >::lowest();
  T max = std::numeric_limits< T >::max();
  int64_t i, size = int64_t( max - min );

  int64_t vmin = int64_t( ( _wl - _ww / 2.0 ) - _offset );
  int64_t vmax = int64_t( vmin + _ww );

  if ( vmin != vmax )
  {

    b = 255.0 / double( vmax - vmin );

  }

  if ( vmax < 0 )
  {

    vmin = vmax = 0;

  }
  else if ( vmin >= size )
  {

    vmin = vmax = size;

  }
  else
  {

    if ( vmin < 0 )
    {

      a -= b * vmin;
      vmin = 0;

    }

    if ( vmax > size )
    {

      vmax = size;

    }

  }

  std::memset( _lut, 0, vmin * sizeof( uint8_t ) );

  for ( i = vmin; i < vmax; i++, a += b )
  {

    _lut[ i ] = uint8_t( a );

  }

  std::memset( _lut + vmax, 255, ( size - vmax + 1 ) * sizeof( uint8_t ) );


}


template class gkg::WL< int8_t >;
template class gkg::WL< uint8_t >;
template class gkg::WL< int16_t >;
template class gkg::WL< uint16_t >;
