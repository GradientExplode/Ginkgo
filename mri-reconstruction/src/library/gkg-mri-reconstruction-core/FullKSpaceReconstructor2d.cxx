#include <gkg-mri-reconstruction-core/FullKSpaceReconstructor2d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-mri-reconstruction-correction/ModulationTransferFunction.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>



template < class T >
gkg::FullKSpaceReconstructor2d< T >::FullKSpaceReconstructor2d()
                                  : gkg::Reconstructor< T >(),
                                    _xApodization( 0 ),
                                    _yApodization( 0 )
{
}


template < class T >
gkg::FullKSpaceReconstructor2d< T >::~FullKSpaceReconstructor2d()
{
}


template < class T >
void 
gkg::FullKSpaceReconstructor2d< T >::setRawLineCount( int32_t rawLineCount )
{

  _rawLineCount = rawLineCount;

}


template < class T >
int32_t gkg::FullKSpaceReconstructor2d< T >::getRawLineCount() const
{

  return _rawLineCount;

}


template < class T >
void gkg::FullKSpaceReconstructor2d< T >::setModulationTransferFunction(
                     const std::vector< std::vector<
                             std::complex< T > > >& modulationTransferFunction )
{

  _modulationTransferFunction = modulationTransferFunction;

}


template < class T >
const std::vector< std::vector< std::complex< T > > >&
gkg::FullKSpaceReconstructor2d< T >::getModulationTransferFunction() const
{

  return _modulationTransferFunction;

}


template < class T >
void gkg::FullKSpaceReconstructor2d< T >::setXApodization(
                                          const Apodization< T >* xApodization )
{

  _xApodization = xApodization;

}


template < class T >
void gkg::FullKSpaceReconstructor2d< T >::setYApodization(
                                          const Apodization< T >* yApodization )
{

  _yApodization = yApodization;

}


template < class T >
void gkg::FullKSpaceReconstructor2d< T >::reconstruct(
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

    // modulating each line with MTF
    if ( !_modulationTransferFunction.empty() && _rawLineCount )
    {

      if ( verbose )
      {

        std::cout << "apply MTF->" << std::flush;

      }
      gkg::ModulationTransferFunction< T >::getInstance().apply(
                                             reconstructed,
                                             _modulationTransferFunction,
                                             _rawLineCount,
                                             verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 11 ) << std::flush;

      }

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

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FullKSpaceReconstructor2d< T >::reconstruct( "
             "const gkg::Volume< std::complex< T > >& raw, "
             "gkg::Volume< std::complex< T > >& reconstructed, "
             "bool verbose  ) const" );

}


template class gkg::FullKSpaceReconstructor2d< float >;
template class gkg::FullKSpaceReconstructor2d< double >;

