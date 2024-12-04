#include <gkg-mri-reconstruction-correction/B0InhomogeneityCorrection.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


template < class T >
gkg::B0InhomogeneityCorrection< T >::B0InhomogeneityCorrection()
{
}


template < class T >
gkg::B0InhomogeneityCorrection< T >::~B0InhomogeneityCorrection()
{
}


template < class T >
void gkg::B0InhomogeneityCorrection< T >::apply(
                             gkg::Volume< std::complex<T > >& data,
                             const gkg::Volume< T >& shiftMap,
                             gkg::B0InhomogeneityCorrection< T >::Method method,
                             bool verbose ) const
{


  try
  {

    // sanity checks
    if ( data.getSizeT() != 1 )
    {

      throw std::runtime_error( "distorted data must have T size equal to 1" );

    }

    if ( shiftMap.getSizeT() != 1 )
    {

      throw std::runtime_error( "shift map must have T size equal to 1" );

    }

    double dataResolutionX = 0.0;
    double dataResolutionY = 0.0;
    double dataResolutionZ = 0.0;

    double shiftMapResolutionX = 0.0;
    double shiftMapResolutionY = 0.0;
    double shiftMapResolutionZ = 0.0;

    if ( data.getHeader().hasAttribute( "resolutionX" ) &&
         data.getHeader().hasAttribute( "resolutionY" ) &&
         data.getHeader().hasAttribute( "resolutionZ" ) )
    {
         
      data.getHeader().getAttribute( "resolutionX", dataResolutionX );
      data.getHeader().getAttribute( "resolutionY", dataResolutionY );
      data.getHeader().getAttribute( "resolutionZ", dataResolutionZ );

    }

    if ( shiftMap.getHeader().hasAttribute( "resolutionX" ) &&
         shiftMap.getHeader().hasAttribute( "resolutionY" ) &&
         shiftMap.getHeader().hasAttribute( "resolutionZ" ) )
    {
         
      shiftMap.getHeader().getAttribute( "resolutionX", shiftMapResolutionX );
      shiftMap.getHeader().getAttribute( "resolutionY", shiftMapResolutionY );
      shiftMap.getHeader().getAttribute( "resolutionZ", shiftMapResolutionZ );

    }

    if ( ( data.getSizeX() != shiftMap.getSizeX() ) ||
         ( data.getSizeY() != shiftMap.getSizeY() ) ||
         ( data.getSizeZ() != shiftMap.getSizeZ() ) ||
         ( dataResolutionX != shiftMapResolutionX ) ||
         ( dataResolutionY != shiftMapResolutionY ) ||
         ( dataResolutionZ != shiftMapResolutionZ ) )
    {

      throw std::runtime_error(
                          "incompatible distorted data and shift map size(s)" );

    }


    switch ( method )
    {

      case gkg::B0InhomogeneityCorrection< T >::PixelShiftMethod:
        applyPixelShiftMethod( data, shiftMap, verbose );
        break;

      case gkg::B0InhomogeneityCorrection< T >::
                                             PixelShiftIntensityCorrectedMethod:
        applyPixelShiftIntensityCorrectedMethod( data, shiftMap, verbose );
        break;

      case gkg::B0InhomogeneityCorrection< T >::PhaseConjugateMethod:
        applyPhaseConjugateMethod( data, shiftMap, verbose );
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::B0InhomogeneityCorrection< T >::apply( "
             "gkg::Volume< std::complex<T > >& data, "
             "const gkg::Volume< T >& shiftMap, "
             "gkg::B0InhomogeneityCorrection< T >::Method method, "
             "bool verbose ) const" );

}


template < class T >
void gkg::B0InhomogeneityCorrection< T >::applyPixelShiftMethod(
                                          gkg::Volume< std::complex<T > >& data,
                                          const gkg::Volume< T >& shiftMap,
                                          bool verbose ) const
{


  try
  {

    int32_t sizeX = data.getSizeX();
    int32_t sizeY = data.getSizeY();
    int32_t sliceCount = data.getSizeZ();

    if ( verbose )
    {

      std::cout << "resampling ->" << std::flush;

    }    

    gkg::Volume< std::complex< T > > dataCopy( data );

    int32_t x, y, slice;
    T yOrigReal, weight;
    int32_t y1, y2;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          yOrigReal = ( T )y + shiftMap( x, y, slice );

          y1 = ( int32_t )std::floor( yOrigReal );
          y2 = y1 + 1;
          weight = yOrigReal - ( T )y1;

          if ( ( y1 < 0 ) || ( y2 > sizeY - 1 ) )
          {

            data( x, y, slice ) = 0;

          }
          else
          {

            data( x, y, slice ) = ( ( T )1.0 - weight ) *
                                    dataCopy( x, y1, slice ) +
                                  weight * dataCopy( x, y2, slice );

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 34 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::B0InhomogeneityCorrection< T >::applyPixelShiftMethod( "
             "gkg::Volume< std::complex<T > >& data, "
             "const gkg::Volume< T >& shiftMap, "
             "bool verbose ) const" );

}


template < class T >
void 
gkg::B0InhomogeneityCorrection< T >::applyPixelShiftIntensityCorrectedMethod(
                                          gkg::Volume< std::complex<T > >& data,
                                          const gkg::Volume< T >& shiftMap,
                                          bool verbose ) const
{


  try
  {

    // applying shifting
    applyPixelShiftMethod( data, shiftMap, verbose );

    // applying intensity correction
    int32_t sizeX = data.getSizeX();
    int32_t sizeY = data.getSizeY();
    int32_t sliceCount = data.getSizeZ();
    int32_t x, y, slice;
    T yDeviate = 0;

    if ( verbose )
    {

      std::cout << "Intensity correction ->" << std::flush;

    }
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( y == 0 )
          {

            yDeviate = ( shiftMap( x, y + 2, slice ) -
                         shiftMap( x, y, slice ) ) / 2;

          }
          else if ( y == sizeY - 1 )
          {

            yDeviate = ( shiftMap( x, y, slice ) -
                         shiftMap( x, y - 2, slice ) ) / 2;

          }
          else
          {

            yDeviate = ( shiftMap( x, y + 1, slice ) -
                         shiftMap( x, y - 1, slice ) ) / 2;

          }
          data( x, y, slice ) *= ( 1 + yDeviate );

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 44 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::B0InhomogeneityCorrection< T >::"
             "applyPixelShiftIntensityCorrectedMethod( "
             "gkg::Volume< std::complex<T > >& data, "
             "const gkg::Volume< T >& shiftMap, "
             "bool verbose ) const" );

}


template < class T >
void gkg::B0InhomogeneityCorrection< T >::applyPhaseConjugateMethod(
                                          gkg::Volume< std::complex<T > >& data,
                                          const gkg::Volume< T >& shiftMap,
                                          bool verbose ) const
{


  try
  {

    int32_t sizeX = data.getSizeX();
    int32_t sizeY = data.getSizeY();
    int32_t sliceCount = data.getSizeZ();
    int32_t x, y, slice, kx, ky;

    gkg::Volume< std::complex< T > > spaceXKy( sizeX, sizeY, sliceCount );
    spaceXKy.fill( std::complex< T >( 0, 0 ) );

    gkg::Volume< std::complex< T > > spaceKxKy( sizeX, sizeY, sliceCount );
    spaceKxKy.fill( std::complex< T >( 0, 0 ) );

    // converting data to (x,ky) space
    if ( verbose )
    {

      std::cout << "FFT Y ->" << std::flush;

    }    
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }
      for ( ky = 0; ky < sizeY; ky++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            spaceXKy( x, ky, slice ) += 
              data( x, y, slice ) *
              std::polar( ( T )1.0, ( T )( -2.0 * M_PI * ( ky * y ) / sizeY ) );

          }
 
        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 29 ) << std::flush;

    }

    // converting data to (kx,ky) space
    if ( verbose )
    {

      std::cout << "FFT X ->" << std::flush;

    }    
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }
      for ( ky = 0; ky < sizeY; ky++ )
      {

        for ( kx = 0; kx < sizeX; kx++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            spaceKxKy( kx, ky, slice ) += 
              spaceXKy( x, ky, slice ) *
              std::polar( ( T )1.0, ( T )( -2.0 * M_PI * ( kx * x ) / sizeX ) );

          }
 
        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 29 ) << std::flush;

    }

    // converting back (x,ky) space
    if ( verbose )
    {

      std::cout << "IFFT X ->" << std::flush;

    }    
    spaceXKy.fill( std::complex< T >( 0, 0 ) );
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }
      for ( x = 0; x < sizeX; x++ )
      {

        for ( ky = 0; ky < sizeY; ky++ )
        {

          for ( kx = 0; kx < sizeX; kx++ )
          {

            spaceXKy( x, ky, slice ) += 
              spaceKxKy( kx, ky, slice ) *
              std::polar( ( T )1.0, ( T )( 2.0 * M_PI * ( kx * x ) / sizeX ) );

          }
 
        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

    // converting back (x,y) space introducing unwarping process
    if ( verbose )
    {

      std::cout << "IFFT Y ->" << std::flush;

    }    
    data.fill( std::complex< T >( 0, 0 ) );

    T kyPrime = 0;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          for ( ky = 0; ky < sizeY; ky++ )
          {

            kyPrime = ( ky + sizeY / 2 ) % sizeY;
            data( x, y, slice ) += 
              spaceXKy( x, ky, slice ) *
              std::polar( ( T )1.0, ( T )( 2.0 * M_PI * ( ky * y +
                                kyPrime * shiftMap( x, y, slice ) ) / sizeY ) );

          }
 
        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }


    // scaling
    if ( verbose )
    {

      std::cout << "scaling ->" << std::flush;

    }    
    T scale = ( ( T )sizeX * ( T )sizeY );
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          data( x, y, slice ) /= scale;

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 31 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::B0InhomogeneityCorrection< T >::"
             "applyPhaseConjugateMethod( "
             "gkg::Volume< std::complex<T > >& data, "
             "const gkg::Volume< T >& shiftMap, "
             "bool verbose ) const" );

}



template class gkg::B0InhomogeneityCorrection< float >;
template class gkg::B0InhomogeneityCorrection< double >;
