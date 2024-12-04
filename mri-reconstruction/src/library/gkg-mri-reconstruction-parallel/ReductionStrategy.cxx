#include <gkg-mri-reconstruction-parallel/ReductionStrategy.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class T >
gkg::ReductionStrategy< T >::ReductionStrategy( const T& reductionFactor )
                            : _reductionFactor( reductionFactor )
{

  try
  {

    if ( _reductionFactor < 1.0 )
    {

      throw std::runtime_error( "wrong reduction factor" );

    }

  }
  GKG_CATCH( "gkg::ReductionStrategy::ReductionStrategy( "
             "const T& reductionFactor )" );

}


template < class T >
gkg::ReductionStrategy< T >::ReductionStrategy(
                                      const gkg::ReductionStrategy< T >& other )
                            : _reductionFactor( other._reductionFactor )
                              
{
}


template < class T >
gkg::ReductionStrategy< T >::~ReductionStrategy()
{
}


template < class T >
gkg::ReductionStrategy< T >& gkg::ReductionStrategy< T >::operator=( 
                                      const gkg::ReductionStrategy< T >& other )
{

  _reductionFactor = other._reductionFactor;
  return *this;

}


template < class T >
const T& gkg::ReductionStrategy< T >::getReductionFactor() const
{

  return _reductionFactor;

}


template < class T >
int32_t gkg::ReductionStrategy< T >::getUnfoldedPositionCount() const
{

  return ( int32_t )_reductionFactor;

}


template < class T >
void gkg::ReductionStrategy< T >::getUnfoldedPositions(
                                     const T& foldedPosition,
                                     int32_t fullSize,
                                     std::vector< T >& unfoldedPositions ) const
{

  try
  {

    if ( ( int32_t )unfoldedPositions.size() != ( int32_t )_reductionFactor )
    {

      unfoldedPositions = std::vector< T >(  ( int32_t )_reductionFactor );

    }
    if ( ( int32_t )_reductionFactor == 1 )
    {

      unfoldedPositions[ 0 ] = foldedPosition;

    }
    else
    {

      size_t p;
      for ( p = 0; p < unfoldedPositions.size(); p++ )
      {

        unfoldedPositions[ p ] = std::fmod( foldedPosition + p * ( T )fullSize /
                                            _reductionFactor,
                                            ( T )fullSize ) -
                                 ( T )fullSize / ( 2 * _reductionFactor );

        if ( unfoldedPositions[ p ] < 0 )
        {

          unfoldedPositions[ p ] += ( T )fullSize;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ReductionStrategy::getUnfoldedPositions( "
             "const T& foldedPosition, "
             "int fullSize, "
             "std::vector< T >& unfoldedPositions ) const" );

}



template class gkg::ReductionStrategy< float >;
template class gkg::ReductionStrategy< double >;
