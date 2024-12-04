#include <gkg-mri-reconstruction-parallel/B1ReferenceFunctor.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>




template < class T >
gkg::B1ReferenceFunctor< T >::B1ReferenceFunctor()
{
}


template < class T >
gkg::B1ReferenceFunctor< T >::~B1ReferenceFunctor()
{
}


template < class T >
void gkg::B1ReferenceFunctor< T >::process(
                             const gkg::Volume< std::complex< T > >& coilVolume,
                             gkg::Volume< std::complex< T > >& b1Reference,
                             bool verbose ) const
{

  try
  {

    // sanity checks
    if ( coilVolume.getSizeT() < 2 )
    {

      throw std::runtime_error(
                              "coil volume must have a T size greater than 1" );

    }

    // collecting size(s)
    int32_t sizeX = coilVolume.getSizeX();
    int32_t sizeY = coilVolume.getSizeY();
    int32_t sizeZ = coilVolume.getSizeZ();
    int32_t coilCount = coilVolume.getSizeT();

    // allocating stuff
    b1Reference = gkg::Volume< std::complex< T > >( sizeX, sizeY, sizeZ );
    b1Reference.getHeader() = coilVolume.getHeader();
    b1Reference.getHeader()[ "sizeT" ] = 1;
    b1Reference.getHeader()[ "resolutionT" ] = 1.0;

    // initializing result
    b1Reference.fill( std::complex< T >( 0, 0 ) );

    // processing reference
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

            b1Reference( x, y, z ) += std::norm( coilVolume( x, y, z, c ) );

          }
          b1Reference( x, y, z ) = std::sqrt( b1Reference( x, y, z ) );

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::B1ReferenceFunctor::process( "
             "const gkg::Volume< std::complex< T > >& coilVolume, "
             "gkg::Volume< std::complex< T > >& b1Reference, "
             "bool verbose ) const" );

}


// force instanciations
template class gkg::B1ReferenceFunctor< float >;
template class gkg::B1ReferenceFunctor< double >;
