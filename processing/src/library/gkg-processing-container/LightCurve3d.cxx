#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-processing-numericalanalysis/CubicSpline3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>

#ifdef GKG_DEBUG
#include <cstdio>
#endif

#define INFINITE_TORTUOSITY   1e37
#define ALMOST_NULL_DISTANCE  1e-10


template < class T >
gkg::LightCurve3d< T >::LightCurve3d()
                       : _closed( false )
{
}


template < class T >
gkg::LightCurve3d< T >::LightCurve3d( int32_t pointCount )
                       : _points( pointCount ),
                         _closed( false )
{
}


template < class T > gkg::LightCurve3d< T >::LightCurve3d( const std::list<
gkg::Vector3d< T > >& points, bool closed ) : _closed( closed ) {

  try
  {

    addPoints( points );

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T >::LightCurve3d( "
             "const std::list< gkg::Vector3d< T > >& points, "
             "bool closed )" );

}


template < class T >
gkg::LightCurve3d< T >::LightCurve3d(
                                const std::vector< gkg::Vector3d< T > >& points,
                                bool closed )
                       : _closed( closed )
{

  try
  {

    addPoints( points );

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T >::LightCurve3d( "
             "const std::vector< gkg::Vector3d< T > >& points, "
             "bool closed )" );

}


template < class T >
gkg::LightCurve3d< T >::LightCurve3d( const gkg::Matrix& matrix,
                                      bool closed )
                       : _closed( closed )
{

  try
  {

    // sanity checks: only regular and homogeneous matrices are accepted
    if ( ( matrix.getSize2() != 3 ) && ( matrix.getSize2() != 4 ) )
    {

      throw std::runtime_error( "not a valid matrix columns count" );

    }

    addPoints( matrix );

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T >::LightCurve3d( "
             "const gkg::Matrix& matrix, "
             "bool closed )" );

}


template < class T >
gkg::LightCurve3d< T >::LightCurve3d( const gkg::LightCurve3d< T >& other )
                       :  _points( other._points ),
                          _closed( other._closed )
{
}


template < class T >
gkg::LightCurve3d< T >::~LightCurve3d()
{
}


template < class T >
gkg::LightCurve3d< T >& 
gkg::LightCurve3d< T >::operator=( const gkg::LightCurve3d< T >& other )
{

  try
  {

    _points = other._points;
    _closed = other._closed;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T >& "
             "gkg::LightCurve3d< T >::operator=( "
             "const gkg::LightCurve3d< T >& other )" );

}


template < class T >
void gkg::LightCurve3d< T >::reserve( int32_t pointCount )
{

  try
  {

    this->_points.reserve( pointCount );

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::reserve( int32_t pointCount )" );

}


template < class T >
typename gkg::LightCurve3d< T >::iterator
gkg::LightCurve3d< T >::begin()
{

  return this->_points.begin();

}


template < class T >
typename gkg::LightCurve3d< T >::iterator
gkg::LightCurve3d< T >::end()
{

  return this->_points.end();

}


template < class T >
typename gkg::LightCurve3d< T >::const_iterator
gkg::LightCurve3d< T >::begin() const
{

  return this->_points.begin();

}


template < class T >
typename gkg::LightCurve3d< T >::const_iterator
gkg::LightCurve3d< T >::end() const
{

  return this->_points.end();

}


template < class T >
typename gkg::LightCurve3d< T >::reverse_iterator
gkg::LightCurve3d< T >::rbegin()
{

  return this->_points.rbegin();

}


template < class T >
typename gkg::LightCurve3d< T >::reverse_iterator
gkg::LightCurve3d< T >::rend()
{

  return this->_points.rend();

}


template < class T >
typename gkg::LightCurve3d< T >::const_reverse_iterator
gkg::LightCurve3d< T >::rbegin() const
{

  return this->_points.rbegin();

}


template < class T >
typename gkg::LightCurve3d< T >::const_reverse_iterator 
gkg::LightCurve3d< T >::rend() const
{

  return this->_points.rend();

}


template < class T >
void gkg::LightCurve3d< T >::addPoint( const gkg::Vector3d< T >& point )
{

  try
  {

    this->_points.push_back( point );

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::addPoint( "
             "const gkg::Vector3d< T >& point )" );

}


template < class T >
void gkg::LightCurve3d< T >::setPoint( int32_t index,
                                       const gkg::Vector3d< T >& point )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= this->getPointCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    this->_points[ index ] = point;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::LightCurve3d< T >::setPoint( "
             "int32_t index, const Vector3d< T >& point )" );

}


template < class T >
void 
gkg::LightCurve3d< T >::addPoints(
                                 const std::list< gkg::Vector3d< T > >& points )
{

  try
  {

    this->_points.insert( this->_points.end(), points.begin(), points.end() );

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::addPoints( "
             "const std::list< gkg::Vector3d< T > >& points )" );

}


template < class T >
void 
gkg::LightCurve3d< T >::addPoints(
                               const std::vector< gkg::Vector3d< T > >& points )
{

  try
  {

    this->_points.insert( this->_points.end(), points.begin(), points.end() );

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::addPoints( "
             "const std::list< gkg::Vector3d< T > >& points )" );

}


template < class T >
void 
gkg::LightCurve3d< T >::addPoints( const gkg::Matrix& matrix )
{

  try
  {

    int32_t p, pointCount = matrix.getSize1();
    for ( p = 0; p < pointCount; p++ )
    {

      this->_points.push_back( gkg::Vector3d< T >( ( T )matrix( p, 0 ),
                                                   ( T )matrix( p, 1 ),
                                                   ( T )matrix( p, 2 ) ) );

    }

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::addPoints( "
             "const gkg::Matrix& matrix )" );

}


template < class T >
void gkg::LightCurve3d< T >::setEquidistantCurve( int32_t pointCount )
{

  try
  {

    if ( pointCount < 2 )
    {

      throw std::runtime_error( "target point count should be at least two" );

    }

    // collecting old and new point count(s)
    int32_t oldPointCount = ( int32_t )this->_points.size();
    if ( oldPointCount < 2 )
    {

      throw std::runtime_error( "origin point count should be at least two" );

    }


    int32_t newPointCount = pointCount;

    // allocating memory for old and new curve length(s)
    std::vector< T > oldCurveLength( oldPointCount, ( T )0 );
    std::vector< T > newCurveLength( newPointCount, ( T )0 );

    // processing old curve length for each old point
    int32_t p;
    oldCurveLength[ 0 ] = 0;
    for ( p = 1; p < oldPointCount; p++ )
    {

      oldCurveLength[ p ] = oldCurveLength[ p - 1 ] + 
                            ( this->_points[ p ] -
                              this->_points[ p - 1 ] ).getNorm();

    }

    // taking the curve length
    T curveLength = oldCurveLength[ oldPointCount - 1 ];

    // processing old curve length for each new point
    for ( p = 0; p < newPointCount; p++ )
    {

      newCurveLength[ p ] = ( T )p * curveLength / ( T )( newPointCount - 1 );

    }

    // allocating new equidistant point vector
    std::vector<gkg::Vector3d< T > > newPoints( newPointCount );

    // processing new point(s)
    newPoints[ 0 ] = _points[ 0 ];
    newPoints[ newPointCount - 1 ] = _points[ oldPointCount - 1 ];

    int32_t n;
    for ( n = 1; n < newPointCount - 1; n++ )
    {

      for ( p = 1; p < oldPointCount; p++ )
      {

        if ( oldCurveLength[ p ] > newCurveLength[ n ] )
        {

          break;

        }

      }
      newPoints[ n ] = ( this->_points[ p - 1 ] *
                         ( oldCurveLength[ p ] - newCurveLength[ n ] ) +
                         this->_points[ p ] *
                         ( newCurveLength[ n ] - oldCurveLength[ p - 1 ] ) ) /
                       ( oldCurveLength[ p ] - oldCurveLength[ p - 1 ] );

    }

    // creating new equidistant curve
    this->_points = newPoints;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::LightCurve3d< T >::setEquidistantCurve( "
             "int32_t pointCount )" );

}




template < class T >
void gkg::LightCurve3d< T >::removePoint( const int32_t& index )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= this->getPointCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    typename std::vector< gkg::Vector3d< T > >::iterator 
      p = this->_points.begin();
    p += index;

    // erasing point at given index
    this->_points.erase( p );

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::removePoint( const int32_t& index )" );

}


template < class T >
void gkg::LightCurve3d< T >::removePoints( const std::set< int32_t >& indices )
{

  try
  {

#ifdef GKG_DEBUG

    std::set< int32_t >::const_iterator index = indices.begin(),
                                        indexe = indices.end();
    while ( index != indexe )
    {

      if ( ( *index < 0 ) || ( *index >= this->getPointCount() ) )
      {

        char message[ 256 ];
        sprintf( message, "invalid index ( %d )", *index );
        throw std::runtime_error( message );

      }
      ++ index;

    }

#endif

    std::vector< gkg::Vector3d< T > >
      newPoints( _points.size() - indices.size() );

    int32_t i, size = ( int32_t )_points.size();
    int32_t count = 0;

    std::set< int32_t >::const_iterator ie = indices.end();

    // building new point vector
    for ( i = 0; i < size; i++ )
    {

      if ( indices.find( i ) != ie )
      {

        newPoints[ count ] = this->_points[ i ];
        ++ count;

      }

    }

    // copying back new point vector
    this->_points = newPoints;

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::removePoints( "
             "const std::set< int32_t >& indices )" );

}


template < class T >
void gkg::LightCurve3d< T >::clear()
{

  try
  {

    // erasing point(s)
    this->_points.clear();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::LightCurve3d< T >::clear()" );

}


template < class T >
const gkg::Vector3d< T >& 
gkg::LightCurve3d< T >::getPoint( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= this->getPointCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    return this->_points[ index ];

  }
  GKG_CATCH( "template < class T > "
             "const gkg::Vector3d< T >& "
             "gkg::LightCurve3d< T >::getPoint( int32_t index ) const" );

}


template < class T >
const std::vector< gkg::Vector3d< T > >& 
gkg::LightCurve3d< T >::getPoints() const
{

  return this->_points;

}


template < class T >
int32_t gkg::LightCurve3d< T >::getPointCount() const
{

  return ( int32_t )this->_points.size();

}


template < class T >
void gkg::LightCurve3d< T >::setClosed()
{

  try
  {

    _closed = true;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::LightCurve3d< T >::setClosed()" );

}


template < class T >
bool gkg::LightCurve3d< T >::isClosed() const
{

  return this->_closed;

}


template < class T >
T gkg::LightCurve3d< T >::getLength() const
{

  try
  {

    typename std::vector< gkg::Vector3d< T > >::const_iterator
      p = this->_points.begin(),
      pe = this->_points.end();

    typename std::vector< gkg::Vector3d< T > >::const_iterator
      nextP = p + 1;

    T length = 0.0;
    if( p != pe )
    {

      while ( nextP != pe )
      {

       length += ( *nextP - *p ).getNorm();

        ++ p;
        ++ nextP;

      }
    }
    return length;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::getLength() const" );

}


template < class T >
T gkg::LightCurve3d< T >::getLength( int32_t fromIndex, int32_t toIndex ) const
{

  try
  {

    int32_t from = fromIndex;
    int32_t to = toIndex;
    if ( from > to )
    {

      std::swap( from, to );

    }
    typename std::vector< gkg::Vector3d< T > >::const_iterator
      p = this->_points.begin() + from,
      pe = this->_points.begin() + to + 1;

    typename std::vector< gkg::Vector3d< T > >::const_iterator
      nextP = p + 1;

    T length = 0.0;
    while ( nextP != pe )
    {

      length += ( *nextP - *p ).getNorm();

      ++ p;
      ++ nextP;

    }
    return length;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::getLength( "
             "int32_t fromIndex, int32_t toIndex ) const" );

}


template < class T >
std::vector< T > gkg::LightCurve3d< T >::getCurvatureProfile() const
{

  try
  {

    gkg::CubicSpline3d< T > spline3d( *this );
    int32_t splineSegment3dCount = spline3d.getSplineSegment3dCount();

    std::vector< T > curvatureProfile( splineSegment3dCount + 1 );
    int32_t s = 0;
    for ( s = 0; s < splineSegment3dCount; s++ )
    {

      curvatureProfile[ s ] =
        spline3d.getSplineSegment3d( s ).getCurvature( 0 );

      if ( s == splineSegment3dCount - 1 )
      {

        curvatureProfile[ s + 1 ] =
          spline3d.getSplineSegment3d( s ).getCurvature( 1 );

      }

    }

    return curvatureProfile;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< T > "
             "gkg::LightCurve3d< T >::getCurvatureProfile() const" );

}


template < class T >
std::vector< gkg::Vector3d< T > >
gkg::LightCurve3d< T >::getDirectionProfile() const
{

  try
  {

    // collecting curve point count
    int32_t pointCount = this->getPointCount();

    // allocating the direction profile vector
    std::vector< gkg::Vector3d< T > > directionProfile( pointCount );

    // 
    if ( pointCount == 2 )
    {

      directionProfile[ 0 ] = ( _points[ 1 ] - _points[ 0 ] ).normalize();
      directionProfile[ 1 ] = directionProfile[ 0 ];

    }
    else
    {


      directionProfile[ 0 ] = ( _points[ 1 ] - _points[ 0 ] ).normalize();

      int32_t p = 0;
      for ( p = 1; p < pointCount - 1; p++ )
      {

        directionProfile[ p ] =
                            ( _points[ p + 1 ] - _points[ p - 1 ] ).normalize();

      }

      directionProfile[ pointCount - 1 ] = 
          ( _points[ pointCount - 1 ] - _points[ pointCount - 2 ] ).normalize();

    }

    return directionProfile;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< gkg::Vector3d< T > > "
             "gkg::LightCurve3d< T >::getDirectionProfile() const" );

}


template < class T >
T gkg::LightCurve3d< T >::getTortuosity() const
{

  try
  {

    // tortuosity = length of the curve / distance between ends

    T distanceBetweenEnds =
                       ( this->_points[ 0 ] -
                         this->_points[ this->_points.size() - 1U ] ).getNorm();

    if ( distanceBetweenEnds < ALMOST_NULL_DISTANCE )
    {

      return INFINITE_TORTUOSITY;

    }
    return this->getLength() / distanceBetweenEnds;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::getTortuosity() const" );

}


template < class T >
gkg::Vector3d< T > gkg::LightCurve3d< T >::getCenterOfMass() const
{

  try
  {

    gkg::Vector3d< T > centerOfMass( 0.0, 0.0, 0.0 );

    typename std::vector< gkg::Vector3d< T > >::const_iterator
      p = this->_points.begin(),
      pe = this->_points.end();
    while ( p != pe )
    {

      centerOfMass += *p;
      ++ p;

    }
    centerOfMass /= ( T )this->getPointCount();

    return centerOfMass;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< T > gkg::LightCurve3d< T >::getCenterOfMass() const" );

}


template < class T >
gkg::LightCurve3d< T > 
gkg::LightCurve3d< T >::getEquidistantCurve( int32_t pointCount ) const
{

  try
  {

    if ( pointCount < 2 )
    {

      throw std::runtime_error( "target point count should be at least two" );

    }

    // collecting old and new point count(s)
    int32_t oldPointCount = ( int32_t )this->_points.size();
    if ( oldPointCount < 2 )
    {

      throw std::runtime_error( "origin point count should be at least two" );

    }

    int32_t newPointCount = pointCount;

    // allocating memory for old and new curve length(s)
    std::vector< T > oldCurveLength( oldPointCount, ( T )0 );
    std::vector< T > newCurveLength( newPointCount, ( T )0 );

    // processing old curve length for each old point
    int32_t p;
    oldCurveLength[ 0 ] = 0;
    for ( p = 1; p < oldPointCount; p++ )
    {

      oldCurveLength[ p ] = oldCurveLength[ p - 1 ] + 
                            ( this->_points[ p ] -
                              this->_points[ p - 1 ] ).getNorm();

    }

    // taking the curve length
    T curveLength = oldCurveLength[ oldPointCount - 1 ];

    // processing old curve length for each new point
    for ( p = 0; p < newPointCount; p++ )
    {

      newCurveLength[ p ] = ( T )p * curveLength / ( T )( newPointCount - 1 );

    }

    // allocating new equidistant point vector
    std::vector<gkg::Vector3d< T > > newPoints( newPointCount );

    // processing new point(s)
    newPoints[ 0 ] = _points[ 0 ];
    newPoints[ newPointCount - 1 ] = _points[ oldPointCount - 1 ];

    int32_t n;
    for ( n = 1; n < newPointCount - 1; n++ )
    {

      for ( p = 1; p < oldPointCount; p++ )
      {

        if ( oldCurveLength[ p ] > newCurveLength[ n ] )
        {

          break;

        }

      }
      newPoints[ n ] = ( this->_points[ p - 1 ] *
                         ( oldCurveLength[ p ] - newCurveLength[ n ] ) +
                         this->_points[ p ] *
                         ( newCurveLength[ n ] - oldCurveLength[ p - 1 ] ) ) /
                       ( oldCurveLength[ p ] - oldCurveLength[ p - 1 ] );

    }

    // creating new equidistant curve
    gkg::LightCurve3d< T > equidistantCurve;
    equidistantCurve.reserve( newPointCount );
    equidistantCurve.addPoints( newPoints );
    
    return equidistantCurve;

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T > "
             "gkg::LightCurve3d< T >::getEquidistantCurve( "
             "int32_t pointCount ) const" );

}


template < class T >
gkg::LightCurve3d< T > 
gkg::LightCurve3d< T >::getEquidistantCurvePart( int32_t fromIndex,
                                                 int32_t toIndex,
                                                 T maximumResamplingStep ) const
{

  try
  {

    // creating new equidistant curve
    gkg::LightCurve3d< T > equidistantCurve;

    if ( fromIndex <= toIndex )
    {

      // collecting old and new point count(s)
      int32_t oldPointCount = toIndex - fromIndex + 1;
      T length = this->getLength( fromIndex, toIndex );
      int32_t newPointCount = std::max( 2, ( int32_t )( length /
                                                        maximumResamplingStep +
                                                        0.5 ) );

      // allocating memory for old and new curve length(s)
      std::vector< T > oldCurveLength( oldPointCount, ( T )0 );
      std::vector< T > newCurveLength( newPointCount, ( T )0 );

      // processing old curve length for each old point
      int32_t p;
      int32_t pPrime;
      oldCurveLength[ 0 ] = 0;
      for ( p = 1, pPrime = fromIndex + 1; p < oldPointCount; p++, pPrime++ )
      {


        oldCurveLength[ p ] = oldCurveLength[ p - 1 ] + 
                              ( this->_points[ pPrime ] -
                                this->_points[ pPrime - 1 ] ).getNorm();

      }

      // taking the curve length
      T curveLength = oldCurveLength[ oldPointCount - 1 ];

      // processing old curve length for each new point
      for ( p = 0; p < newPointCount; p++ )
      {

        newCurveLength[ p ] = ( T )p * curveLength / ( T )( newPointCount - 1 );

      }

      // allocating new equidistant point vector
      std::vector<gkg::Vector3d< T > > newPoints( newPointCount );

      // processing new point(s)
      newPoints[ 0 ] = _points[ fromIndex ];
      newPoints[ newPointCount - 1 ] = _points[ toIndex ];

      int32_t n;
      for ( n = 1; n < newPointCount - 1; n++ )
      {

        for ( p = 1, pPrime = fromIndex + 1; p < oldPointCount; p++, pPrime++ )
        {

          if ( oldCurveLength[ p ] > newCurveLength[ n ] )
          {

            break;

          }

        }
        newPoints[ n ] = ( this->_points[ pPrime - 1 ] *
                           ( oldCurveLength[ p ] - newCurveLength[ n ] ) +
                           this->_points[ pPrime ] *
                           ( newCurveLength[ n ] - oldCurveLength[ p - 1 ] ) ) /
                         ( oldCurveLength[ p ] - oldCurveLength[ p - 1 ] );

      }

      // filling new equidistant curve
      equidistantCurve.reserve( newPointCount );
      equidistantCurve.addPoints( newPoints );
    
    }
    else
    {

      // collecting old and new point count(s)
      int32_t oldPointCount = fromIndex - toIndex + 1;
      T length = this->getLength( fromIndex, toIndex );
      int32_t newPointCount = std::max( 2, ( int32_t )( length /
                                                        maximumResamplingStep +
                                                        0.5 ) );

      // allocating memory for old and new curve length(s)
      std::vector< T > oldCurveLength( oldPointCount, ( T )0 );
      std::vector< T > newCurveLength( newPointCount, ( T )0 );

      // processing old curve length for each old point
      int32_t p;
      int32_t pPrime;
      oldCurveLength[ 0 ] = 0;
      for ( p = 1, pPrime = fromIndex - 1; p < oldPointCount; p++, pPrime-- )
      {


        oldCurveLength[ p ] = oldCurveLength[ p - 1 ] + 
                              ( this->_points[ pPrime ] -
                                this->_points[ pPrime + 1 ] ).getNorm();

      }

      // taking the curve length
      T curveLength = oldCurveLength[ oldPointCount - 1 ];

      // processing old curve length for each new point
      for ( p = 0; p < newPointCount; p++ )
      {

        newCurveLength[ p ] = ( T )p * curveLength / ( T )( newPointCount - 1 );

      }

      // allocating new equidistant point vector
      std::vector<gkg::Vector3d< T > > newPoints( newPointCount );

      // processing new point(s)
      newPoints[ 0 ] = _points[ fromIndex ];
      newPoints[ newPointCount - 1 ] = _points[ toIndex ];

      int32_t n;
      for ( n = 1; n < newPointCount - 1; n++ )
      {

        for ( p = 1, pPrime = fromIndex - 1; p < oldPointCount; p++, pPrime-- )
        {

          if ( oldCurveLength[ p ] > newCurveLength[ n ] )
          {

            break;

          }

        }
        newPoints[ n ] = ( this->_points[ pPrime + 1 ] *
                           ( oldCurveLength[ p ] - newCurveLength[ n ] ) +
                           this->_points[ pPrime ] *
                           ( newCurveLength[ n ] - oldCurveLength[ p - 1 ] ) ) /
                         ( oldCurveLength[ p ] - oldCurveLength[ p - 1 ] );

      }

      // filling new equidistant curve
      equidistantCurve.reserve( newPointCount );
      equidistantCurve.addPoints( newPoints );

    }

    return equidistantCurve;

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T > "
             "gkg::LightCurve3d< T >::getEquidistantCurvePart( "
             "int32_t fromIndex, "
             "int32_t toIndex, "
             "T maximumResamplingStep ) const" );

}


template < class T >
gkg::LightCurve3d< T > 
gkg::LightCurve3d< T >::getCurvePart( int32_t fromIndex, int32_t toIndex ) const
{

  try
  {

    // creating new equidistant curve
    gkg::LightCurve3d< T > curvePart;

    std::vector< gkg::Vector3d< float > > points;
    int32_t p = 0;
    if ( fromIndex <= toIndex )
    {

      for ( p = fromIndex; p <= toIndex; p++ )
      {

        curvePart.addPoint( this->_points[ p ] );

      }

    }
    else
    {

      for ( p = toIndex; p <= fromIndex; p++ )
      {

        curvePart.addPoint( this->_points[ p ] );

      }

    }

    return curvePart;

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T > "
             "gkg::LightCurve3d< T >::getCurvePart( "
             "int32_t fromIndex, "
             "int32_t toIndex ) const" );

}


template < class T >
T gkg::LightCurve3d< T >::getDistance2(
                                     const gkg::LightCurve3d< T >& other ) const
{

  try
  {

    if ( other.getPointCount() != getPointCount() )
    {

      throw std::runtime_error( "not the same point count" );

    }

    T distance2 = 0.0;
    typename gkg::LightCurve3d< T >::const_iterator
      p1 = begin(),
      p1e = end();
    typename gkg::LightCurve3d< T >::const_iterator
      p2 = other.begin();

    while ( p1 != p1e )
    {

      distance2 += ( *p1 - *p2 ).getNorm2();
      ++ p1;
      ++ p2;

    }
    return distance2;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::getDistance2( "
             "const gkg::LightCurve3d< T >& other ) const" );

}


template < class T >
T gkg::LightCurve3d< T >::getMaximumDistance2(
                                     const gkg::LightCurve3d< T >& other ) const
{

  try
  {

    if ( other.getPointCount() != getPointCount() )
    {

      throw std::runtime_error( "not the same point count" );

    }

    T distance2 = 0.0;
    T firstMaximumDistance2 = 0.0;
    typename gkg::LightCurve3d< T >::const_iterator
      p1 = begin(),
      p1e = end();
    typename gkg::LightCurve3d< T >::const_iterator
      p2 = other.begin();

    while ( p1 != p1e )
    {

      distance2 += ( *p1 - *p2 ).getNorm2();
      if ( distance2 > firstMaximumDistance2 )
      {

        firstMaximumDistance2 = distance2;

      }
      ++ p1;
      ++ p2;

    }

    T secondMaximumDistance2 = 0.0;
    p1 = begin();
    typename gkg::LightCurve3d< T >::const_reverse_iterator
      p2r = other.rbegin();

    while ( p1 != p1e )
    {

      distance2 += ( *p1 - *p2r ).getNorm2();
      if ( distance2 > secondMaximumDistance2 )
      {

        secondMaximumDistance2 = distance2;

      }
      ++ p1;
      ++ p2r;

    }


    return std::min( firstMaximumDistance2, secondMaximumDistance2 );

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::getMaximumDistance2( "
             "const gkg::LightCurve3d< T >& other ) const" );

}


template < class T >
T gkg::LightCurve3d< T >::getDistance(
                       const LightCurve3d< T >& other,
                       gkg::LightCurve3d< T >::DistanceType distanceType,
                       const std::vector< double >* furtherParameters ) const
{

  try
  {


    T distance = ( T )0.0;

    switch ( distanceType )
    {

      case gkg::LightCurve3d< T >::NonSymmetricMeanClosestPointDistance:

        {

          if ( furtherParameters )
          {

            if ( !furtherParameters->empty() )
            {

              throw std::runtime_error( "no further parameters required" );

            }

          }

          typename gkg::LightCurve3d< T >::const_iterator
            p1 = begin(),
            p1e = end();
          typename gkg::LightCurve3d< T >::const_iterator
            p2 = other.begin(),
            p2e = other.end();

          T meanDistance = 0.0;
          T minimumDistance2 = 0.0;
          T distance2 = 0.0;

          T dx = 0.0;
          T dy = 0.0;
          T dz = 0.0;

          while ( p1 != p1e )
          {

            p2 = other.begin();
            dx = p1->x - p2->x;
            dy = p1->y - p2->y;
            dz = p1->z - p2->z;
            minimumDistance2 = dx * dx + dy * dy + dz * dz;

            ++ p2;
            while ( p2 != p2e )
            {

              dx = p1->x - p2->x;
              dy = p1->y - p2->y;
              dz = p1->z - p2->z;
              distance2 = dx * dx + dy * dy + dz * dz;
              if ( distance2 < minimumDistance2 )
              {

                minimumDistance2 = distance2;

              }
              ++ p2;

            }
            meanDistance += std::sqrt( minimumDistance2 );

            ++ p1;

          }

          distance = meanDistance / ( T )this->getPointCount();

        }
        break;

      case gkg::LightCurve3d< T >::SymmetricMeanOfMeanClosestPointDistance:

        {

          distance =
            ( this->getDistance(
                other,
                gkg::LightCurve3d< T >::NonSymmetricMeanClosestPointDistance,
                furtherParameters ) +
              other.getDistance(
                *this,
                gkg::LightCurve3d< T >::NonSymmetricMeanClosestPointDistance,
                furtherParameters ) ) /
            2.0;

        }
        break;


      case gkg::LightCurve3d< T >::SymmetricMaximumOfMeanClosestPointDistance:

        {

          distance =
            std::max(
              this->getDistance(
                other,
                gkg::LightCurve3d< T >::NonSymmetricMeanClosestPointDistance,
                furtherParameters ),
              other.getDistance(
                *this,
                gkg::LightCurve3d< T >::NonSymmetricMeanClosestPointDistance,
                furtherParameters ) );

        }
        break;

    }

    return distance;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::getDistance( "
             "const LightCurve3d< T >& other, "
             "gkg::LightCurve3d< T >::DistanceType distanceType, "
             "onst std::vector< double >* furtherParameters ) const" );


}


template < class T >
gkg::Matrix gkg::LightCurve3d< T >::toMatrix() const
{

  try
  {

    if ( !getPointCount() )
    {

       throw std::runtime_error( "not points in the curve" );

    }

    gkg::Matrix matrix( getPointCount(), 3 );
    typename std::vector< gkg::Vector3d< T > >::const_iterator
      p = this->_points.begin(),
      pe = this->_points.end();
    int32_t pointCount = 0;
    while ( p != pe )
    {

      matrix( pointCount, 0 ) = p->x;
      matrix( pointCount, 1 ) = p->y;
      matrix( pointCount, 2 ) = p->z;

      ++ p;
      ++ pointCount;

    }
    return matrix;

  }
  GKG_CATCH( "template < class T > "
             "Matrix gkg::LightCurve3d< T >::toMatrix() const" );

}


template < class T >
gkg::Matrix gkg::LightCurve3d< T >::toHomogeneousMatrix() const
{

  try
  {

    if ( !getPointCount() )
    {

       throw std::runtime_error( "not points in the curve" );

    }

    gkg::Matrix homogeneousMatrix( getPointCount(), 4 );
    typename std::vector< gkg::Vector3d< T > >::const_iterator
      p = this->_points.begin(),
      pe = this->_points.end();
    int32_t pointCount = 0;
    while ( p != pe )
    {

      homogeneousMatrix( pointCount, 0 ) = p->x;
      homogeneousMatrix( pointCount, 1 ) = p->y;
      homogeneousMatrix( pointCount, 2 ) = p->z;
      homogeneousMatrix( pointCount, 3 ) = 1.0;

      ++ p;
      ++ pointCount;

    }
    return homogeneousMatrix;

  }
  GKG_CATCH( "template < class T > "
             "Matrix gkg::LightCurve3d< T >::toHomogeneousMatrix() const" );

}


template < class T >
void gkg::LightCurve3d< T >::reverse()
{

  try
  {

    std::reverse( _points.begin(), _points.end() );

  }
  GKG_CATCH( "template < class T > void gkg::LightCurve3d< T >::reverse()" );

}


template < class T >
T gkg::LightCurve3d< T >::addTortuosity(
                            T magnitude,
                            T angularDispersionInDegrees,
                            T waveLength,
                            const gkg::Vector3d< T >& referenceCurveOrientation,
                            const gkg::BoundingBox< T >& fieldOfView,
                            int32_t excludedExtremityPointCount,
                            int32_t maximumIterationCount )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////

    int32_t pointCount = ( int32_t )_points.size();
    if ( pointCount < 3 )
    {

      throw std::runtime_error( "point count should be at least 3" );

    }

    if ( excludedExtremityPointCount < 0 )
    {

      throw std::runtime_error( "excluded point count must be positive" );

    }

    if ( 2 * excludedExtremityPointCount >= pointCount )
    {

      throw std::runtime_error( "excluded point count is two large with respect"
                                " to the curve pint count" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // converting target angular dispersion in radians
    ////////////////////////////////////////////////////////////////////////////

    T angularDispersionInRadians = angularDispersionInDegrees * M_PI / 180.0;


    ////////////////////////////////////////////////////////////////////////////
    // pointing to the num. analysis factory and creating a random generator
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* 
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );


    ////////////////////////////////////////////////////////////////////////////
    // computing the neighborhood size
    ////////////////////////////////////////////////////////////////////////////

    T curveResolution = ( _points[ 1 ] - _points[ 0 ] ).getNorm();
    int32_t neighborhoodSize = ( int32_t )( ( 2.0f * waveLength ) / 
                                            curveResolution );

    int32_t iterationCount = 0;

    T inducedAngularDispersionInRadians = 0.0f;

    int32_t selectedPointIndex = 0;
    int32_t lowerPointIndex = 0;
    int32_t upperPointIndex = 0;
    int32_t lowerStartingPointIndex = 0;
    int32_t upperStartingPointIndex = 0;
    T oldAngularDispersionInRadians = 0.0;
    T newAngularDispersionInRadians = 0.0;
    int32_t p = 0;
    gkg::Vector3d< T > firstAxis;
    gkg::Vector3d< T > secondAxis;
    gkg::Vector3d< T > thirdAxis;
    T displacementAlongSecondAxis = 0.0f;
    T displacementAlongThirdAxis = 0.0f;
    double offset = 0.0f;
    bool exitingFieldOfView = false;
    gkg::Vector3d< T > displacement;
    int32_t neighborPointIndex = 0;
    int32_t displacementIndex = 0;
    while ( ( inducedAngularDispersionInRadians < 
              angularDispersionInRadians ) &&
            ( iterationCount < maximumIterationCount ) )
    {

      // randomly selecting a point 
      //// a modifier pour mettre les limites du tirage dans le constructeur
      selectedPointIndex = ( int32_t )factory->getUniformRandomUInt32(
                                              randomGenerator,
                                              ( uint32_t )( pointCount / 2 ) ) +
                           pointCount / 4;

      // computing lower and upper bounds of point indices to be moved
      lowerPointIndex = std::max( selectedPointIndex - neighborhoodSize,
                                  excludedExtremityPointCount );
      upperPointIndex = std::min( selectedPointIndex + neighborhoodSize,
                                  pointCount - 1 -
                                  excludedExtremityPointCount );

      // computing lower and upper indices of all impacted point(s)
      lowerStartingPointIndex = std::max( lowerPointIndex - 1,
                                          excludedExtremityPointCount );
      upperStartingPointIndex = upperPointIndex;
      if ( upperPointIndex == pointCount - 1 - excludedExtremityPointCount )
      {

        -- upperStartingPointIndex;

      }

      // computing the old angular deviation of the selected curve portion
      oldAngularDispersionInRadians = 0.0f;
      for ( p = lowerStartingPointIndex; p < upperStartingPointIndex; p++ )
      {

        oldAngularDispersionInRadians += gkg::getLineAngles(
                                                _points[ p + 1 ] - _points[ p ],
                                                referenceCurveOrientation );
      }

      // computing the orthonormal trieder at the selected point
      if ( selectedPointIndex < pointCount - 1 )
      {

        firstAxis = _points[ selectedPointIndex + 1 ] -
                    _points[ selectedPointIndex ];
        firstAxis.normalize();

      }
      else 
      {

        firstAxis = _points[ selectedPointIndex ] -
                    _points[ selectedPointIndex - 1 ];
        firstAxis.normalize();

      }
      gkg::getRandomOrthonormalTriederFromFirstAxis( firstAxis,
                                                     secondAxis,
                                                     thirdAxis );

      // computing displacement along the second and third axis
      displacementAlongSecondAxis =
                         ( T )factory->getGaussianRandomNumber( randomGenerator,
                                                                0.0,
                                                                magnitude );
      displacementAlongThirdAxis =
                         ( T )factory->getGaussianRandomNumber( randomGenerator,
                                                                0.0,
                                                                magnitude );

      
      // computing local deformation
      exitingFieldOfView = false;

      std::vector< gkg::Vector3d< T > > 
        displacements( upperPointIndex - lowerPointIndex + 1 );

      displacementIndex = 0;
      for ( neighborPointIndex = lowerPointIndex;
            neighborPointIndex <= upperPointIndex;
            neighborPointIndex++ )
      {

        const gkg::Vector3d< T >&
          neighborPoint = _points[ neighborPointIndex ];

        offset = ( double )std::abs( neighborPointIndex - 
                                     selectedPointIndex );
        displacement = ( secondAxis * displacementAlongSecondAxis +
                         thirdAxis * displacementAlongThirdAxis ) *
                       ( T )std::exp( -( offset * offset ) /
                                     ( double )neighborhoodSize );

        if ( !fieldOfView.contains( neighborPoint + displacement ) )
        {

          exitingFieldOfView = true;
          break;

        }
        displacements[ displacementIndex ] = displacement;
        ++ displacementIndex;

      }

      if ( !exitingFieldOfView )
      {

        displacementIndex = 0;
        for ( neighborPointIndex = lowerPointIndex;
              neighborPointIndex <= upperPointIndex;
              neighborPointIndex ++ )
        {

          // moving each sphere
          _points[ neighborPointIndex ] += displacements[ displacementIndex ];
          ++ displacementIndex;

        }

        // computing the new angular deviation of the selected curve portion
        newAngularDispersionInRadians = 0.0f;
        for ( p = lowerStartingPointIndex; p < upperStartingPointIndex; p++ )
        {

          newAngularDispersionInRadians += gkg::getLineAngles(
                                                _points[ p + 1 ] - _points[ p ],
                                                referenceCurveOrientation );
        }

        inducedAngularDispersionInRadians = 
                                     std::abs( newAngularDispersionInRadians -
                                               oldAngularDispersionInRadians ) /
                                                     pointCount;
      }

      ++ iterationCount;

    }

    return inducedAngularDispersionInRadians * 180.0 / M_PI;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::LightCurve3d< T >::addTortuosity( "
             "T magnitude, "
             "T angularDispersionInDegrees, "
             "T waveLength, "
             "const gkg::Vector3d< T >& referenceCurveOrientation, "
             "const gkg::BoundingBox< T >& fieldOfView, "
             "int32_t excludedExtremityPointCount, "
             "int32_t maximumIterationCount )" );

}


template < class T >
template < class V >
void gkg::LightCurve3d< T >::getScalarVolumeProfile(
               const gkg::Volume< V >& scalarVolume,
               const gkg::Transform3d< T >& transform3dFromScalarVolumeToVolume,
               std::vector< V >& scalarVolumeProfile,
               const V& backgroundValue,
               int32_t rank,
               bool usePartialVolumingInterpolation ) const
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // reallocating scalar profile
    ////////////////////////////////////////////////////////////////////////////

    if ( scalarVolumeProfile.size() != _points.size() )
    {

      scalarVolumeProfile.resize( _points.size() );

    }

    ////////////////////////////////////////////////////////////////////////////
    // collecting the scalar volume resolution and bounding box
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    scalarVolume.getResolution( resolution );

    gkg::BoundingBox< int32_t > boundingBox( scalarVolume );


    ////////////////////////////////////////////////////////////////////////////
    // collecting the scalar volume resolution and bounding box
    ////////////////////////////////////////////////////////////////////////////

    const gkg::QuickResampler< V >* quickResampler = 0;

    if ( usePartialVolumingInterpolation )
    {

      quickResampler = &gkg::PartialVolumingQuickResampler< V >::getInstance();

    }

    ////////////////////////////////////////////////////////////////////////////
    // looping over 3D curve points
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< T > pointInScalarVolume;
    gkg::Vector3d< int32_t > voxelInScalarVolume;

    typename gkg::LightCurve3d< T >::const_iterator p = this->begin(),
                                                    pe = this->end();
    typename std::vector< V >::iterator v = scalarVolumeProfile.begin();
    while ( p != pe )
    {

      transform3dFromScalarVolumeToVolume.getInverse( *p, pointInScalarVolume );

      if ( !usePartialVolumingInterpolation )
      {

        voxelInScalarVolume.x = ( int32_t )( pointInScalarVolume.x /
                                             resolution.x );
        voxelInScalarVolume.y = ( int32_t )( pointInScalarVolume.y /
                                             resolution.y );
        voxelInScalarVolume.z = ( int32_t )( pointInScalarVolume.z /
                                             resolution.z );

        if ( boundingBox.contains( voxelInScalarVolume ) )
        {

          *v = scalarVolume( voxelInScalarVolume, rank );

        }

      }
      else
      {

        quickResampler->resample( scalarVolume,
                                  backgroundValue,
                                  pointInScalarVolume,
                                  *v,
                                  &resolution,
                                  rank );

      }
      ++ v;
      ++ p;

    }

  }
  GKG_CATCH( "template < class T > "
             "template < class V > "
             "void gkg::LightCurve3d< T >::getScalarVolumeProfile( "
             "const gkg::Volume< V >& scalarVolume, "
             "const gkg::Transform3d< T >& transform3dFromCurveToVolume, "
             "std::vector< V >& scalarVolumeProfile, "
             "const V& backgroundValue, "
             "int32_t rank, "
             "bool usePartialVolumingInterpolation ) const" );

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::LightCurve3d< float >& thing )
{

  os << "{ ";

  std::vector< gkg::Vector3d< float > >::const_iterator
    p = thing.getPoints().begin(),
    pe = thing.getPoints().end();
  -- pe;

  while ( p != pe )
  {

    os << *p << "; ";
    ++ p;

  }

  os << *p << " }";
  return os;

}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::LightCurve3d< double >& thing )
{

  os << "{ ";

  std::vector< gkg::Vector3d< double > >::const_iterator
    p = thing.getPoints().begin(),
    pe = thing.getPoints().end();
  -- pe;

  while ( p != pe )
  {

    os << *p << "; ";
    ++ p;

  }

  os << *p << ( thing.isClosed() ? "closed" : "opened" ) << " }";
  return os;

}


// 
// forcing instanciations
//

template class gkg::LightCurve3d< float >;
template class gkg::LightCurve3d< double >;

template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< int8_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< int8_t >& scalarVolumeProfile,
                  const int8_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< uint8_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< uint8_t >& scalarVolumeProfile,
                  const uint8_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< int16_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< int16_t >& scalarVolumeProfile,
                  const int16_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< uint16_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< uint16_t >& scalarVolumeProfile,
                  const uint16_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< int32_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< int32_t >& scalarVolumeProfile,
                  const int32_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< uint32_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< uint32_t >& scalarVolumeProfile,
                  const uint32_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< int64_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< int64_t >& scalarVolumeProfile,
                  const int64_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< uint64_t >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< uint64_t >& scalarVolumeProfile,
                  const uint64_t& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< float >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< float >& scalarVolumeProfile,
                  const float& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< float >::getScalarVolumeProfile(
                  const gkg::Volume< double >& scalarVolume,
                  const gkg::Transform3d< float >& transform3dFromCurveToVolume,
                  std::vector< double >& scalarVolumeProfile,
                  const double& backgroundValue,
                  int32_t rank,
                  bool usePartialVolumingInterpolation ) const;


template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< int8_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< int8_t >& scalarVolumeProfile,
                 const int8_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< uint8_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< uint8_t >& scalarVolumeProfile,
                 const uint8_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< int16_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< int16_t >& scalarVolumeProfile,
                 const int16_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< uint16_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< uint16_t >& scalarVolumeProfile,
                 const uint16_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< int32_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< int32_t >& scalarVolumeProfile,
                 const int32_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< uint32_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< uint32_t >& scalarVolumeProfile,
                 const uint32_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< int64_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< int64_t >& scalarVolumeProfile,
                 const int64_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< uint64_t >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< uint64_t >& scalarVolumeProfile,
                 const uint64_t& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< float >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< float >& scalarVolumeProfile,
                 const float& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;
template void gkg::LightCurve3d< double >::getScalarVolumeProfile(
                 const gkg::Volume< double >& scalarVolume,
                 const gkg::Transform3d< double >& transform3dFromCurveToVolume,
                 std::vector< double >& scalarVolumeProfile,
                 const double& backgroundValue,
                 int32_t rank,
                 bool usePartialVolumingInterpolation ) const;



#undef INFINITE_TORTUOSITY
#undef ALMOST_NULL_DISTANCE
