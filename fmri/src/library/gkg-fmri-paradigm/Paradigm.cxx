#include <gkg-fmri-paradigm/Paradigm.h>
#include <gkg-fmri-stimulus/DiscreteStimulus.h>
#include <gkg-fmri-stimulus/ContinuousStimulus.h>
#include <gkg-fmri-hrf/CanonicalHRF.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>
#include <map>


#define GKG_PARADIGM_TINY    1e-300
#define GKG_HRF_THRESHOLD    0.01


gkg::Paradigm::Paradigm()
              : gkg::HeaderedObject(),
                _regressorCount( 0 ),
                _driftPolynomOrder( 0 ),
                _firstCall( true )
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Paradigm" ) );

}


gkg::Paradigm::~Paradigm()
{

  std::list< gkg::Stimulus* >::iterator s = _stimuli.begin(),
                                        se = _stimuli.end();
  while ( s != se )
  {

    delete *s;
    ++ s;

  }

}


void gkg::Paradigm::addDiscreteStimulus( const std::string& name,
                                         const gkg::Vector& times,
                                         const gkg::Vector& durations,
                                         const gkg::Vector& magnitudes )
{

  try
  {

    _stimuli.push_back( new gkg::DiscreteStimulus( name,
                                                   times,
                                                   durations,
                                                   magnitudes ) );
    ++ _regressorCount;

  }
  GKG_CATCH( "void gkg::Paradigm::addDiscreteStimulus( "
             "const std::string& name, "
             "const gkg::Vector& times, "
             "const gkg::Vector& durations, "
             "const gkg::Vector& magnitudes )" );

}


void gkg::Paradigm::addContinuousStimulus( const std::string& name,
                                           double startingTime,
                                           double samplingPeriod,
                                           const gkg::Vector& magnitudes )
{

  try
  {

    _stimuli.push_back( new gkg::ContinuousStimulus( name,
                                                     startingTime,
                                                     samplingPeriod,
                                                     magnitudes ) );
    ++ _regressorCount;

  }
  GKG_CATCH( "void gkg::Paradigm::addContinuousStimulus( "
             "const std::string& name, "
             "double startingTime, "
             "double samplingPeriod, "
             "const gkg::Vector& magnitudes )" );

}


int32_t gkg::Paradigm::getRegressorCount() const
{

  return _regressorCount;

}


const std::list< gkg::Stimulus* >& gkg::Paradigm::getStimuli() const
{

  return _stimuli;

}


void gkg::Paradigm::getDesignMatrix(
                             const gkg::Vector& times,
                             const gkg::HemodynamicResponseFunction& hrf,
                             int32_t driftPolynomOrder,
                             gkg::Matrix& designMatrix,
                             bool verbose )
{

  try
  {

    // collecting time information
    int32_t timeCount = times.getSize();
    double minimumTime = times( 0 );
    double maximumTime = times( 0 );
    int32_t t;
    for ( t = 1; t < timeCount; t++ )
    {

      if ( times( t ) < minimumTime )
      {

        minimumTime = times( t );

      }
      else if ( times( t ) > maximumTime )
      {

        maximumTime = times( t );

      }

    }

    if ( verbose )
    {

      std::cout << "minimum time : " << minimumTime << " s" << std::endl;
      std::cout << "maximum time : " << maximumTime << " s" << std::endl;
      std::cout << "time count : " << timeCount << std::endl;

    }

    // allocating design matrix
    // regressor count = 1                 -> constant regressor
    //                   driftPolynomOrder -> drift correction

    // Flag used to handle several successive calls to getDesignMatrix function
    if( _firstCall )
    {

      _regressorCount += 1 + driftPolynomOrder;
      _firstCall = false;

    }
    else if( _driftPolynomOrder != driftPolynomOrder )
    {

      // Do not take previous driftPolynom value into account
      _regressorCount += driftPolynomOrder - _driftPolynomOrder;

    }

    _driftPolynomOrder = driftPolynomOrder;

    if ( verbose )
    {

      std::cout << "stimuli regressor count : "
                << _regressorCount - 1 - _driftPolynomOrder << std::endl;

    }

    designMatrix.reallocate( timeCount, _regressorCount );

    if ( verbose )
    {

      std::cout << "drift polynom order : " << driftPolynomOrder << std::endl;
      std::cout << "global regressor count : " << _regressorCount << std::endl;

    }

    // allocating column
    gkg::Vector column( timeCount );

    // looping over stimuli
    int32_t c = 0;
    std::list< gkg::Stimulus* >::const_iterator s = _stimuli.begin(),
                                                se = _stimuli.end();
    while ( s != se )
    {

      gkg::Stimulus* stimulus = *s;
      stimulus->convolve( hrf, times, column );
      designMatrix.setColumn( c, column ); 
      ++ s;
      ++ c;

    }

    // constant regressor
    column.fill( 1.0 );
    designMatrix.setColumn( c, column ); 
    ++ c;

    // drift regressors
    double slope = 1.0 / std::max( maximumTime - minimumTime,
                                   GKG_PARADIGM_TINY );
    int32_t o;
    for ( o = 1; o <= driftPolynomOrder; o++ )
    {

      for ( t = 0; t < timeCount; t++ )
      {

        column( t ) = std::pow( slope * ( times( t ) - minimumTime ),
                                ( double )o );

      }
      designMatrix.setColumn( c, column ); 
      ++ c;

    }

  }
  GKG_CATCH( "void gkg::Paradigm::getDesignMatrix( "
             "const gkg::Vector& times, "
             "const gkg::HemodynamicResponseFunction& hrf, "
             "int32_t driftPolynomOrder, "
             "gkg::Matrix& designMatrix, "
             "bool verbose )" );

}


