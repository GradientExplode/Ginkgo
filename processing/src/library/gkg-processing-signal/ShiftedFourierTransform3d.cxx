#include <gkg-processing-signal/ShiftedFourierTransform3d.h>
#include <gkg-processing-signal/ShiftedFourierTransform.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>
#include <string>


template < class T >
gkg::ShiftedFourierTransform3d< T >::ShiftedFourierTransform3d()
{
}


template < class T >
gkg::ShiftedFourierTransform3d< T >::~ShiftedFourierTransform3d()
{
}


template < class T >
void gkg::ShiftedFourierTransform3d< T >::applyRawDirect(
                                 gkg::Volume< std::complex< T > >& data,
                                 const gkg::Volume< T >& shift,
                                 gkg::ShiftedFourierTransform3d< T >::Axis axis,
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
    if ( shift.getSizeT() != 1 )
    {

      throw std::runtime_error( "shift map has sizeT different from 1" );

    }
    if ( ( shift.getSizeX() != data.getSizeX() ) ||
         ( shift.getSizeY() != data.getSizeY() ) ||
         ( shift.getSizeZ() != data.getSizeZ() ) )
    {

      throw std::runtime_error(
                              "incompatible shift map and input data size(s)" );

    }
         
    // processing 3D FFT
    switch ( axis )
    {

      case gkg::ShiftedFourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeX() );
          std::vector< T > tmpShift( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmpData[ x ] = data( x, line, slice );
                tmpShift[ x ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().applyRawDirect(
                                               tmpData, tmpShift, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmpData[ x ];

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

      case gkg::ShiftedFourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeY() );
          std::vector< T > tmpShift( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmpData[ line ] = data( x, line, slice );
                tmpShift[ line ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().applyRawDirect(
                                               tmpData, tmpShift, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmpData[ line ];

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

      case gkg::ShiftedFourierTransform3d< T >::Z_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeZ() );
          std::vector< T > tmpShift( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmpData[ slice ] = data( x, line, slice );
                tmpShift[ slice ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().applyRawDirect(
                                               tmpData, tmpShift, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmpData[ slice ];

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
             "void gkg::ShiftedFourierTransform3d< T >::applyRawDirect( "
             "gkg::Volume< std::complex< T > >& data, "
             "const gkg::Volume< T >& shift, "
             "gkg::ShiftedFourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform3d< T >::applyRawInverse(
                                 gkg::Volume< std::complex< T > >& data,
                                 const gkg::Volume< T >& shift,
                                 gkg::ShiftedFourierTransform3d< T >::Axis axis,
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
    if ( shift.getSizeT() != 1 )
    {

      throw std::runtime_error( "shift map has sizeT different from 1" );

    }
    if ( ( shift.getSizeX() != data.getSizeX() ) ||
         ( shift.getSizeY() != data.getSizeY() ) ||
         ( shift.getSizeZ() != data.getSizeZ() ) )
    {

      throw std::runtime_error(
                              "incompatible shift map and input data size(s)" );

    }

    // processing 3D FFT
    switch ( axis )
    {

      case gkg::ShiftedFourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeX() );
          std::vector< T > tmpShift( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmpData[ x ] = data( x, line, slice );
                tmpShift[ x ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().applyRawInverse(
                                               tmpData, tmpShift, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmpData[ x ];

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

      case gkg::ShiftedFourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeY() );
          std::vector< T > tmpShift( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmpData[ line ] = data( x, line, slice );
                tmpShift[ line ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().applyRawInverse(
                                               tmpData, tmpShift, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmpData[ line ];

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

      case gkg::ShiftedFourierTransform3d< T >::Z_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeZ() );
          std::vector< T > tmpShift( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmpData[ slice ] = data( x, line, slice );
                tmpShift[ slice ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().applyRawInverse(
                                               tmpData, tmpShift, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmpData[ slice ];

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
             "void gkg::ShiftedFourierTransform3d< T >::applyRawInverse( "
             "gkg::Volume< std::complex< T > >& data, "
             "const gkg::Volume< T >& shift, "
             "gkg::ShiftedFourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform3d< T >::applyCenteredDirect(
                                  gkg::Volume< std::complex< T > >& data,
                                  const gkg::Volume< T >& shift,
                                  gkg::ShiftedFourierTransform3d< T >::Axis axis,
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
    if ( shift.getSizeT() != 1 )
    {

      throw std::runtime_error( "shift map has sizeT different from 1" );

    }
    if ( ( shift.getSizeX() != data.getSizeX() ) ||
         ( shift.getSizeY() != data.getSizeY() ) ||
         ( shift.getSizeZ() != data.getSizeZ() ) )
    {

      throw std::runtime_error(
                              "incompatible shift map and input data size(s)" );

    }

    // processing 3D FFT
    switch ( axis )
    {

      case gkg::ShiftedFourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeX() );
          std::vector< T > tmpShift( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmpData[ x ] = data( x, line, slice );
                tmpShift[ x ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().
                                                            applyCenteredDirect(
                                               tmpData, tmpShift, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmpData[ x ];

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

      case gkg::ShiftedFourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeY() );
          std::vector< T > tmpShift( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmpData[ line ] = data( x, line, slice );
                tmpShift[ line ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().
                                                            applyCenteredDirect(
                                               tmpData, tmpShift, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmpData[ line ];

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

      case gkg::ShiftedFourierTransform3d< T >::Z_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeZ() );
          std::vector< T > tmpShift( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmpData[ slice ] = data( x, line, slice );
                tmpShift[ slice ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().
                                                            applyCenteredDirect(
                                               tmpData, tmpShift, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmpData[ slice ];

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
             "void gkg::ShiftedFourierTransform3d< T >::applyCenteredDirect( "
             "gkg::Volume< std::complex< T > >& data, "
             "const gkg::Volume< T >& shift, "
             "gkg::ShiftedFourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform3d< T >::applyCenteredInverse(
                                 gkg::Volume< std::complex< T > >& data,
                                 const gkg::Volume< T >& shift,
                                 gkg::ShiftedFourierTransform3d< T >::Axis axis,
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
    if ( shift.getSizeT() != 1 )
    {

      throw std::runtime_error( "shift map has sizeT different from 1" );

    }
    if ( ( shift.getSizeX() != data.getSizeX() ) ||
         ( shift.getSizeY() != data.getSizeY() ) ||
         ( shift.getSizeZ() != data.getSizeZ() ) )
    {

      throw std::runtime_error(
                              "incompatible shift map and input data size(s)" );

    }

    // processing 3D FFT
    switch ( axis )
    {

      case gkg::ShiftedFourierTransform3d< T >::X_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeX() );
          std::vector< T > tmpShift( data.getSizeX() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( line = 0; line < data.getSizeY(); line++ )
            {

              for ( x = 0; x < data.getSizeX(); x++ )
              {

                tmpData[ x ] = data( x, line, slice );
                tmpShift[ x ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().
                                                           applyCenteredInverse(
                                               tmpData, tmpShift, apodization );
              for ( x = 0; x < data.getSizeX(); x++ )
              {

                data( x, line, slice ) = tmpData[ x ];

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

      case gkg::ShiftedFourierTransform3d< T >::Y_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeY() );
          std::vector< T > tmpShift( data.getSizeY() );
          for ( slice = 0; slice < data.getSizeZ(); slice++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( line = 0; line < data.getSizeY(); line++ )
              {

                tmpData[ line ] = data( x, line, slice );
                tmpShift[ line ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().
                                                          applyCenteredInverse(
                                               tmpData, tmpShift, apodization );
              for ( line = 0; line < data.getSizeY(); line++ )
              {

                data( x, line, slice ) = tmpData[ line ];

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

      case gkg::ShiftedFourierTransform3d< T >::Z_AXIS:

        {

          int32_t x = 0, line = 0, slice = 0;
          std::vector< std::complex< T > > tmpData( data.getSizeZ() );
          std::vector< T > tmpShift( data.getSizeZ() );
          for ( line = 0; line < data.getSizeY(); line++ )
          {

            for ( x = 0; x < data.getSizeX(); x++ )
            {

              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                tmpData[ slice ] = data( x, line, slice );
                tmpShift[ slice ] = shift( x, line, slice );

              }
              gkg::ShiftedFourierTransform< T >::getInstance().
                                                           applyCenteredInverse(
                                               tmpData, tmpShift, apodization );
              for ( slice = 0; slice < data.getSizeZ(); slice++ )
              {

                data( x, line, slice ) = tmpData[ slice ];

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
             "void gkg::ShiftedFourierTransform3d< T >::applyCenteredInverse( "
             "gkg::Volume< std::complex< T > >& data, "
             "const gkg::Volume< T >& shift, "
             "gkg::ShiftedFourierTransform3d< T >::Axis axis, "
             "const gkg::Apodization< T >* apodization, "
             "bool verbose ) const" );

}


template class gkg::ShiftedFourierTransform3d< float >;
template class gkg::ShiftedFourierTransform3d< double >;
