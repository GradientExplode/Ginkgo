#ifndef _gkg_processing_algobase_VolumeMinMaxFilterContext_i_h_
#define _gkg_processing_algobase_VolumeMinMaxFilterContext_i_h_


#include <gkg-processing-algobase/VolumeMinMaxFilterContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::VolumeMinMaxFilterContext< T >::VolumeMinMaxFilterContext(
                                                 const gkg::Volume< T >& volume,
                                                 T& minimum,
                                                 T& maximum )
                                       : gkg::LoopContext< int32_t >(),
                                         _ptr0( &volume( 0 ) ),
                                         _minimum( minimum ),
                                         _maximum( maximum )
{
}


template < class T >
void gkg::VolumeMinMaxFilterContext< T >::doIt( int32_t startIndex, 
                                                int32_t count )
{

  try
  {

    T minimum = _minimum;
    T maximum = _maximum;
    const T* ptr = _ptr0 + startIndex;

    while ( count-- )
    {

      if ( *ptr < minimum )
      {

        minimum = *ptr;

      }
      else if ( *ptr > maximum )
      {

        maximum = *ptr;

      }

      ++ptr;

    }

    lock();

    if ( minimum < _minimum )
    {

      _minimum = minimum;

    }
    if ( maximum > _maximum )
    {

      _maximum = maximum;

    }

    unlock();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeMinMaxFilterContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
