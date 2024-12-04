#ifndef _gkg_processing_algobase_DifferentFromFunction_i_h_
#define _gkg_processing_algobase_DifferentFromFunction_i_h_


#include <gkg-processing-algobase/DifferentFromFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::DifferentFromFunction< T >::DifferentFromFunction( const T& threshold )
                                : gkg::TestFunction< T >(),
                                  _threshold( threshold )
{
}


template < class T >
inline
gkg::DifferentFromFunction< T >::~DifferentFromFunction()
{
}


template < class T >
inline
bool gkg::DifferentFromFunction< T >::getState( const T& value ) const
{

  return ( value != _threshold );

}




#endif
