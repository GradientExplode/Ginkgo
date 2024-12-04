#ifndef _gkg_processing_algobase_LowerOrEqualToFunction_i_h_
#define _gkg_processing_algobase_LowerOrEqualToFunction_i_h_


#include <gkg-processing-algobase/LowerOrEqualToFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::LowerOrEqualToFunction< T >::LowerOrEqualToFunction( const T& threshold )
                                 : gkg::TestFunction< T >(),
                                   _threshold( threshold )
{
}


template < class T >
inline
gkg::LowerOrEqualToFunction< T >::~LowerOrEqualToFunction()
{
}


template < class T >
inline
bool gkg::LowerOrEqualToFunction< T >::getState( const T& value ) const
{

  return ( value <= _threshold );

}




#endif
