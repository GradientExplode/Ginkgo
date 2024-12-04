#ifndef _gkg_processing_algobase_BetweenFunction_i_h_
#define _gkg_processing_algobase_BetweenFunction_i_h_


#include <gkg-processing-algobase/BetweenFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::BetweenFunction< T >::BetweenFunction( const T& lowerThreshold,
                                            const T& upperThreshold )
                          : gkg::TestFunction< T >(),
                            _lowerThreshold( lowerThreshold ),
                            _upperThreshold( upperThreshold )
{
}


template < class T >
inline
gkg::BetweenFunction< T >::~BetweenFunction()
{
}


template < class T >
inline
bool gkg::BetweenFunction< T >::getState( const T& value ) const
{

  return ( ( value > _lowerThreshold ) &&
           ( value < _upperThreshold ) );

}




#endif
