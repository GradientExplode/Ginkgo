#include <gkg-mri-reconstruction-correction/ReadOutGradient.h>
#include <gkg-core-exception/Exception.h>
#include <string>


template < class T >
gkg::ReadOutGradient< T >::ReadOutGradient( int32_t sampleCount )
{

  try
  {

    if ( sampleCount < 1 )
    {

      throw std::runtime_error( "not enough samples" );

    }

    // allocating trajectory memory
    _trajectory = std::vector< T >( sampleCount );

  }
  GKG_CATCH( "template < class T > "
             "gkg::ReadOutGradient< T >::ReadOutGradient( int sampleCount )" );

}


template < class T >
gkg::ReadOutGradient< T >::~ReadOutGradient()
{
}


template < class T >
std::vector< T > gkg::ReadOutGradient< T >::getTrajectory() const
{

  return _trajectory;

}


template class gkg::ReadOutGradient< float >;
template class gkg::ReadOutGradient< double >;
