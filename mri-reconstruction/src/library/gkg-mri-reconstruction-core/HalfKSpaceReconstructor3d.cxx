#include <gkg-mri-reconstruction-core/HalfKSpaceReconstructor3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-mri-reconstruction-correction/ModulationTransferFunction.h>
#include <gkg-mri-reconstruction-correction/EchoVolumarPhaseCorrection.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-processing-signal/ShiftedFourierTransform3d.h>
#include <gkg-processing-signal/Scrolling3d.h>
#include <gkg-mri-reconstruction-signal/HalfFourierTransform3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


#define DEFAULT_HALF_FOURIER_THRESHOLD  0.35


template < class T >
gkg::HalfKSpaceReconstructor3d< T >::HalfKSpaceReconstructor3d()
                                    : gkg::Reconstructor< T >(),
                                      _overScanCountY( 0 ),
                                      _overScanCountZ( 0 ),
                                      _rawLineCount( 0 ),
                                      _rawSliceCount( 0 ),
                                      _halfFourierThreshold(
                                         DEFAULT_HALF_FOURIER_THRESHOLD ),
                                      _yScrollingRatio( 0.0 ),
                                      _zScrollingRatio( 0.0 ),
                                      _xApodization( 0 ),
                                      _yApodization( 0 ),
                                      _zApodization( 0 )
{
}


template < class T >
gkg::HalfKSpaceReconstructor3d< T >::~HalfKSpaceReconstructor3d()
{
}


template < class T >
void 
gkg::HalfKSpaceReconstructor3d< T >::setOverScanCountY( int32_t overScanCountY )
{

  _overScanCountY = overScanCountY;

}


template < class T >
int32_t gkg::HalfKSpaceReconstructor3d< T >::getOverScanCountY() const
{

  return _overScanCountY;

}


template < class T >
void 
gkg::HalfKSpaceReconstructor3d< T >::setOverScanCountZ( int32_t overScanCountZ )
{

  _overScanCountZ = overScanCountZ;

}


template < class T >
int32_t gkg::HalfKSpaceReconstructor3d< T >::getOverScanCountZ() const
{

  return _overScanCountZ;

}


template < class T >
void 
gkg::HalfKSpaceReconstructor3d< T >::setRawLineCount( int32_t rawLineCount )
{

  _rawLineCount = rawLineCount;

}


template < class T >
int32_t gkg::HalfKSpaceReconstructor3d< T >::getRawLineCount() const
{

  return _rawLineCount;

}


template < class T >
void 
gkg::HalfKSpaceReconstructor3d< T >::setRawSliceCount( int32_t rawSliceCount )
{

  _rawSliceCount = rawSliceCount;

}


template < class T >
int32_t gkg::HalfKSpaceReconstructor3d< T >::getRawSliceCount() const
{

  return _rawSliceCount;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setHalfFourierThreshold(
                                    double halfFourierThreshold )
{

  _halfFourierThreshold = halfFourierThreshold;

}


template < class T >
double gkg::HalfKSpaceReconstructor3d< T >::getHalfFourierThreshold() const
{

  return _halfFourierThreshold;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setModulationTransferFunction(
                     const std::vector< std::vector<
                             std::complex< T > > >& modulationTransferFunction )
{

  _modulationTransferFunction = modulationTransferFunction;

}


template < class T >
const std::vector< std::vector< std::complex< T > > >&
gkg::HalfKSpaceReconstructor3d< T >::getModulationTransferFunction() const
{

  return _modulationTransferFunction;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setPhaseCorrection(
                 const std::vector< std::vector< T > >& constantPhaseCorrection,
                 const std::vector< std::vector< T > >& linearPhaseCorrection )
{

  _constantPhaseCorrection = constantPhaseCorrection;
  _linearPhaseCorrection = linearPhaseCorrection;

}


template < class T >
const std::vector< std::vector< T > >&
gkg::HalfKSpaceReconstructor3d< T >::getConstantPhaseCorrection() const
{

  return _constantPhaseCorrection;

}


template < class T >
const std::vector< std::vector< T > >&
gkg::HalfKSpaceReconstructor3d< T >::getLinearPhaseCorrection() const
{

  return _linearPhaseCorrection;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setYScrollingRatio(
                                                      const T& yScrollingRatio )
{

  _yScrollingRatio = yScrollingRatio;

}


template < class T >
const T& gkg::HalfKSpaceReconstructor3d< T >::getYScrollingRatio() const
{

  return _yScrollingRatio;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setZScrollingRatio(
                                                      const T& zScrollingRatio )
{

  _zScrollingRatio = zScrollingRatio;

}


template < class T >
const T& gkg::HalfKSpaceReconstructor3d< T >::getZScrollingRatio() const
{

  return _zScrollingRatio;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setXApodization(
                                     const gkg::Apodization< T >* xApodization )
{

  _xApodization = xApodization;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setYApodization(
                                     const gkg::Apodization< T >* yApodization )
{

  _yApodization = yApodization;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::setZApodization(
                                     const gkg::Apodization< T >* zApodization )
{

  _zApodization = zApodization;

}


template < class T >
void gkg::HalfKSpaceReconstructor3d< T >::reconstruct(
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
    if ( !_modulationTransferFunction.empty() )
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
    else if ( !_constantPhaseCorrection.empty() &&
              !_linearPhaseCorrection.empty() )
    {

      if ( verbose )
      {

        std::cout << "apply EVI PC->" << std::flush;

      }
      gkg::EchoVolumarPhaseCorrection< T >::getInstance().apply(
                                             reconstructed,
                                             _constantPhaseCorrection,
                                             _linearPhaseCorrection,
                                             _rawLineCount,
                                             _rawSliceCount,
                                             verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 14 ) << std::flush;

      }

    }

    // applying scrolling along Y axis
    if ( _yScrollingRatio != 0.0 )
    {

      if ( verbose )
      {

        std::cout << "apply Scrolling3d along Y->" << std::flush;

      }
      gkg::Scrolling3d< T >::getInstance().apply(
                                             reconstructed,
                                             _yScrollingRatio,
                                             gkg::Scrolling3d< T >::Y_AXIS,
                                             verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 27 ) << std::flush;

      }

    }

    // processing Fourier transform along Y axis
    // note: if the number of overscan is greater that 35% of the raw Y size,
    //       then we use standard Fourier transform instead of Half Fourier one
    if ( _overScanCountY > _halfFourierThreshold * _rawLineCount )
    {

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
    else
    {

      if ( verbose )
      {

        std::cout << "HFR Y->" << std::flush;

      }
      gkg::HalfFourierTransform3d< T >::getInstance().applyCenteredInverse(
                                     reconstructed,
                                     gkg::FourierTransform3d< T >::Y_AXIS,
                                     _rawLineCount,
                                     _overScanCountY,
                                     _yApodization,
                                     verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 7 ) << std::flush;

      }

    }

    // applying scrolling along Z axis
    if ( _zScrollingRatio != 0.0 )
    {

      if ( verbose )
      {

        std::cout << "apply Scrolling3d along Z->" << std::flush;

      }
      gkg::Scrolling3d< T >::getInstance().apply(
                                             reconstructed,
                                             _zScrollingRatio,
                                             gkg::Scrolling3d< T >::Z_AXIS,
                                             verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 27 ) << std::flush;

      }

    }

    // processing Fourier transform along Z axis
    // note: if the number of overscan is greater that 35% of the raw Z size,
    //       then we use standard Fourier transform instead of Half Fourier one
    if ( _overScanCountZ > _halfFourierThreshold * _rawSliceCount )
    {

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
    else
    {

      if ( verbose )
      {

        std::cout << "HFR Z->" << std::flush;

      }
      gkg::HalfFourierTransform3d< T >::getInstance().applyCenteredInverse(
                                     reconstructed,
                                     gkg::FourierTransform3d< T >::Z_AXIS,
                                     _rawSliceCount,
                                     _overScanCountZ,
                                     _zApodization,
                                     verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 7 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HalfKSpaceReconstructor3d< T >::reconstruct( "
             "const gkg::Volume< std::complex< T > >& raw, "
             "gkg::Volume< std::complex< T > >& reconstructed, "
             "bool verbose  ) const" );

}


template class gkg::HalfKSpaceReconstructor3d< float >;
template class gkg::HalfKSpaceReconstructor3d< double >;


#undef DEFAULT_HALF_FOURIER_THRESHOLD
