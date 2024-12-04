#ifndef _gkg_processing_algobase_OutsideOrEqualToFunction_i_h_
#define _gkg_processing_algobase_OutsideOrEqualToFunction_i_h_


#include <gkg-processing-algobase/OutsideOrEqualToFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::OutsideOrEqualToFunction< T >::OutsideOrEqualToFunction(
                                                       const T& lowerThreshold,
                                                       const T& upperThreshold )
                                   : gkg::TestFunction< T >(),
                                     _lowerThreshold( lowerThreshold ),
                                     _upperThreshold( upperThreshold )
{
}


template < class T >
inline
gkg::OutsideOrEqualToFunction< T >::~OutsideOrEqualToFunction()
{
}


template < class T >
inline
bool gkg::OutsideOrEqualToFunction< T >::getState( const T& value ) const
{

  return ( ( value <= _lowerThreshold ) &&
           ( value >= _upperThreshold ) );

}




#endif
