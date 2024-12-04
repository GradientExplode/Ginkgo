#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>
#include <string>


template < class T >
gkg::FourierTransform3d< T >::FourierTransform3d()
{
}


template < class T >
gkg::FourierTransform3d< T >::~FourierTransform3d()
{
}


template < class T >
void gkg::FourierTransform3d< T >::applyRawDirect(
                                       gkg::Volume< std::complex< T > >& data,
                                       gkg::FourierTransform3d< T >::Axis axis,
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

      case gkg::FourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmp[ x ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyRawDirect(
                                                             tmp, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmp[ x ];

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

      case gkg::FourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmp[ line ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyRawDirect(
                                                             tmp, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmp[ line ];

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

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmp[ slice ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyRawDirect(
                                                             tmp, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmp[ slice ];

              }

            }

            if ( verbose )
            {

              if ( slice != 0 )
              {

                std::cout << gkg::Eraser( 20 );

              }
              std::cout << " line[ " << std::setw( 4 ) << slice + 1 
                        << " / " << std::setw( 4 ) << data.getSizeZ()
                        << " ]" << std::flush;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 20 ) << std::flush;

          }

        }
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform3d< T >::applyRawDirect( "
             "gkg::Volume< std::complex< T > >& data, "
             "gkg::FourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}


template < class T >
void gkg::FourierTransform3d< T >::applyRawInverse(
                                       gkg::Volume< std::complex< T > >& data,
                                       gkg::FourierTransform3d< T >::Axis axis,
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

      case gkg::FourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmp[ x ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyRawInverse(
                                                             tmp, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmp[ x ];

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

      case gkg::FourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmp[ line ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyRawInverse(
                                                             tmp, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmp[ line ];

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

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmp[ slice ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyRawInverse(
                                                             tmp, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmp[ slice ];

              }

            }

            if ( verbose )
            {

              if ( slice != 0 )
              {

                std::cout << gkg::Eraser( 20 );

              }
              std::cout << " line[ " << std::setw( 4 ) << slice + 1 
                        << " / " << std::setw( 4 ) << data.getSizeZ()
                        << " ]" << std::flush;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 20 ) << std::flush;

          }

        }
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform3d< T >::applyRawInverse( "
             "gkg::Volume< std::complex< T > >& data, "
             "gkg::FourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::FourierTransform3d< T >::applyCenteredDirect(
                                       gkg::Volume< std::complex< T > >& data,
                                       gkg::FourierTransform3d< T >::Axis axis,
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

      case gkg::FourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmp[ x ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyCenteredDirect(
                                                             tmp, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmp[ x ];

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

      case gkg::FourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmp[ line ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyCenteredDirect(
                                                             tmp, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmp[ line ];

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

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmp[ slice ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyCenteredDirect(
                                                             tmp, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmp[ slice ];

              }

            }

            if ( verbose )
            {

              if ( slice != 0 )
              {

                std::cout << gkg::Eraser( 20 );

              }
              std::cout << " line[ " << std::setw( 4 ) << slice + 1 
                        << " / " << std::setw( 4 ) << data.getSizeZ()
                        << " ]" << std::flush;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 20 ) << std::flush;

          }

        }
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform3d< T >::applyCenteredDirect( "
             "gkg::Volume< std::complex< T > >& data, "
             "gkg::FourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}


template < class T >
void gkg::FourierTransform3d< T >::applyCenteredInverse(
                                       gkg::Volume< std::complex< T > >& data,
                                       gkg::FourierTransform3d< T >::Axis axis,
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

      case gkg::FourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmp[ x ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyCenteredInverse(
                                                             tmp, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmp[ x ];

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

      case gkg::FourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmp[ line ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyCenteredInverse(
                                                             tmp, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmp[ line ];

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

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmp( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmp[ slice ] = data( x, line, slice );

              }
              gkg::FourierTransform< T >::getInstance().applyCenteredInverse(
                                                             tmp, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmp[ slice ];

              }

            }

            if ( verbose )
            {

              if ( line != 0 )
              {

                std::cout << gkg::Eraser( 20 );

              }
              std::cout << " line[ " << std::setw( 4 ) << line + 1 
                        << " / " << std::setw( 4 ) << data.getSizeY()
                        << " ]" << std::flush;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 20 ) << std::flush;

          }

        }
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform3d< T >::applyCenteredInverse( "
             "gkg::Volume< std::complex< T > >& data, "
             "gkg::FourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}


template class gkg::FourierTransform3d< float >;
template class gkg::FourierTransform3d< double >;
