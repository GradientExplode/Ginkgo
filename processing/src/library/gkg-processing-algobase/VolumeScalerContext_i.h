#ifndef _gkg_processing_algobase_VolumeScalerContext_i_h_
#define _gkg_processing_algobase_VolumeScalerContext_i_h_


#include <gkg-processing-algobase/VolumeScalerContext.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::VolumeScalerContext< T >::VolumeScalerContext(
                                            gkg::Volume< T >& in,
                                            const gkg::Scaler< T >& itemScaler )
                             : gkg::LoopContext< int32_t >(),
                               _in( in ),
                               _itemScaler( itemScaler )
{
}


template < class T >
void gkg::VolumeScalerContext< T >::doIt( int32_t startIndex, int32_t count )
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

            _itemScaler.scale( _in( x, y, z, t ) );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeScalerContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
