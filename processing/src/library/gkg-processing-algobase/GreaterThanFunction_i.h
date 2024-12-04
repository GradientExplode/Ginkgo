#ifndef _gkg_processing_algobase_GreaterThanFunction_i_h_
#define _gkg_processing_algobase_GreaterThanFunction_i_h_


#include <gkg-processing-algobase/GreaterThanFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::GreaterThanFunction< T >::GreaterThanFunction( const T& threshold )
                              : gkg::TestFunction< T >(),
                                _threshold( threshold )
{
}


template < class T >
inline
gkg::GreaterThanFunction< T >::~GreaterThanFunction()
{
}


template < class T >
inline
bool gkg::GreaterThanFunction< T >::getState( const T& value ) const
{

  return ( value > _threshold );

}




#endif
