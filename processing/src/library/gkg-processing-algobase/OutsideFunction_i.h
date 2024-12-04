#ifndef _gkg_processing_algobase_OutsideFunction_i_h_
#define _gkg_processing_algobase_OutsideFunction_i_h_


#include <gkg-processing-algobase/OutsideFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::OutsideFunction< T >::OutsideFunction( const T& lowerThreshold,
                                            const T& upperThreshold )
                          : gkg::TestFunction< T >(),
                            _lowerThreshold( lowerThreshold ),
                            _upperThreshold( upperThreshold )
{
}


template < class T >
inline
gkg::OutsideFunction< T >::~OutsideFunction()
{
}


template < class T >
inline
bool gkg::OutsideFunction< T >::getState( const T& value ) const
{

  return ( ( value < _lowerThreshold ) &&
           ( value > _upperThreshold ) );

}




#endif
