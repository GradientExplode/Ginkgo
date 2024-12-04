#ifndef _gkg_processing_algobase_BetweenOrEqualToFunction_i_h_
#define _gkg_processing_algobase_BetweenOrEqualToFunction_i_h_


#include <gkg-processing-algobase/BetweenOrEqualToFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::BetweenOrEqualToFunction< T >::BetweenOrEqualToFunction(
                                                       const T& lowerThreshold,
                                                       const T& upperThreshold )
                                   : gkg::TestFunction< T >(),
                                     _lowerThreshold( lowerThreshold ),
                                     _upperThreshold( upperThreshold )
{
}


template < class T >
inline
gkg::BetweenOrEqualToFunction< T >::~BetweenOrEqualToFunction()
{
}


template < class T >
inline
bool gkg::BetweenOrEqualToFunction< T >::getState( const T& value ) const
{

  return ( ( value >= _lowerThreshold ) &&
           ( value <= _upperThreshold ) );

}




#endif
