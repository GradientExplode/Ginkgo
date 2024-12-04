#include <gkg-processing-algobase/PowerOfTwo.h>
#include <cmath>


gkg::PowerOfTwo::PowerOfTwo()
{
}


gkg::PowerOfTwo::~PowerOfTwo()
{
}


template < class T >
int32_t gkg::PowerOfTwo::getLowerOrEqual( const T& value ) const
{

  int32_t greaterOrEqual = getGreaterOrEqual( value );

  if ( ( T )greaterOrEqual != value )
  {

    if ( greaterOrEqual > 1 )
    {

      return greaterOrEqual / 2;

    }
    else
    {

      return 1;

    }

  }
  else
  {

    return greaterOrEqual;

  }

}


template < class T >
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const T& value ) const
{

  return 1 << ( int32_t )std::ceil( std::log( ( double )value ) / 
                                    std::log( 2.0 ) );

}


template < class T >
int32_t gkg::PowerOfTwo::getStrictlyLower( const T& value ) const
{

  int32_t greaterOrEqual = getGreaterOrEqual( value );

  if ( greaterOrEqual > 1 )
  {

    return greaterOrEqual / 2;

  }
  else
  {

    return 1;

  }

}


template < class T >
int32_t gkg::PowerOfTwo::getStrictlyGreater( const T& value ) const
{

  int32_t greaterOrEqual = getGreaterOrEqual( value );

  if ( ( T )greaterOrEqual == value )
  {

    return greaterOrEqual * 2;

  }
  else
  {

    return greaterOrEqual;

  }

}


template < class T >
int32_t gkg::PowerOfTwo::getClosest( const T& value ) const
{

  int32_t greaterOrEqual = getGreaterOrEqual( value );
  int32_t lowerOrEqual = getLowerOrEqual( value );

  return ( std::fabs( ( double )greaterOrEqual - ( double )value ) <= 
           std::fabs( ( double )lowerOrEqual - ( double )value ) ) ?
         greaterOrEqual : lowerOrEqual;

}


template < class T >
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const T& value ) const
{

  int32_t lowerOrEqual = getLowerOrEqual( value );
  return ( int32_t )std::ceil( std::log( ( double )lowerOrEqual ) /
                               std::log( 2.0 ) );

}


template < class T >
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( const T& value ) const
{

  int32_t greaterOrEqual = getGreaterOrEqual( value );
  return ( int32_t )std::ceil( std::log( ( double )greaterOrEqual ) /
                               std::log( 2.0 ) );

}


template < class T >
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent( const T& value ) const
{

  int32_t striclyLower = getStrictlyLower( value );
  return ( int32_t )std::ceil( std::log( ( double )striclyLower ) /
                               std::log( 2.0 ) );

}


template < class T >
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent( const T& value ) const
{

  int32_t striclyGreater = getStrictlyGreater( value );
  return ( int32_t )std::ceil( std::log( ( double )striclyGreater ) /
                               std::log( 2.0 ) );

}


template < class T >
int32_t gkg::PowerOfTwo::getClosestExponent( const T& value ) const
{

  int32_t closest = getClosest( value );
  return ( int32_t )std::ceil( std::log( ( double )closest ) /
                               std::log( 2.0 ) );

}


// value

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const int16_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const uint16_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const int32_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const uint32_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const int64_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const uint64_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const float& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqual( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqual( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLower( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreater( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosest( const double& value ) const;

// exponent

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( 
                                                   const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent( const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent( 
                                                   const int16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const int16_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( 
                                                  const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent(
                                                  const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent(
                                                  const uint16_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const uint16_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( 
                                                   const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent( const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent( 
                                                   const int32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const int32_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent(
                                                  const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent(
                                                  const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent(
                                                  const uint32_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const uint32_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( 
                                                   const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent( const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent( 
                                                   const int64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const int64_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent(
                                                  const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent(
                                                  const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent(
                                                  const uint64_t& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const uint64_t& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent( const float& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const float& value ) const;

template 
int32_t gkg::PowerOfTwo::getLowerOrEqualExponent( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getGreaterOrEqualExponent( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyLowerExponent( const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getStrictlyGreaterExponent( 
                                                    const double& value ) const;
template 
int32_t gkg::PowerOfTwo::getClosestExponent( const double& value ) const;

