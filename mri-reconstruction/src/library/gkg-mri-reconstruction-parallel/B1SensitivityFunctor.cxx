#include <gkg-mri-reconstruction-parallel/B1SensitivityFunctor.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <cmath>
#include <iostream>
#include <iomanip>




template < class T >
gkg::B1SensitivityFunctor< T >::B1SensitivityFunctor( const T& epsilon )
                               : _epsilon( epsilon )
{
}


template < class T >
gkg::B1SensitivityFunctor< T >::~B1SensitivityFunctor()
{
}


template < class T >
void gkg::B1SensitivityFunctor< T >::process(
                         const gkg::Volume< std::complex< T > >& coilVolume,
                         const gkg::Volume< std::complex< T > >& b1Reference,
                         gkg::Volume< std::complex< T > >& b1Sensitivity,
                         bool verbose ) const
{

  try
  {

    // collecting size(s)
    int32_t sizeX = coilVolume.getSizeX();
    int32_t sizeY = coilVolume.getSizeY();
    int32_t sizeZ = coilVolume.getSizeZ();
    int32_t coilCount = coilVolume.getSizeT();

    // sanity checks
    if ( coilCount < 2 )
    {

      throw std::runtime_error(
                              "coil volume must have a T size greater than 1" );

    }
    if ( ( b1Reference.getSizeX() != sizeX ) ||
         ( b1Reference.getSizeY() != sizeY ) ||
         ( b1Reference.getSizeZ() != sizeZ ) ||
         ( b1Reference.getSizeT() != 1 ) )
    {

      throw std::runtime_error(
                               "B1 reference not consistent with coil volume ");

    }
         
    // allocating stuff
    b1Sensitivity = gkg::Volume< std::complex< T > >( sizeX,
                                                      sizeY,
                                                      sizeZ,
                                                      coilCount );
    b1Sensitivity.getHeader() = coilVolume.getHeader();

    // processing sensitivity maps
    int32_t x, y, z, c;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }
 
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          for ( c = 0; c < coilCount; c++ )
          {

            if ( std::abs( b1Reference( x, y, z ) ) < _epsilon )
            {

              b1Sensitivity( x, y, z, c ) = std::complex< T >( 0.0, 0.0 );

            }
            else
            {

              b1Sensitivity( x, y, z, c ) = coilVolume( x, y, z, c ) /
                                            b1Reference( x, y, z );
              if ( std::abs( b1Sensitivity( x, y, z, c ) ) > 1.0 )
              {

                throw std::runtime_error( "sensitivity greater than 1.0" );

              }

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::B1SensitivityFunctor::process( "
             "const gkg::Volume< std::complex< T > >& coilVolume, "
             "const gkg::Volume< std::complex< T > >& b1Reference, "
             "gkg::Volume< std::complex< T > >& b1Sensitivity, "
             "bool verbose ) const" );

}


template class gkg::B1SensitivityFunctor< float >;
template class gkg::B1SensitivityFunctor< double >;
