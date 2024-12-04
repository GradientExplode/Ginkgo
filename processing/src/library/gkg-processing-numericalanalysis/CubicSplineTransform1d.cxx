#include <gkg-processing-numericalanalysis/CubicSplineTransform1d.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::CubicSplineTransform1d::CubicSplineTransform1d( const gkg::Vector& data )
                            : _coefficients( data.getSize() ),
                              _size( data.getSize() ),
                              _sizeMinusOne( data.getSize() - 1 ),
                              _sizeMinusOneTimes2( 2 * ( data.getSize() - 1 ) )

{

  try
  {

    // constants
    const double z1 = -0.26794919243112;  // -2 + sqrt( 3 )
    const double cz1 = 0.28867513459481;  // z1 / ( z1^2 - 1 )

    // initial value for the causal recursion using a mirror symmetric boundary
    // condition
    // cp(0) = ( 1/2 - z1^(2N-2) ) sum_{k=0}^{2N-3} data(k) z1^k data(k),
    // with: data(N) = data(N-2), data(N+1) = data(N-3), ...,
    // data(2N-3) = data(1).
    double cp = data( 0 );
    double z1powK = 1.0;

    int32_t k;
    for ( k = 1; k < _size; k++ )
    {

      z1powK = z1powK * z1;
      cp += data( k ) * z1powK;

    }

    // at this point, we have: z1powK = z1^(N-1)
    for ( k = 2; k < _size; k++ )
    {
    
      z1powK = z1powK * z1;
      cp += data( _size - k ) * z1powK;

    }

    // at this point, we have: z1powK = z1^(2N-3)
    z1powK = z1powK * z1;
    cp = cp / ( 1 - z1powK );

    // storing the first causal coefficient
    _coefficients( 0 ) = cp;

    // performing the causal recursion
    for ( k = 1; k < _size; k++ )
    {

      cp = data( k ) + z1 * cp;
      _coefficients( k ) = cp;

    }

    // initial value for the anticausal recursion
    double cm = cz1 * ( 2.0 * cp - _coefficients( _size - 1 ) );
    _coefficients( _size - 1 ) = 6.0 * cm;

    // performing the anticausal recursion
    for ( k = ( _size - 2 ); k >= 0; k-- )
    {

      cm = z1 * ( cm - _coefficients( k ) );
      _coefficients( k ) = 6.0 * cm;

    }

  }
  GKG_CATCH( "gkg::CubicSplineTransform1d::CubicSplineTransform1d( "
             "const gkg::Vector& data )" );

}


gkg::CubicSplineTransform1d::~CubicSplineTransform1d()
{
}


double gkg::CubicSplineTransform1d::getCoefficient( int32_t t ) const
{

  try
  {

    return _coefficients( t );

  }
  GKG_CATCH( "double gkg::CubicSplineTransform1d::getCoefficient( "
             "int32_t t ) const" );

}

double gkg::CubicSplineTransform1d::getValue( double time ) const
{

  try
  {

    double rightUpSuperiorPoint = time + _sizeMinusOne;
    if ( ( rightUpSuperiorPoint < 0 ) ||
         ( rightUpSuperiorPoint > 3 * _sizeMinusOne ) )
    {

      return 0.0;

    }

    int32_t positiveTime = ( int32_t )( rightUpSuperiorPoint + 2 ) -
                           _sizeMinusOne;
    int32_t negativeTime = positiveTime - 3;

    // processing the B-spline values and the B-spline coefficient indices
    // including mirror conditions
    double bSplineValues[ 4 ];
    int32_t bSplineIndices[ 4 ];
    int32_t t, i = 0;
    for ( t = negativeTime; t <= positiveTime; t++ )
    {

      bSplineValues[ i ] = cubicSplineBasis( time - ( double )t );
      bSplineIndices[ i ] = cubicSplineMirror( t );
      ++ i;

    }

    // computing the interpolated value incrementally
    double signal = 0.0;
    for ( t = negativeTime; t <= positiveTime; t++ )
    {
    
      // pointing towards the coefficient value at position t
      signal += _coefficients( bSplineIndices[ t ] ) * bSplineValues[ t ];
    
    }

    return signal;

  }
  GKG_CATCH( "double gkg::CubicSplineTransform1d::getValue( double t ) const" );


}


double gkg::CubicSplineTransform1d::cubicSplineBasis( double time )
{

  try
  {

    double absTime = std::fabs( time );

    if ( absTime >= 2 )
    {

      return 0.0;

    }

    double tmp, value;
    if ( absTime < 1 )
    {

      tmp = absTime * absTime;
      value = 0.66666666666667 - tmp + 0.5 * absTime * tmp;

    }
    else
    {

      tmp = 2 - absTime;
      value = tmp * tmp * tmp / 6.0;

    }
 
    return value;

  }
  GKG_CATCH( "double gkg::CubicSplineTransform1d::cubicSplineBasis( "
             "double time ) const" );

}


int32_t gkg::CubicSplineTransform1d::cubicSplineMirror( int32_t t ) const
{

  try
  {

    return ( ( t ) < 0.0 ? ( -( t ) ) :
                           ( ( t ) > ( _sizeMinusOne ) ?
                             ( ( _sizeMinusOneTimes2 ) - ( t ) ) : ( t ) ) );

  }
  GKG_CATCH( "int32_t gkg::CubicSplineTransform1d::cubicSplineMirror( "
             "int32_t t ) const" );

}

