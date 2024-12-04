#ifndef _gkg_processing_algobase_GreaterOrEqualToFunction_i_h_
#define _gkg_processing_algobase_GreaterOrEqualToFunction_i_h_


#include <gkg-processing-algobase/GreaterOrEqualToFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::GreaterOrEqualToFunction< T >::GreaterOrEqualToFunction(
                                                            const T& threshold )
                                   : gkg::TestFunction< T >(),
                                     _threshold( threshold )
{
}


template < class T >
inline
gkg::GreaterOrEqualToFunction< T >::~GreaterOrEqualToFunction()
{
}


template < class T >
inline
bool gkg::GreaterOrEqualToFunction< T >::getState( const T& value ) const
{

  return ( value >= _threshold );

}




#endif
