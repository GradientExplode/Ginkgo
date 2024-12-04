#ifndef _gkg_processing_algobase_VolumeBinarizerContext_i_h_
#define _gkg_processing_algobase_VolumeBinarizerContext_i_h_


#include <gkg-processing-algobase/VolumeBinarizerContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class OUT >
gkg::VolumeBinarizerContext< IN, OUT >::VolumeBinarizerContext(
                                const gkg::Volume< IN >& in,
                                gkg::Volume< OUT >& out,
                                const gkg::Binarizer< IN, OUT >& itemBinarizer )
                                      : gkg::LoopContext< int32_t >(),
                                        _in( in ),
                                        _out( out ),
                                        _itemBinarizer( itemBinarizer )
{
}


template < class IN, class OUT >
void gkg::VolumeBinarizerContext< IN, OUT >::doIt( int32_t startIndex, 
                                                   int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;
    int32_t sizeX = _in.getSizeX();
    int32_t sizeY = _in.getSizeY();
    int32_t sizeT = _in.getSizeT();
    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = startIndex; z < stopIndex; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            _itemBinarizer.binarize( _in( x, y, z, t ), _out( x, y, z, t ) );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::VolumeBinarizerContext< IN, OUT >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif