#include <gkg-fmri-stimulus/ContinuousStimulus.h>
#include <gkg-processing-numericalanalysis/CubicSplineTransform1d.h>
#include <gkg-processing-algobase/Convolution1d_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


gkg::ContinuousStimulus::ContinuousStimulus( const std::string& name,
                                             double startingTime,
                                             double samplingPeriod,
                                             const gkg::Vector& magnitudes )
                        : gkg::Stimulus( name ),
                          _startingTime( startingTime ),
                          _samplingPeriod( samplingPeriod ),
                          _magnitudes( magnitudes )
{
}


gkg::ContinuousStimulus::~ContinuousStimulus()
{
}


double gkg::ContinuousStimulus::getStartingTime() const
{

  return _startingTime;

}


double gkg::ContinuousStimulus::getSamplingPeriod() const
{

  return _samplingPeriod;

}


const gkg::Vector& gkg::ContinuousStimulus::getMagnitudes() const
{

  return _magnitudes;

}


void gkg::ContinuousStimulus::sample( const gkg::Vector& times,
                                      gkg::Vector& result ) const
{

  try
  {

    // sanity check
    if ( times.getSize() != result.getSize() )
    {

      throw std::runtime_error( "inconsistent vector size(s)" );

    }

    // performing spline transform
    gkg::CubicSplineTransform1d cubicSplineTransform1d( _magnitudes );

    // processing result
    int32_t t;
    for ( t = 0; t < times.getSize(); t++ )
    {

      result( t ) = cubicSplineTransform1d.getValue(
                                                ( times( t ) - _startingTime ) /
                                                _samplingPeriod ); 

    }


  }
  GKG_CATCH( "void gkg::ContinuousStimulus::sample( "
             "const gkg::Vector& times, "
             "gkg::Vector& result ) const" );

}


void gkg::ContinuousStimulus::convolve(
                                    const gkg::HemodynamicResponseFunction& hrf,
                                    const gkg::Vector& times,
                                    gkg::Vector& result ) const
{

  try
  {

    // sanity check
    int32_t timeCount = times.getSize();
    if ( timeCount != result.getSize() )
    {

      throw std::runtime_error( "inconsistent vector size(s)" );

    }

    // performing spline transform
    int32_t magnitudeCount = _magnitudes.getSize();
    gkg::CubicSplineTransform1d cubicSplineTransform1d( _magnitudes );

    // processing HRF kernel
    int32_t hrfKernelCount =
                      ( int32_t )gkg::ceil( GKG_BOLD_HRF_DURATION /
                                            GKG_BOLD_HRF_CONVOLUTION_STEP ) + 1;
    gkg::Vector hrfKernel( hrfKernelCount );
    int32_t i;
    for ( i = 0; i < hrfKernelCount; i++ )
    {

      hrfKernel( i ) = hrf.getValue( i * GKG_BOLD_HRF_CONVOLUTION_STEP );

    }

    // processing offset
    int32_t offset = ( int32_t )gkg::ceil( 2.0 * _samplingPeriod /
                                           GKG_BOLD_HRF_CONVOLUTION_STEP );

    // processing cubic spline kernel
    int32_t cubicSplineKernelCount = 
                     ( int32_t )gkg::ceil( 4.0 * _samplingPeriod /
                                           GKG_BOLD_HRF_CONVOLUTION_STEP ) + 1;

    gkg::Vector cubicSplineKernel( cubicSplineKernelCount );
    for ( i = 0; i < cubicSplineKernelCount; i++ )
    {

      cubicSplineKernel( i ) = gkg::CubicSplineTransform1d::cubicSplineBasis(
                                ( i - offset ) * GKG_BOLD_HRF_CONVOLUTION_STEP /
                                _samplingPeriod );

    }

    // convolving HRF kernel and cubic spline basis
    // convolution( i ) = ( hrf * cubicspline )( ( i - offset ) * convol_step )
    int32_t convolutionCount = hrfKernelCount + cubicSplineKernelCount - 1;
    gkg::Vector convolution( convolutionCount );
    gkg::Convolution1d convolution1d;
    convolution1d.convolve( hrfKernel, cubicSplineKernel, convolution );
    gkg::Scaler< gkg::Vector > scaler( GKG_BOLD_HRF_CONVOLUTION_STEP );
    scaler.scale( convolution );

    // reconstructing the signal
    int32_t t, k, index;
    for ( t = 0; t < timeCount; t++ )
    {

      result( t ) = 0.0;
      for ( k = 0; k < magnitudeCount; k++ )
      {

        index = ( int32_t )gkg::round(
                  offset +
                  ( times( t ) - _startingTime - _samplingPeriod * k ) /
                  GKG_BOLD_HRF_CONVOLUTION_STEP );

        if ( ( index >= 0 ) && ( index < convolutionCount ) )
        {

          result( t ) += cubicSplineTransform1d.getCoefficient( k ) *
                         convolution( index );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ContinuousStimulus::convolve( "
             "const gkg::HemodynamicResponseFunction& hrf, "
             "const gkg::Vector& times, "
             "gkg::Vector& result ) const" );

}


