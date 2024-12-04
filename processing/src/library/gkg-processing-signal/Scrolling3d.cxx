#include <gkg-processing-signal/Scrolling3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>


template < class T >
gkg::Scrolling3d< T >::Scrolling3d()
{
}


template < class T >
gkg::Scrolling3d< T >::~Scrolling3d()
{
}


template < class T >
void gkg::Scrolling3d< T >::apply( gkg::Volume< std::complex< T > >& data,
                                   const T& scrollingRatio,
                                   gkg::Scrolling3d< T >::Axis axis,
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

      case gkg::Scrolling3d< T >::X_AXIS:

        {

          int32_t x, line, slice;
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) *=
                           std::polar( ( T )1.0,
                                       ( T )( 2 * M_PI * scrollingRatio * x ) );

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

      case gkg::Scrolling3d< T >::Y_AXIS:

        {

          int32_t x, line, slice;
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) *=
                        std::polar( ( T )1.0,
                                    ( T )( 2 * M_PI * scrollingRatio * line ) );

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

      case gkg::Scrolling3d< T >::Z_AXIS:

        {

          int32_t x, line, slice;
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) *=
                       std::polar( ( T )1.0,
                                   ( T )( 2 * M_PI * scrollingRatio * slice ) );

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

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Scrolling3d< T >::apply( "
             "gkg::Volume< std::complex< T > >& data, "
             "gkg::Scrolling3d< T >::Axis axis, "
             "const T& scrollingRatio, "
             "bool verbose ) const" );

}


template class gkg::Scrolling3d< float >;
template class gkg::Scrolling3d< double >;
