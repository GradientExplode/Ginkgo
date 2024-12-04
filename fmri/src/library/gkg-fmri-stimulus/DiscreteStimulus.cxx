#include <gkg-fmri-stimulus/DiscreteStimulus.h>
#include <gkg-fmri-stimulus/ContinuousStimulus.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


gkg::DiscreteStimulus::DiscreteStimulus( const std::string& name,
                                         const gkg::Vector& times,
                                         const gkg::Vector& durations,
                                         const gkg::Vector& magnitudes )
                      : gkg::Stimulus( name ),
                        _times( times ),
                        _durations( durations ),
                        _magnitudes( magnitudes )
{
}


gkg::DiscreteStimulus::~DiscreteStimulus()
{
}


const gkg::Vector& gkg::DiscreteStimulus::getTimes() const
{

  return _times;

}


const gkg::Vector& gkg::DiscreteStimulus::getDurations() const
{

  return _durations;

}


const gkg::Vector& gkg::DiscreteStimulus::getMagnitudes() const
{

  return _magnitudes;

}


void gkg::DiscreteStimulus::sample( const gkg::Vector& times,
                                    gkg::Vector& result ) const
{

  try
  {

    // sanity check
    if ( times.getSize() != result.getSize() )
    {

      throw std::runtime_error( "inconsistent vector size(s)" );

    }

    // initializing result
    result.setZero();

    // looping over times
    int32_t t, r;
    double time = 0.0;
    double duration = 0.0;
    double magnitude = 0.0;
    for ( t = 0; t < _times.getSize(); t++ )
    {

      time = _times( t );
      duration = _durations( t ) + GKG_BOLD_STIMULUS_PERSISTENCE;
      magnitude = _magnitudes( t );

      for ( r = 0; r < times.getSize(); r++ )
      {

        if ( ( times( r ) >= time ) &&
             ( times( r ) < ( time + duration ) ) )
        {

          result( r ) = magnitude;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::DiscreteStimulus::sample( "
             "const gkg::Vector& times, "
             "gkg::Vector& result ) const" );

}


void gkg::DiscreteStimulus::convolve(
                                    const gkg::HemodynamicResponseFunction& hrf,
                                    const gkg::Vector& times,
                                    gkg::Vector& result ) const
{

  try
  {


    // sanity check
    if ( times.getSize() != result.getSize() )
    {

      throw std::runtime_error( "inconsistent vector size(s)" );

    }

    // processing minimum time
    gkg::MinimumFilter< gkg::Vector, double > minimumFilter;    
    double startingTime = 0.0;
    minimumFilter.filter( times, startingTime );

    // processing maximum time
    gkg::MaximumFilter< gkg::Vector, double > maximumFilter;    
    double maximumTime = 0.0;
    maximumFilter.filter( times, maximumTime );
    maximumTime += GKG_BOLD_HRF_DURATION;
 
    // processing time step and count
    double samplingPeriod = GKG_BOLD_HRF_CONVOLUTION_STEP;
    int32_t timeCount = ( int32_t )gkg::ceil( ( maximumTime - startingTime ) /
                                          samplingPeriod );


    // oversampling stimulus
    gkg::Vector newTimes( timeCount );
    int32_t t;
    for ( t = 0; t < timeCount; t++ )
    {

      newTimes( t ) = startingTime + t * samplingPeriod;

    }

    gkg::Vector newMagnitudes( timeCount );
    sample( newTimes, newMagnitudes );

    // processing oversampled stimulus as a continuous one
    gkg::ContinuousStimulus continuousStimulus( "_internal_",
                                                startingTime,
                                                samplingPeriod,
                                                newMagnitudes );
    continuousStimulus.convolve( hrf, times, result );

  }
  GKG_CATCH( "void gkg::DiscreteStimulus::convolve( "
             "const gkg::HemodynamicResponseFunction& hrf, "
             "const gkg::Vector& times, "
             "gkg::Vector& result ) const" );

}

