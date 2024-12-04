#ifndef _gkg_processing_extrapolation_HoleFillingExtrapolator3d_i_h_
#define _gkg_processing_extrapolation_HoleFillingExtrapolator3d_i_h_


#include <gkg-processing-extrapolation/HoleFillingExtrapolator3d.h>
#include <gkg-processing-extrapolation/Extrapolator_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class IN, class MASK, class OUT >
inline
gkg::HoleFillingExtrapolator3d< IN, MASK, OUT >::HoleFillingExtrapolator3d()
                                          : gkg::Extrapolator< IN, MASK, OUT >()
{
}


template < class IN, class MASK, class OUT >
inline
gkg::HoleFillingExtrapolator3d< IN, MASK, OUT >::~HoleFillingExtrapolator3d()
{
}


template < class IN, class MASK, class OUT >
inline
void gkg::HoleFillingExtrapolator3d< IN, MASK, OUT >::extrapolate(
                                  const gkg::Volume< IN >& input,
                                  const gkg::Volume< MASK >& restrictedMask,
                                  const gkg::Volume< MASK >& enlargedMask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  gkg::Volume< OUT >& output,
                                  bool verbose ) const
{

  try
  {

    int32_t sizeX = input.getSizeX();
    int32_t sizeY = input.getSizeY();
    int32_t sizeZ = input.getSizeZ();
    int32_t sizeT = input.getSizeT();

    // sanity checks
    if ( ( restrictedMask.getSizeX() != sizeX ) || 
         ( restrictedMask.getSizeY() != sizeY ) || 
         ( restrictedMask.getSizeZ() != sizeZ ) || 
         ( restrictedMask.getSizeT() != sizeT ) ||
         ( enlargedMask.getSizeX() != sizeX ) || 
         ( enlargedMask.getSizeY() != sizeY ) || 
         ( enlargedMask.getSizeZ() != sizeZ ) || 
         ( enlargedMask.getSizeT() != sizeT ) )
    {

      throw std::runtime_error( "inconsistent input and mask size(s)" );

    }

    // duplicating input in case output reference is equal to input reference
    gkg::Volume< IN > tmpInput( input );

    // copying input to output
    gkg::Converter< gkg::Volume< IN >, gkg::Volume< OUT > > volumeConverter;
    gkg::Volume< OUT > tmpOutput;
    volumeConverter.convert( tmpInput, tmpOutput );

    // building hole mask
    if ( verbose )
    {

      std::cout << "building hole mask" << std::flush;

    }
    gkg::Volume< uint8_t > holeMask( sizeX, sizeY, sizeZ, sizeT );
    typename gkg::Volume< MASK >::const_iterator r = restrictedMask.begin();
    typename gkg::Volume< MASK >::const_iterator e = enlargedMask.begin();
    typename gkg::Volume< uint8_t >::iterator h = holeMask.begin(),
                                              he = holeMask.end();
    while ( h != he )

    {

      if ( !testFunction.getState( *r ) && testFunction.getState( *e ) )
      {

        *h = 1U;

      }
      else
      {

        *h = 0U;

      }
      ++ r;
      ++ e;
      ++ h;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 18 ) << std::flush;

    }

    // filling holes with extrapolated values
    if ( verbose )
    {

      std::cout << "filling holes" << std::flush;

    }
    int32_t x, y, z, t;
    int32_t nx, ny, nz;

    gkg::BoundingBox< int32_t > bBox( 0, sizeX - 1,
                                      0, sizeY - 1,
                                      0, sizeZ - 1,
                                      0, sizeT - 1 );
                                  

    int32_t maximumDelta = std::min( std::min( sizeX - 1, sizeY - 1 ), 
                                     sizeZ - 1 );

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( holeMask( x, y, z, t ) )
            {

              // looking around for valid neighbors
              int32_t delta = 1;
              int32_t count = 0;
              OUT sum = 0;
              while ( ( delta < maximumDelta ) && !count )
              {

                for ( nz = z - delta; nz <= z + delta; nz++ )
                {

                  for ( ny = y - delta; ny <= y + delta; ny++ )
                  {

                    for ( nx = x - delta; nx <= x + delta; nx++ )
                    {

                      if ( bBox.contains( nx, ny, nz, t ) )
                      {

                        if ( !holeMask( nx, ny, nz, t ) &&
                             restrictedMask( nx, ny, nz, t ) )
                        {

                          sum += tmpOutput( nx, ny, nz, t );
                          ++ count;

                        }

                      }

                    }

                  }

                }
                ++ delta;

              }

              tmpOutput( x, y, z, t ) = ( count ? sum / ( OUT )count :
                                                  ( OUT )0 );

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 13 ) << std::flush;

    }

    // smoothing extrapolated values only inside holes
    if ( verbose )
    {

      std::cout << "smoothing holes" << std::flush;

    }
    output = tmpOutput;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( holeMask( x, y, z, t ) )
            {

              int32_t count = 0;
              OUT sum = 0;
              for ( nz = z - 1; nz <= z + 1; nz++ )
              {

                for ( ny = y - 1; ny <= y + 1; ny++ )
                {

                  for ( nx = x - 1; nx <= x + 1; nx++ )
                  {

                    if ( bBox.contains( nx, ny, nz, t ) )
                    {

                      if ( enlargedMask( nx, ny, nz, t ) )
                      {

                        sum += tmpOutput( nx, ny, nz, t );
                        ++ count;

                      }

                    }

                  }

                }

              }
              output( x, y, z, t ) = sum / ( OUT )count;

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 15 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "void gkg::HoleFillingExtrapolator3d< IN, MASK, OUT >::"
             "extrapolate( "
             "const gkg::Volume< IN >& input, "
             "const gkg::Volume< MASK >& restrictedMask, "
             "const gkg::Volume< MASK >& enlargedMask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "gkg::Volume< OUT >& ouput, "
             "bool verbose ) const" );

}


#endif
