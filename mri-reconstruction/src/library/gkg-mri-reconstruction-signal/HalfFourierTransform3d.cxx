#include <gkg-mri-reconstruction-signal/HalfFourierTransform3d.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-processing-signal/Apodization.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


#define MASK_SLOPE_WIDTH   2.0
#define SLOPE( l, p, w ) ( 1.0 / ( 1.0 + exp( ( ( p ) - ( l ) ) / ( w ) ) ) )


template < class T >
gkg::HalfFourierTransform3d< T >::HalfFourierTransform3d()
{
}


template < class T >
gkg::HalfFourierTransform3d< T >::~HalfFourierTransform3d()
{
}


template < class T >
void gkg::HalfFourierTransform3d< T >::applyCenteredInverse(
                             gkg::Volume< std::complex< T > >& data,
                             gkg::HalfFourierTransform3d< T >::Axis axis,
                             int32_t rawSize,
                             int32_t overScanCount,
                             const gkg::Apodization< T >* apodization,
                             bool verbose ) const
{

  try
  {


    // sanity checks
    if ( data.getSizeT() != 1 )
    {

      throw std::runtime_error( "input data has sizeT different from 1" );

    }

    // processing 3D FFT
    switch ( axis )
    {

      case gkg::FourierTransform3d< T >::Y_AXIS:

        {

          int32_t x, line, slice;

          std::vector< T > maskLow( rawSize );
          std::vector< T > maskHigh( rawSize );
          gkg::Volume< std::complex< T > > dataLow( data.getSizeX(),
                                                    data.getSizeY() );
          gkg::Volume< std::complex< T > > dataHigh( data.getSizeX(),
                                                     data.getSizeY() );
          T line1 = 2 * MASK_SLOPE_WIDTH;
          T line2 = 2 * overScanCount - 2 * line1;
          for ( line = 0; line < rawSize; line++ )
          {

            // here we use rawSize - 1 - line instead of line, because 
            // k-space has been reordered, and low frequencies are located
            // in the bottom lines
            maskLow[ rawSize - 1 - line ] =
                            SLOPE( line, line1, MASK_SLOPE_WIDTH ) -
                            SLOPE( line, line2, MASK_SLOPE_WIDTH );
            maskHigh[ rawSize - 1 - line ] =
                            SLOPE( line, line1, MASK_SLOPE_WIDTH ) +
                            SLOPE( line, line2, MASK_SLOPE_WIDTH );

          }

          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            // copy data to low data
            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                dataLow( x, line ) = data( x, line, slice );
                dataHigh( x, line ) = data( x, line, slice );

              }

            }

            // multiply each complex column of data by the real low mask
            for ( line = 0; line < rawSize; line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                dataLow( x, line ) *= maskLow[ line ];
                dataHigh( x, line ) *= maskHigh[ line ];

               }

            }

            // apply standard Fourier transform
            gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                         dataLow,
                                         gkg::FourierTransform3d< T >::Y_AXIS,
                                         apodization );
            gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                         dataHigh,
                                         gkg::FourierTransform3d< T >::Y_AXIS,
                                         apodization );

            // rotate the phase of data by minus phase of low data
            // calculates only real part; imaginary is set to zero
            T div = 0.0f;
            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = dataHigh( x, line );
                div = sqrt( std::norm( dataLow( x, line ) ) );
                if ( div )
                {

                  data( x, line, slice ) =
                    std::complex< T >( ( std::real( dataHigh( x, line ) ) * 
                                         std::real( dataLow( x, line ) ) +
                                         std::imag( dataHigh( x, line ) ) * 
                                         std::imag( dataLow( x, line ) ) ) /
                                       div,
                                       0.0f );

                }
                data( x, line, slice ) = 
                std::complex< T >( std::real( data( x, line, slice ) ), 0.0f );

              }

            }

            if ( verbose )
            {

              if ( slice != 0 )
              {

                std::cout << gkg::Eraser( 21 );

              }
              std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                        << " / " << std::setw( 4 ) << data.getSizeZ()
                        << " ]" << std::flush;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 21 ) << std::flush;

          }

        }
        break;

      case gkg::FourierTransform3d< T >::Z_AXIS:

        {

          int32_t x, line, slice;

          std::vector< T > maskLow( rawSize );
          std::vector< T > maskHigh( rawSize );
          gkg::Volume< std::complex< T > > dataLow( data.getSizeX(),
                                                    data.getSizeZ() );
          gkg::Volume< std::complex< T > > dataHigh( data.getSizeX(),
                                                     data.getSizeZ() );
          T slice1 = 2 * MASK_SLOPE_WIDTH;
          T slice2 = 2 * overScanCount - 2 * slice1;
          for ( slice = 0; slice < rawSize; slice++ )
          {

            // here we use rawSize - 1 - line instead of line, because 
            // k-space has been reordered, and low frequencies are located
            // in the bottom lines
            maskLow[ rawSize - 1 - slice ] =
                            SLOPE( slice, slice1, MASK_SLOPE_WIDTH ) -
                            SLOPE( slice, slice2, MASK_SLOPE_WIDTH );
            maskHigh[ rawSize - 1 - slice ] =
                            SLOPE( slice, slice1, MASK_SLOPE_WIDTH ) +
                            SLOPE( slice, slice2, MASK_SLOPE_WIDTH );

          }

          for ( line = 0; line < data.getSizeY(); line++ )
          {

            // copy data to low data
            for ( slice = 0; slice < data.getSizeZ(); slice++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                dataLow( x, slice ) = data( x, line, slice );
                dataHigh( x, slice ) = data( x, line, slice );

              }

            }

            // multiply each complex column of data by the real low mask
            for ( slice = 0; slice < rawSize; slice++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                dataLow( x, slice ) *= maskLow[ slice ];
                dataHigh( x, slice ) *= maskHigh[ slice ];

               }

            }

            // apply standard Fourier transform
            gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                         dataLow,
                                         gkg::FourierTransform3d< T >::Y_AXIS,
                                         apodization );
            gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                         dataHigh,
                                         gkg::FourierTransform3d< T >::Y_AXIS,
                                         apodization );

            // rotate the phase of data by minus phase of low data
            // calculates only real part; imaginary is set to zero
            T div = 0.0f;
            for ( slice = 0; slice < data.getSizeZ(); slice++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = dataHigh( x, slice );
                div = sqrt( std::norm( dataLow( x, slice ) ) );
                if ( div )
                {

                  data( x, line, slice ) =
                    std::complex< T >( ( std::real( dataHigh( x, slice ) ) * 
                                         std::real( dataLow( x, slice ) ) +
                                         std::imag( dataHigh( x, slice ) ) * 
                                         std::imag( dataLow( x, slice ) ) ) /
                                       div,
                                       0.0f );

                }
                data( x, line, slice ) = 
                std::complex< T >( std::real( data( x, line, slice ) ), 0.0f );

              }

            }

            if ( verbose )
            {

              if ( line != 0 )
              {

                std::cout << gkg::Eraser( 21 );

              }
              std::cout << " line[ " << std::setw( 4 ) << line + 1 
                        << " / " << std::setw( 4 ) << data.getSizeY()
                        << " ]" << std::flush;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 21 ) << std::flush;

          }

        }
        break;

      default:

        throw std::runtime_error( " only Y and Z axis were implemented!" );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HalfFourierTransform3d< T >::applyCenteredInverse( "
             "gkg::Volume< std::complex< T > >& data, "
             "gkg::HalfFourierTransform3d< T >::Axis axis, "
             "int rawSize, "
             "int overScanCount, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}



template class gkg::HalfFourierTransform3d< float >;
template class gkg::HalfFourierTransform3d< double >;


#undef MASK_SLOP_WIDTH
#undef SLOPE
