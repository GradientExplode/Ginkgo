#include <gkg-graphics-X11-base/ScaleDivider.h>
#include <algorithm>
#include <cmath>


template < class T >
const T gkg::ScaleDivider< T >::StepEpsilon = ( T )1.0e-3;
template < class T >
const T gkg::ScaleDivider< T >::BorderEpsilon = ( T )1.0e-10;


template < class T >
gkg::ScaleDivider< T >::ScaleDivider( const T& lowerBound,
                                      const T& upperBound,
                                      const T& step )
                       : gkg::RCGUIObject(),
                         _lowerBound( std::min( lowerBound, upperBound ) ),
                         _upperBound( std::max( lowerBound, upperBound ) ),
                         _majorStep( ( T )std::fabs( ( double )step ) )
{
}


template < class T >
gkg::ScaleDivider< T >::~ScaleDivider()
{
}


template < class T >
const T& gkg::ScaleDivider< T >::getLowerBound() const
{

  return _lowerBound;

}


template < class T >
const T& gkg::ScaleDivider< T >::getUpperBound() const
{

  return _upperBound;

}


template < class T >
const T& gkg::ScaleDivider< T >::getMajorStep() const
{

  return _majorStep;

}


template < class T >
int32_t gkg::ScaleDivider< T >::getMajorCount() const
{

  return ( int32_t )_majorMarks.size();

}


template < class T >
const T& gkg::ScaleDivider< T >::getMajorMark( int32_t index ) const
{

  return _majorMarks[ index ];

}


template < class T >
int32_t gkg::ScaleDivider< T >::getMinorCount() const
{

  return ( int32_t )_minorMarks.size();

}


template < class T >
const T& gkg::ScaleDivider< T >::getMinorMark( int32_t index ) const
{

  return _minorMarks[ index ];

}


template < class T >
bool gkg::ScaleDivider< T >::limitRange( T& value,
                                         const T& value1,
                                         const T& value2,
                                         const T& epsilonRel,
                                         const T& epsilonAbs ) const
{

  bool result = true;
  T lowerValue = std::min( value1, value2 ); 
  T upperValue = std::max( value1, value2 ); 
  T lowerDelta = std::max( ( T )std::fabs( epsilonRel * lowerValue ),
                           ( T )std::fabs( epsilonAbs ) );
  T upperDelta = std::max( ( T )std::fabs( epsilonRel * upperValue ),
                           ( T )std::fabs( epsilonAbs ) );
  if ( value < lowerValue )
  {

    if ( value < lowerValue - lowerDelta )
    {

      result = false;

    }
    value = lowerValue;

  }
  else if ( value > upperValue )
  {

    if ( value > upperValue + upperDelta )
    {

      result = false;

    }
    value = upperValue;

  }
  return result;

}


template class gkg::ScaleDivider< float >;
template class gkg::ScaleDivider< double >;
