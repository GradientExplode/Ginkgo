#ifndef _gkg_processing_algobase_Convolution3d_i_h_
#define _gkg_processing_algobase_Convolution3d_i_h_


#include <gkg-processing-algobase/Convolution3d.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class K, class OUT >
inline
void gkg::Convolution3d< IN, K, OUT >::convolve( const gkg::Volume< IN >& in,
                                                 const gkg::Volume< K >& kernel,
                                                 gkg::Volume< OUT >& out ) const
{

  try
  {

    if ( kernel.getSizeT() != 1 )
    {

      throw std::runtime_error( "kernel must have T size equal to 1" );

    }

    int32_t kernelSizeX = kernel.getSizeX();
    int32_t kernelSizeY = kernel.getSizeY();
    int32_t kernelSizeZ = kernel.getSizeZ();

    if ( ( kernelSizeX % 2 == 0 ) ||
         ( kernelSizeY % 2 == 0 ) ||
         ( kernelSizeZ % 2 == 0 ) )
    {

      std::cerr << "kernel has even dimensions" << std::endl;

    }
         
    // allocating output
    gkg::Volume< OUT > tmpOut( in.getSizeX(),
                               in.getSizeY(),
                               in.getSizeZ(),
                               in.getSizeT() );

    // duplicating header
    tmpOut.getHeader() = in.getHeader();
    tmpOut.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    // convoluting
    int32_t inSizeX = in.getSizeX();
    int32_t inSizeY = in.getSizeY();
    int32_t inSizeZ = in.getSizeZ();
    int32_t inSizeT = in.getSizeT();

    int32_t middleX = ( kernelSizeX - 1 ) / 2;
    int32_t middleY = ( kernelSizeY - 1 ) / 2;
    int32_t middleZ = ( kernelSizeZ - 1 ) / 2;

    int32_t x, y, z, t, kx, ky, kz, nx, ny, nz;

    double accumulator;

    for ( t = 0; t < inSizeT; t++ )
    {

      for ( z = 0; z < inSizeZ; z++ )
      {

        for ( y = 0; y < inSizeY; y++ )
        {

          for ( x = 0; x < inSizeX; x++ )
          {

            accumulator = 0.0;
            for ( kz = 0; kz < kernelSizeZ; kz++ )
            {

              for ( ky = 0; ky < kernelSizeY; ky++ )
              {

                for ( kx = 0; kx < kernelSizeX; kx++ )
                {

                  nx = x + kx - middleX;
                  ny = y + ky - middleY;
                  nz = z + kz - middleZ;

                  if ( nx < 0 )
                  {

                    nx = 0;

                  }
                  else if ( nx >= inSizeX )
                  {

                    nx = inSizeX - 1;

                  }
                  if ( ny < 0 )
                  {

                    ny = 0;

                  }
                  else if ( ny >= inSizeY )
                  {

                    ny = inSizeY - 1;

                  }
                  if ( nz < 0 )
                  {

                    nz = 0;

                  }
                  else if ( nz >= inSizeZ )
                  {

                    nz = inSizeZ - 1;

                  }
                  accumulator += ( double )in( nx, ny, nz, t ) *
                                 ( double )kernel( kx, ky, kz );

                }

              }

            }
            tmpOut( x, y, z, t ) = ( OUT )accumulator;

          }

        }

      }

    }
    out = tmpOut;

  }
  GKG_CATCH( "template < class IN, class K, class OUT > "
             "inline "
             "void gkg::Convolution3d< IN, K, OUT >::convolve( "
             "const gkg::Volume< IN >& in, "
             "const gkg::Volume< K >& kernel, "
             "gkg::Volume< OUT >& out ) const" );


}



#endif
