#ifndef _gkg_processing_algobase_VolumeMinMaxFilterWithMaskContext_i_h_
#define _gkg_processing_algobase_VolumeMinMaxFilterWithMaskContext_i_h_


#include <gkg-processing-algobase/VolumeMinMaxFilterWithMaskContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T, class M >
gkg::VolumeMinMaxFilterWithMaskContext< T, M >::
                                              VolumeMinMaxFilterWithMaskContext(
                                     const gkg::Volume< T >& volume,
                                     const gkg::Volume< M >& mask,
                                     const gkg::TestFunction< M >& testFunction,
                                     T& minimum,
                                     T& maximum )
                                               : gkg::LoopContext< int32_t >(),
                                                 _ptr0( &volume( 0 ) ),
                                                 _m0( &mask( 0 ) ),
                                                 _testFunction( &testFunction ),
                                                 _minimum( minimum ),
                                                 _maximum( maximum )
{
}


template < class T, class M >
void gkg::VolumeMinMaxFilterWithMaskContext< T, M >::doIt( int32_t startIndex, 
                                                           int32_t count )
{

  try
  {

    T minimum = _minimum;
    T maximum = _maximum;
    const T* ptr = _ptr0 + startIndex;
    const M* m = _m0 + startIndex;

    while ( count-- )
    {

      if ( _testFunction->getState( *m ) )
      {

        if ( *ptr < minimum )
        {

          minimum = *ptr;

        }
        else if ( *ptr > maximum )
        {

          maximum = *ptr;

        }

      }
      ++ ptr;
      ++ m;


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
  GKG_CATCH( "template < class T, class M > "
             "void gkg::VolumeMinMaxFilterWithMaskContext< T, M >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
