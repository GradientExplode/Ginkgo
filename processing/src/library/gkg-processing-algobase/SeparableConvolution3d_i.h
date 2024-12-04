#ifndef _gkg_processing_algobase_SeparableConvolution3d_i_h_
#define _gkg_processing_algobase_SeparableConvolution3d_i_h_


#include <gkg-processing-algobase/SeparableConvolution3d.h>
#include <gkg-processing-algobase/Convolution3d_i.h>


template < class IN, class K, class OUT >
inline
void gkg::SeparableConvolution3d< IN, K, OUT >::convolve(
                                                const gkg::Volume< IN >& in,
                                                const std::vector< K >& kernelX,
                                                const std::vector< K >& kernelY,
                                                const std::vector< K >& kernelZ,
                                                gkg::Volume< OUT >& out ) const
{

  try
  {

     // building volume kernels
    int32_t kernelSizeX = ( int32_t )kernelX.size();
    int32_t kernelSizeY = ( int32_t )kernelY.size();
    int32_t kernelSizeZ = ( int32_t )kernelZ.size();
    int32_t x, y, z;
    gkg::Volume< K > kerX( kernelSizeX, 1, 1 );
    for ( x = 0; x < kernelSizeX; x++ )
    {

      kerX( x, 0, 0 ) = kernelX[ x ];

    }
    gkg::Volume< K > kerY( 1, kernelSizeY, 1 );
    for ( y = 0; y < kernelSizeY; y++ )
    {

      kerY( 0, y, 0 ) = kernelY[ y ];

    }
    gkg::Volume< K > kerZ( 1, 1, kernelSizeZ );
    for ( z = 0; z < kernelSizeZ; z++ )
    {

      kerZ( 0, 0, z ) = kernelZ[ z ];

    }

    // convoluting
    gkg::Convolution3d< IN, K, OUT > convolution3dIN;
    gkg::Convolution3d< OUT, K, OUT > convolution3dOUT;

    convolution3dIN.convolve( in, kerX, out );
    convolution3dOUT.convolve( out, kerY, out );
    convolution3dOUT.convolve( out, kerZ, out );

  }
  GKG_CATCH( "template < class IN, class K, class OUT > "
             "inline "
             "void gkg::SeparableConvolution3d< IN, K, OUT >::convolve( "
             "const gkg::Volume< IN >& in, "
             "const std::vector< K >& kernelX, "
             "const std::vector< K >& kernelY, "
             "const std::vector< K >& kernelZ, "
             "gkg::Volume< OUT >& out ) const" );

}



#endif