void gkg::Paradigm::getDesignLabels( const gkg::Vector& times,
                                     std::vector< std::string >& labels )
{

  try
  {

    gkg::CanonicalHRF hrf( GKG_BOLD_CANONICAL_HRF_DELAY,
                           GKG_BOLD_CANONICAL_HRF_DELAY_UNDERSHOOT,
                           GKG_BOLD_CANONICAL_HRF_DISPERSION,
                           GKG_BOLD_CANONICAL_HRF_DISPERSION_UNDERSHOOT,
                           GKG_BOLD_CANONICAL_HRF_RATIO );

    // collecting time information
    int32_t timeCount = times.getSize();
    double minimumTime = times( 0 );
    double maximumTime = times( 0 );
    int32_t t;
    for ( t = 1; t < timeCount; t++ )
    {

      if ( times( t ) < minimumTime )
      {

        minimumTime = times( t );

      }
      else if ( times( t ) > maximumTime )
      {

        maximumTime = times( t );

      }

    }

    // allocating labels
    std::vector< std::string > regressors( _regressorCount );
    labels.clear();
    labels.resize( timeCount, "none" );

    // get design matrix without drifts and constant regressor
    gkg::Matrix designMatrix( timeCount, _regressorCount );

    // allocating column
    gkg::Vector column( timeCount );

    // looping over stimuli
    int32_t c = 0;
    std::list< gkg::Stimulus* >::const_iterator s = _stimuli.begin(),
                                                se = _stimuli.end();
    while ( s != se )
    {

      gkg::Stimulus* stimulus = *s;
      stimulus->convolve( hrf, times, column );
      designMatrix.setColumn( c, column );
      regressors[ c ] = stimulus->getName();
      ++ s;
      ++ c;

    }

    // set labels
    int32_t s1, s2;

    for ( s1 = 0; s1 < timeCount; s1++ )
    {

      int32_t sMax = 0;
      float max = (float)designMatrix( s1, 0 );

      for ( s2 = 1; s2 < _regressorCount; s2++ )
      {

        float value = (float)designMatrix( s1, s2 );

        if ( value > max )
        {

          max = value;
          sMax = s2;

        }

      }

      if ( max > GKG_HRF_THRESHOLD )
      {

        labels[ s1 ] = regressors[ sMax ];

      }

    }

  }
  GKG_CATCH( "void gkg::Paradigm::getDesignLabels( const gkg::Vector& times, "
             "std::vector< std::string >& labels )" );

}


#undef GKG_HRF_THRESHOLD
#undef GKG_PARADIGM_TINY
