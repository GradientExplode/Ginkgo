#ifndef _gkg_processing_algobase_InverseFunction_i_h_
#define _gkg_processing_algobase_InverseFunction_i_h_


#include <gkg-processing-algobase/InverseFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::InverseFunction< T >::InverseFunction(
                                    const gkg::TestFunction< T >& testFunction )
                          : gkg::TestFunction< T >(),
                            _testFunction( testFunction )
{
}


template < class T >
inline
gkg::InverseFunction< T >::~InverseFunction()
{
}


template < class T >
inline
bool gkg::InverseFunction< T >::getState( const T& value ) const
{

  return !_testFunction.getState( value );

}




#endif
