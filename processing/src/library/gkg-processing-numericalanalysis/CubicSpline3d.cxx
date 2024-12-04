#include <gkg-processing-numericalanalysis/CubicSpline3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


#define CUBIC_SPLINE3D_INTEGRATION_PRECISION 1e-7


//
// class SplineSegment3dFunction< T >
//

template < class T >
gkg::SplineSegment3dFunction< T >::SplineSegment3dFunction( const T& A,
                                                            const T& B,
                                                            const T& C,
                                                            const T& D,
                                                            const T& E )
                                  : gkg::IntegrableFunction(),
                                    _A( A ),
                                    _B( B ),
                                    _C( C ),
                                    _D( D ),
                                    _E( E )
{
}


template < class T >
gkg::SplineSegment3dFunction< T >::~SplineSegment3dFunction()
{
}


template < class T >
double gkg::SplineSegment3dFunction< T >::getValue( double u ) const
{

  try
  {
  
    double t = u;
    double value = _E + _D * t;
  
    t *= u;
    value += _C * t;

    t *= u;
    value += _B * t;

    t *= u;
    value += _A * t;

    return value;

  }
  GKG_CATCH( "template < class T > "
             "double gkg::SplineSegmentFunction< T >::getValue( "
             "double u ) const" );

}


//
// class SplineSegment3d< T >
//

template < class T >
gkg::SplineSegment3d< T >::SplineSegment3d()
                          : _length( 0 )
{
}


template < class T >
gkg::SplineSegment3d< T >::SplineSegment3d( const gkg::Vector3d< T >& a, 
	                                    const gkg::Vector3d< T >& b,
	                                    const gkg::Vector3d< T >& c,
	                                    const gkg::Vector3d< T >& d )
                          : _a( a ),
                            _b( b ),
                            _c( c ),
                            _d( d )
{

  try
  {

    T A =  9 * _a.dot( _a );
    T B = 12 * _a.dot( _b );
    T C =  6 * _a.dot( _c ) + 4 * _b.dot( _b );
    T D =  4 * _b.dot( _c );
    T E =      _c.dot( _c );

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory = 
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    
    SplineSegment3dFunction< T > splineSegment3dFunction( A, B, C, D, E );
    _length = ( T )std::sqrt( factory->getQAGSIntegral( splineSegment3dFunction, 
                                       0,
                                       1,
                                       0,
                                       CUBIC_SPLINE3D_INTEGRATION_PRECISION ) );

  }
  GKG_CATCH( "template < class T > "
             "gkg::SplineSegment3d< T >::Spline3dSegment( "
             "const gkg::Vector3d< T >& a, "
	     "const gkg::Vector3d< T >& b, "
	     "const gkg::Vector3d< T >& c, "
	     "const gkg::Vector3d< T >& d )" );
       


}


template < class T >
gkg::SplineSegment3d< T >::SplineSegment3d(
                                        const gkg::SplineSegment3d< T >& other )
                          : _a( other._a ),
                            _b( other._b ),
                            _c( other._c ),
                            _d( other._d ),
                            _length( other._length )
                          
{
}


template < class T >
gkg::SplineSegment3d< T >::~SplineSegment3d()
{
}


template < class T >
gkg::SplineSegment3d< T >& 
gkg::SplineSegment3d< T >::operator=( const gkg::SplineSegment3d< T >& other )
{

  try
  {

    if ( &other != this )
    {

      _a = other._a;
      _b = other._b;
      _c = other._c;
      _d = other._d;
      _length = other._length;

    }
    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::SplineSegment3d< T >& "
             "gkg::SplineSegment3d< T >::operator=( "
             "const gkg::SplineSegment3d< T >& other )" );

}


template < class T >
const gkg::Vector3d< T >& gkg::SplineSegment3d< T >::getA() const
{

  return _a;

}


template < class T >
const gkg::Vector3d< T >& gkg::SplineSegment3d< T >::getB() const
{

  return _b;

}


