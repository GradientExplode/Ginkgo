#ifndef _gkg_processing_algobase_LowerThanFunction_i_h_
#define _gkg_processing_algobase_LowerThanFunction_i_h_


#include <gkg-processing-algobase/LowerThanFunction.h>
#include <gkg-processing-algobase/TestFunction_i.h>



template < class T >
inline
gkg::LowerThanFunction< T >::LowerThanFunction( const T& threshold )
                            : gkg::TestFunction< T >(),
                              _threshold( threshold )
{
}


template < class T >
inline
gkg::LowerThanFunction< T >::~LowerThanFunction()
{
}


template < class T >
inline
bool gkg::LowerThanFunction< T >::getState( const T& value ) const
{

  return ( value < _threshold );

}




#endif
