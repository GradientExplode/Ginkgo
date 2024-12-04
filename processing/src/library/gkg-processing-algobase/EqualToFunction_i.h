#ifndef _gkg_processing_algobase_EqualToFunction_i_h_
#define _gkg_processing_algobase_EqualToFunction_i_h_


#include <gkg-processing-algobase/EqualToFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::EqualToFunction< T >::EqualToFunction( const T& threshold )
                          : gkg::TestFunction< T >(),
                            _threshold( threshold )
{
}


template < class T >
inline
gkg::EqualToFunction< T >::~EqualToFunction()
{
}


template < class T >
inline
bool gkg::EqualToFunction< T >::getState( const T& value ) const
{

  return ( value == _threshold );

}




#endif