template < class T >
const gkg::Vector3d< T >& gkg::SplineSegment3d< T >::getC() const
{

  return _c;

}


template < class T >
const gkg::Vector3d< T >& gkg::SplineSegment3d< T >::getD() const
{

  return _d;

}


template < class T >
const T& gkg::SplineSegment3d< T >::getLength() const
{

  return _length;

}


template < class T >
gkg::Vector3d< T > 
gkg::SplineSegment3d< T >::getValue( const T& normalizedArcLength ) const
{

  try
  {

    gkg::Vector3d< T > value = _d;

    T t = normalizedArcLength;
    value += _c * t;

    t *= normalizedArcLength;
    value += _b * t;

    t *= normalizedArcLength;
    value += _a * t;
           
    return value;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< T > gkg::SplineSegment3d< T >::getValue( "
             "const T& normalizedArcLength ) const" );

}


template < class T >
gkg::Vector3d< T > 
gkg::SplineSegment3d< T >::getTangent( const T& normalizedArcLength ) const
{

  try
  {

    gkg::Vector3d< T > tangent = _c;

    T t = normalizedArcLength;
    tangent += _b * t * ( T )2;

    t *= normalizedArcLength;
    tangent += _a * t * ( T )3;

    return tangent;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< T > gkg::SplineSegment3d< T >::getTangent( "
             "const T& normalizedArcLength ) const" );

}


template < class T >
T 
gkg::SplineSegment3d< T >::getCurvature( const T& normalizedArcLength ) const
{

  try
  {

    return ( _a * normalizedArcLength * ( T )6 + _b * ( T )2 ).getNorm();

  }
  GKG_CATCH( "template < class T > "
             "T gkg::SplineSegment3d< T >::getCurvature( "
             "const T& normalizedArcLength ) const" );

}


//
// class CubicSpline3d
//

template < class T >
gkg::CubicSpline3d< T >::CubicSpline3d(
                                    const gkg::LightCurve3d< T >& lightCurve3d )
{

  try
  {
     
    // processing curve tangents
    int32_t i, n = lightCurve3d.getPointCount();
    std::vector< gkg::Vector3d< T > > tangents( n );

    tangents[ 0 ] = lightCurve3d.getPoint( 1 ) - lightCurve3d.getPoint( 0 );
    n--;
    tangents[ n ] = lightCurve3d.getPoint( n ) - lightCurve3d.getPoint( n - 1 );

    for ( i = 1; i < n; i++ )
    {
      tangents[ i ] = ( lightCurve3d.getPoint( i + 1 ) -
                        lightCurve3d.getPoint( i - 1 ) ) * T( 0.5 );

    }

    this->processSegments( lightCurve3d.getPoints(), tangents );

  }
  GKG_CATCH( "template < class T > "
	     "gkg::CubicSpline3d< T >::CubicSpline3d( "
             "const gkg::LightCurve3d< T >& lightCurve3d )" );

}


template < class T >
gkg::CubicSpline3d< T >::CubicSpline3d(
                              const std::vector< Vector3d< T > >& controlPoints )
{

  try
  {
     
    // processing curve tangents
    int32_t i = 0;
    int32_t n = controlPoints.size();
    std::vector< gkg::Vector3d< T > > tangents( n );

    tangents[ 0 ] = controlPoints[ 1 ] - controlPoints[ 0 ];
    n--;
    tangents[ n ] = controlPoints[ n ] - controlPoints[ n - 1 ];

    for ( i = 1; i < n; i++ )
    {
      tangents[ i ] = ( controlPoints[ i + 1 ] -
                        controlPoints[ i - 1 ] ) * T( 0.5 );

    }

    this->processSegments( controlPoints, tangents );

  }
  GKG_CATCH( "template < class T > "
	     "gkg::CubicSpline3d< T >::CubicSpline3d( "
             "const std::vector< Vector3d< T > >& controlPoints )" );

}


template < class T >
gkg::CubicSpline3d< T >::CubicSpline3d()
{
}


template < class T >
gkg::CubicSpline3d< T >::~CubicSpline3d()
{
  
}


template < class T >
void gkg::CubicSpline3d< T >::processSegments(
                             const std::vector< Vector3d< T > >& controlPoints,
                             const std::vector< gkg::Vector3d< T > >& tangents )
{

  try
  {
     
    int32_t segmentCount = ( int32_t )controlPoints.size() - 1;
    _splineSegment3ds.resize( segmentCount );

    // processing first spline segment
    gkg::Vector3d< T > P0, P1;
    gkg::Vector3d< T > T0, T1;
    
    // processing spline segment(s)
    int32_t i;
    for ( i = 0; i < segmentCount; i++ )
    {

      P0 = controlPoints[ i ];
      P1 = controlPoints[ i + 1 ];
      T0 = tangents[ i ];
      T1 = tangents[ i + 1 ];

      _splineSegment3ds[ i ] = gkg::SplineSegment3d< T >(
                              P0 * ( T )2 - P1 * ( T )2 + T0 + T1,
                              P0 * ( T )( -3 ) + P1 * ( T )3 - T0 * ( T )2 - T1,
                              T0,
                              P0 );

    }

  }
  GKG_CATCH( "template < class T > "
	     "gkg::CubicSpline3d< T >::processSegments( "
             "const std::vector< Vector3d< T > >& controlPoints, "
             "const std::vector< gkg::Vector3d< T > >& tangents )" );

}


template < class T >
T gkg::CubicSpline3d< T >::getLength() const
{

  try
  {

    T length = 0;
    typename std::vector< gkg::SplineSegment3d< T > >::const_iterator
      s = _splineSegment3ds.begin(),
      se = _splineSegment3ds.end();
    while( s != se )
    {

      length += s->getLength();
      ++ s;

    }
    return length;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::CubicSpline3d< T >::getLength() const" );

}


template < class T >
int32_t gkg::CubicSpline3d< T >::getSplineSegment3dCount() const
{

  try
  {

    return ( int32_t )_splineSegment3ds.size();

  }
  GKG_CATCH( "template < class T > "
             "int32_t "
             "gkg::CubicSpline3d< T >::getSplineSegment3dCount() const" );

}


template < class T >
const gkg::SplineSegment3d< T >& 
gkg::CubicSpline3d< T >::getSplineSegment3d( int32_t index ) const
{

  try
  {


    if ( ( index < 0 ) ||
         ( index >= ( int32_t )_splineSegment3ds.size() ) )
    {

      throw std::runtime_error( "index not valid" );

    }
    return _splineSegment3ds[ index ];

  }
  GKG_CATCH( "template < class T > "
             "const gkg::SplineSegment3d< T >&  "
             "gkg::CubicSpline3d< T >::getSplineSegment3d( "
             "int32_t index ) const" );

}


template < class T >
const gkg::SplineSegment3d< T >& 
gkg::CubicSpline3d< T >::getSplineSegment3d( const T& arcLength,
                                             T& currentCurveLength ) const
{

  try
  {
     
    currentCurveLength = 0;
    typename std::vector< gkg::SplineSegment3d< T > >::const_iterator
      s = _splineSegment3ds.begin(),
      se = _splineSegment3ds.end();
    while ( ( s != se ) && ( currentCurveLength < arcLength ) )
    {

      currentCurveLength += s->getLength();
      ++ s;

    }
    if ( s == se )
    {

      if ( gkg::equal( currentCurveLength, arcLength,
                       ( T )( 2 * CUBIC_SPLINE3D_INTEGRATION_PRECISION ) ) )
      {

        -- s;
        currentCurveLength = arcLength - s->getLength();

      }
      else if ( arcLength < 0 )
      {

        throw std::runtime_error( "arc length cannot be negative" );

      }
      else
      {

        throw std::runtime_error( "arc length greater that spline length" );

      }

    }
    return *s;
  
  }
  GKG_CATCH( "template < class T > "
             "const gkg::SplineSegment3d< T >&  "
             "gkg::CubicSpline3d< T >::getSplineSegment3d( "
             "const T& arcLength ) const" );

}


template < class T >
gkg::Vector3d< T > 
gkg::CubicSpline3d< T >::getValue( const T& arcLength ) const
{

  try
  {

    T currentCurveLength = 0;
    const gkg::SplineSegment3d< T >&
      splineSegment3d = getSplineSegment3d( arcLength, currentCurveLength );
    return splineSegment3d.getValue( ( arcLength - currentCurveLength ) /
                                     splineSegment3d.getLength() );
  
  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< T > "
             "gkg::CubicSpline3d< T >::getValue( "
             "const T& arcLength ) const" );
            
}


template < class T >
gkg::Vector3d< T > 
gkg::CubicSpline3d< T >::getTangent( const T& arcLength ) const
{

  try
  {

    T currentCurveLength = 0;
    const gkg::SplineSegment3d< T >&
      splineSegment3d = getSplineSegment3d( arcLength, currentCurveLength );
    return splineSegment3d.getTangent( ( arcLength - currentCurveLength ) /
                                       splineSegment3d.getLength() );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< T > "
             "gkg::CubicSpline3d< T >::getTangent( "
             "const T& arcLength ) const" );
            
}


template < class T >
T 
gkg::CubicSpline3d< T >::getCurvature( const T& arcLength ) const
{

  try
  {

    T currentCurveLength = 0;
    const gkg::SplineSegment3d< T >&
      splineSegment3d = getSplineSegment3d( arcLength, currentCurveLength );
    return splineSegment3d.getCurvature( ( arcLength - currentCurveLength ) /
                                         splineSegment3d.getLength() );

  }
  GKG_CATCH( "template < class T > "
             "T gkg::CubicSpline3d< T >::getCurvature( "
             "const T& arcLength ) const");

}


template < class T >
void gkg::CubicSpline3d< T >::getInterpolatedCurve3d( 
                         std::vector< gkg::Vector3d< T > >& interpolatedCurve3d,
                         int32_t pointsPerSegment ) const
{

  try
  {

    int32_t i, n = (int32_t)_splineSegment3ds.size() * pointsPerSegment;
    T step = T( 1 ) / T( pointsPerSegment );
    typename std::vector< SplineSegment3d< T > >::const_iterator
      s = _splineSegment3ds.begin(),
      se = _splineSegment3ds.end();

    interpolatedCurve3d.clear();
    interpolatedCurve3d.reserve( n );

    while ( s != se )
    {

      T t = T( 0 );

      for ( i = 0; i < pointsPerSegment; i++ )
      {

        interpolatedCurve3d.push_back( s->getValue( t ) );
        t += step;

      }

      ++s;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CubicSpline3d< T >::getInterpolatedCurve3d( "
             "std::vector< gkg::Vector3d< T > >& interpolatedCurve3d, "
             "int32_t pointsPerSegment ) const" );

}


template < class T >
void gkg::CubicSpline3d< T >::getInterpolatedCurve3d( 
                                    gkg::LightCurve3d< T >& interpolatedCurve3d,
                                    int32_t pointsPerSegment ) const
{

  try
  {

    std::vector< gkg::Vector3d< T > > points;

    getInterpolatedCurve3d( points, pointsPerSegment );

    interpolatedCurve3d.clear();
    interpolatedCurve3d.addPoints( points );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CubicSpline3d< T >::getInterpolatedCurve3d( "
             "gkg::LightCurve3d< T >& interpolatedCurve3d, "
             "int32_t pointsPerSegment ) const" );

}


// 
// forcing instanciations
//

template class gkg::SplineSegment3dFunction< float >;
template class gkg::SplineSegment3dFunction< double >;

template class gkg::SplineSegment3d< float >;
template class gkg::SplineSegment3d< double >;

template class gkg::CubicSpline3d< float >;
template class gkg::CubicSpline3d< double >;
