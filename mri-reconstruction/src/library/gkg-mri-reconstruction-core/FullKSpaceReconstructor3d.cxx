#include <gkg-mri-reconstruction-core/FullKSpaceReconstructor3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>



template < class T >
gkg::FullKSpaceReconstructor3d< T >::FullKSpaceReconstructor3d()
                                    : gkg::Reconstructor< T >(),
                                      _xApodization( 0 ),
                                      _yApodization( 0 ),
                                      _zApodization( 0 )
{
}


template < class T >
gkg::FullKSpaceReconstructor3d< T >::~FullKSpaceReconstructor3d()
{
}


template < class T >
void gkg::FullKSpaceReconstructor3d< T >::setXApodization(
                                          const Apodization< T >* xApodization )
{

  _xApodization = xApodization;

}


template < class T >
void gkg::FullKSpaceReconstructor3d< T >::setYApodization(
                                          const Apodization< T >* yApodization )
{

  _yApodization = yApodization;

}


template < class T >
void gkg::FullKSpaceReconstructor3d< T >::setZApodization(
                                          const Apodization< T >* zApodization )
{

  _zApodization = zApodization;

}


template < class T >
void gkg::FullKSpaceReconstructor3d< T >::reconstruct(
                                const gkg::Volume< std::complex< T > >& raw,
                                gkg::Volume< std::complex< T > >& reconstructed,
                                bool verbose  ) const
{

  try
  {

    // sanity checks
    if ( raw.getSizeT() != 1 )
    {

      throw std::runtime_error( "raw data has sizeT different from 1" );

    }

    // allocating output
    if ( &raw != &reconstructed )
    {

      reconstructed = raw;

    }

    // processing Fourier transform along X axis
    if ( verbose )
    {

      std::cout << "FFT X->" << std::flush;

    }
    gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                           reconstructed,
                                           gkg::FourierTransform3d< T >::X_AXIS,
                                           _xApodization,
                                           verbose );
    if ( verbose )
    {

      std::cout << gkg::Eraser( 7 ) << std::flush;

    }

    // processing Fourier transform along Y axis
    if ( verbose )
    {

      std::cout << "FFT Y->" << std::flush;

    }
    gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                           reconstructed,
                                           gkg::FourierTransform3d< T >::Y_AXIS,
                                           _yApodization,
                                           verbose );
    if ( verbose )
    {

      std::cout << gkg::Eraser( 7 ) << std::flush;

    }

    // processing Fourier transform along Z axis
    if ( verbose )
    {

      std::cout << "FFT Z->" << std::flush;

    }
    gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                           reconstructed,
                                           gkg::FourierTransform3d< T >::Z_AXIS,
                                           _zApodization,
                                           verbose );
    if ( verbose )
    {

      std::cout << gkg::Eraser( 7 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FullKSpaceReconstructor3d< T >::reconstruct( "
             "const gkg::Volume< std::complex< T > >& raw, "
             "gkg::Volume< std::complex< T > >& reconstructed, "
             "bool verbose  ) const" );

}


template class gkg::FullKSpaceReconstructor3d< float >;
template class gkg::FullKSpaceReconstructor3d< double >;

