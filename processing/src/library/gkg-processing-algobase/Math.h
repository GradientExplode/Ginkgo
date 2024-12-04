#ifndef _gkg_processing_algobase_Math_h_
#define _gkg_processing_algobase_Math_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <algorithm>
#include <cmath>


namespace gkg
{


template < class T >
inline
const T& max( const T& a, const T& b, const T& c, const T& d )
{

  return std::max( std::max( a, b ), std::max( c, d ) );

}


template < class T >
inline
const T& min( const T& a, const T& b, const T& c, const T& d )
{

  return std::min( std::min( a, b ), std::min( c, d ) );

}


template < class T >
inline
T middle( const T& a, const T& b )
{

  return ( a + b ) / 2;

}


inline
bool equal( float x, float y, float epsilon )
{

  return ( ( x - y ) < epsilon ) && ( ( y - x ) < epsilon );

}


inline
bool equal( double x, double y, double epsilon )
{

  return ( ( x - y ) < epsilon ) && ( ( y - x ) < epsilon );

}


inline
int32_t floor( float x )
{

  return ( ( x ) > 0.0 ? ( int32_t )( x ):
                          ( ( ( int32_t )( x ) - x ) != 0.0 ?
                            ( int32_t )( x ) - 1 :
                            ( int32_t )( x ) ) );

}


inline
int32_t floor( double x )
{

  return ( ( x ) > 0.0 ? ( int32_t )( x ):
                          ( ( ( int32_t )( x ) - x ) != 0.0 ?
                            ( int32_t )( x ) - 1 :
                            ( int32_t )( x ) ) );

}


inline
int32_t round( float x )
{

  return x > 0 ? ( int32_t )( x + 0.5 ) : -( int32_t )( -x + 0.5 );

}


inline
int32_t round( double x )
{

  return x > 0 ? ( int32_t )( x + 0.5 ) : -( int32_t )( -x + 0.5 );

}


inline
int32_t ceil( float x )
{

  return -floor( -x );

}


inline
int32_t ceil( double x )
{

  return -floor( -x );

}


inline
int32_t abs( int32_t x )
{

  return ::abs(x);


}


inline
long abs( long x )
{

   return x >= 0 ? x : -x;

}


inline
double abs( double x )
{

  return std::fabs(x);

}


inline 
float safeAcos( float dot )
{

  if ( dot < -1.0 )
  {

    return M_PI;

  }
  else if ( dot > +1.0 )
  {

    return 0;

  }
  return ( float )std::acos( ( double )dot );

}


inline 
double safeAcos( double dot )
{

  if ( dot < -1.0 )
  {

    return M_PI;

  }
  else if ( dot > +1.0 )
  {

    return 0;

  }
  return std::acos( dot );

}


inline 
float safeAsin( float value )
{

  if ( value < -1.0 )
  {

    return ( float )( -M_PI / 2.0 );

  }
  else if ( value > +1.0 )
  {

    return ( float )( +M_PI / 2.0 );

  }
  return ( float )std::asin( ( double )value );

}


inline 
double safeAsin( double value )
{

  if ( value < -1.0 )
  {

    return -M_PI / 2.0;

  }
  else if ( value > +1.0 )
  {

    return +M_PI / 2.0;

  }
  return std::asin( value );

}


inline
double getVectorAngles( const Vector3d< double >& v1,
                        const Vector3d< double >& v2 )
{

  return safeAcos( v1.dot( v2 ) / ( v1.getNorm() * v2.getNorm() ) );

}


inline
float getVectorAngles( const Vector3d< float >& v1,
                       const Vector3d< float >& v2 )
{

  return ( float )safeAcos( v1.dot( v2 ) / ( v1.getNorm() * v2.getNorm() ) );

}


inline
double getLineAngles( const Vector3d< double >& v1,
                      const Vector3d< double >& v2 )
{

  double dotProduct = v1.dot( v2 ) / ( v1.getNorm() * v2.getNorm() );
  return std::fabs( std::asin(
                std::sqrt( std::max( 0.0, 1.0 - dotProduct * dotProduct ) ) ) );

}


inline
float getLineAngles( const Vector3d< float >& v1,
                     const Vector3d< float >& v2 )
{

  double dotProduct = ( float )v1.dot( v2 ) / ( v1.getNorm() * v2.getNorm() );
  return ( float )std::fabs( std::asin( std::sqrt(
                           std::max( 0.0, 1.0 - dotProduct * dotProduct ) ) ) );

}


// find the smallest value out of {1,2,5}*10^n with an integer number n
// which is greater than or equal to x
double ceil125( double x );



float getPointToTriangleDistanceAndProjectionPoint(
                                           const Vector3d< float >& point,
                                           const Vector3d< float >& vertex1,
                                           const Vector3d< float >& vertex2,
                                           const Vector3d< float >& vertex3,
                                           Vector3d< float >& projectionPoint );

double getPointToTriangleDistanceAndProjectionPoint(
                                          const Vector3d< double >& point,
                                          const Vector3d< double >& vertex1,
                                          const Vector3d< double >& vertex2,
                                          const Vector3d< double >& vertex3,
                                          Vector3d< double >& projectionPoint );

float getPointToTriangleDistance( const Vector3d< float >& point,
                                  const Vector3d< float >& vertex1,
                                  const Vector3d< float >& vertex2,
                                  const Vector3d< float >& vertex3 );

double getPointToTriangleDistance( const Vector3d< double >& point,
                                   const Vector3d< double >& vertex1,
                                   const Vector3d< double >& vertex2,
                                   const Vector3d< double >& vertex3 );

float getPointToSegmentDistance( const Vector3d< float >& point,
                                 const Vector3d< float >& endPoint1,
                                 const Vector3d< float >& endPoint2 );

double getPointToSegmentDistance( const Vector3d< double >& point,
                                  const Vector3d< double >& endPoint1,
                                  const Vector3d< double >& endPoint2 );

float getPartialTriangleAreas( const Vector3d< float >& projectPoint,
                               const Vector3d< float >& vertex1,
                               const Vector3d< float >& vertex2,
                               const Vector3d< float >& vertex3,
                               float& area1,
                               float& area2,
                               float& area3 );

double getPartialTriangleAreas( const Vector3d< double >& projectPoint,
                                const Vector3d< double >& vertex1,
                                const Vector3d< double >& vertex2,
                                const Vector3d< double >& vertex3,
                                double& area1,
                                double& area2,
                                double& area3 );

float getTriangleArea( const gkg::Vector3d< float >& site1,
                       const gkg::Vector3d< float >& site2,
                       const gkg::Vector3d< float >& site3 );

double getTriangleArea( const gkg::Vector3d< double >& site1,
                        const gkg::Vector3d< double >& site2,
                        const gkg::Vector3d< double >& site3 );

}


#endif
