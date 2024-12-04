#include <gkg-mri-reconstruction-correction/TrapezoidROGradient.h>


template < class T >
gkg::TrapezoidROGradient< T >::TrapezoidROGradient( int32_t sampleCount,
                                                    const T& rampTime,
                                                    const T& topFlatTime )
                              : gkg::ReadOutGradient< T >( sampleCount )
{

  T acquisitionTime = topFlatTime + 2 * rampTime;
  int32_t position1 = ( int32_t )( sampleCount * rampTime / acquisitionTime );
  int32_t position2 =  sampleCount - position1;

  // calculating normalized trajectory points
  int32_t p;
  for ( p = 0; p < position1; p++ )
  {

    this->_trajectory[ p ] = ( T )p / ( T )position1;

  }
  for ( p = position1; p < position2; p++ )
  {

    this->_trajectory[ p ] = 1.0;

  }
  for ( p = position2; p < sampleCount; p++ )
  {

    this->_trajectory[ p ] = 1.0 - ( T )( p - position2 ) / ( T )position1;

  }

  // integrating
  for ( p = 1; p < sampleCount; p++ )
  {

    this->_trajectory[ p ] += this->_trajectory[ p - 1 ];

  }

}


template < class T >
gkg::TrapezoidROGradient< T >::~TrapezoidROGradient()
{
}


template class gkg::TrapezoidROGradient< float >;
template class gkg::TrapezoidROGradient< double >;
