#ifndef _gkg_core_pattern_StdVectorCompare_h_
#define _gkg_core_pattern_StdVectorCompare_h_

#include <vector>
#include <gkg-core-exception/Exception.h>


namespace gkg
{


template < class T >
struct StdVectorCompare /*: public std::binary_function< std::vector< T >,
                                                       std::vector< T >,
                                                       bool >
                          deprecated in c++17 */
{

  typedef std::vector< T > first_argument_type;
  typedef std::vector< T > second_argument_type;
  typedef bool result_type;

  bool operator()( const std::vector< T >& v1,
                   const std::vector< T >& v2 ) const
  {

    try
    {

      int32_t size = ( int32_t )v1.size();
      if ( ( int32_t )v2.size() != size )
      {

        throw std::runtime_error( "incompatible vector size(s)" );

      }

      bool result = false;
      bool resultIsKnown = false;
      int32_t index = 0;
      for ( index = size - 1; index >= 0; index-- )
      {

        if( !resultIsKnown )
        { 

          if( ( v1[ index ] < v2[ index ] ) ||
                                             ( v1[ index ] == v2[ index ] ) ) 
          {

            if( v1[ index ] < v2[ index ] )
            {

              result = true;
              resultIsKnown = true;

            }

          }
          else
          {

            resultIsKnown = true;

          }
        
        }

      }


      return result;


    }
    GKG_CATCH( "template < class T > "
               "bool StdVectorCompare< T >::operator( "
               "const std::vector< T >& v1, "
               "const std::vector< T >& v2 ) const" );

  }



};


}


#endif
