#include <gkg-simulation-mri-sequence/MRISequence.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


#define GYROMAGNETIC_RATIO   42.576 * 1e6 /* Hz/T */


////////////////////////////////////////////////////////////////////////////////
// class MRISequenceIntegrableFunction
////////////////////////////////////////////////////////////////////////////////


gkg::MRISequenceIntegrableFunction::MRISequenceIntegrableFunction(
                                           const gkg::MRISequence* mriSequence )
                                   : _mriSequence( mriSequence )
{
}


gkg::MRISequenceIntegrableFunction::~MRISequenceIntegrableFunction()
{
}


double gkg::MRISequenceIntegrableFunction::getValue( double timeInUs ) const
{

  try
  {

    float normalizedIntegralGradientValue = 0.0f;
    _mriSequence->getNormalizedValueAt( ( float )timeInUs,
                                        normalizedIntegralGradientValue );
    
    return ( double )normalizedIntegralGradientValue;

  }
  GKG_CATCH( "double gkg::MRISequenceIntegrableFunction::getValue( "
             "double timeInUs ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// class MRISequence
////////////////////////////////////////////////////////////////////////////////

gkg::MRISequence::MRISequence( const gkg::Dictionary& parameters,
                               float timeStepInUs,
                               bool verbose,
                               std::ofstream* osLog )
                 : _integrableFunction( this ),
                   _factory(gkg::NumericalAnalysisSelector::getInstance().
                                                    getImplementationFactory() )

{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting gradient magnitude set(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary::const_iterator 
      i = parameters.find( "gradient_magnitudes" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'gradient_magnitudes' item is required" );

    }
    gkg::GenericObjectList golGradientMagnitudeSets =
                                i->second->getValue< gkg::GenericObjectList >();

    _setCount = ( int32_t )golGradientMagnitudeSets.size();

    _gradientMagnitudeCounts.resize( _setCount );
    _gradientMagnitudeSets.resize( _setCount );


    _maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter = 0.0f;
    float gradientMagnitude = 0.0f;
    float absoluteGradientMagnitude = 0.0f;
    int32_t s = 0;
    for ( s = 0; s < _setCount; s++ )
    {

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "gradient magnitude set " << s + 1 << " : "
                 << std::flush;

        }
        else
        {

          std::cout << "gradient magnitude set " << s + 1 << " : "
                    << std::flush;

        }

      }

      gkg::GenericObjectList 
        golGradientMagnitudes = golGradientMagnitudeSets[ s ]->getValue<
                                                     gkg::GenericObjectList >();


      int32_t gradientMagnitudeCount = ( int32_t )golGradientMagnitudes.size();

      _gradientMagnitudeCounts[ s ] = gradientMagnitudeCount;
      _gradientMagnitudeSets[ s ].resize( gradientMagnitudeCount );

      int32_t g = 0;
      for ( g = 0; g < gradientMagnitudeCount; g++ )
      {

        gradientMagnitude = ( float )golGradientMagnitudes[ g ]->getScalar();
        absoluteGradientMagnitude = std::abs( gradientMagnitude );
        _gradientMagnitudeSets[ s ][ g ] = gradientMagnitude;
        
        if ( absoluteGradientMagnitude >
             _maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter )
        {
        
          _maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter =
            absoluteGradientMagnitude;
        
        } 
        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << _gradientMagnitudeSets[ s ][ g ] << " "
                   << std::flush;

          }
          else
          {

            std::cout << _gradientMagnitudeSets[ s ][ g ] << " "
                      << std::flush;

          }

        }

      }
      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << std::endl;

        }
        else
        {

          std::cout << std::endl;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting gradient orientation set(s)
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "gradient_orientations" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'gradient_orientations' item is required" );

    }
    gkg::GenericObjectList golGradientOrientationSets =
                                i->second->getValue< gkg::GenericObjectList >();

    int32_t setCount = ( int32_t )golGradientOrientationSets.size();

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << setCount << " orientation set(s) detected" << std::endl;

      }
      else
      {

        std::cout << setCount << " orientation set(s) detected" << std::endl;

      }

    }
    if ( setCount != _setCount )
    {

      throw std::runtime_error( "gradient orientation set count must be equal "
                                "to gradient magnitude set count" );

    }

    _gradientOrientationCounts.resize( _setCount );
    _gradientOrientationSets.resize( _setCount );

    for ( s = 0; s < _setCount; s++ )
    {

      try
      {

        int32_t gradientOrientationCount = ( int32_t )( 
                           golGradientOrientationSets[ s ]->getScalar() + 0.5 );

        _gradientOrientationCounts[ s ] = gradientOrientationCount;
        _gradientOrientationSets[ s ] = gkg::ElectrostaticOrientationSet(
                                                     gradientOrientationCount );
        if ( verbose )
        {
 
          if ( osLog )
          {
 
            *osLog << "gradient orientation set " << s + 1 << " : "
                   << std::endl;
 
          }
          else
          {
 
            std::cout << "gradient orientation set " << s + 1 << " : "
                      << std::endl;
 
          }
          int32_t o = 0;
          for ( o = 0; o < gradientOrientationCount; o++ )
          {

            if ( osLog )
            {
 
              *osLog << _gradientOrientationSets[ s ].getOrientation( o )
                     << " "
                     << std::endl;
 
            }
            else
            {
 
              std::cout << _gradientOrientationSets[ s ].getOrientation( o )
                        << " "
                        << std::endl;
 
            }

          }

        }

      }
      catch( std::exception& )
      {

        gkg::GenericObjectList 
          golGradientOrientations = golGradientOrientationSets[ s ]->getValue<
                                                     gkg::GenericObjectList >();
        int32_t gradientOrientationCount =
                                      ( int32_t )golGradientOrientations.size();
        _gradientOrientationCounts[ s ] = gradientOrientationCount;

        std::vector< gkg::Vector3d< float > > 
          gradientOrientations( gradientOrientationCount );

        int32_t o = 0;
        for ( o = 0; o < gradientOrientationCount; o++ )
        {

          gkg::GenericObjectList 
          golGradientOrientation = golGradientOrientations[ o ]->getValue<
                                                     gkg::GenericObjectList >();

          if ( golGradientOrientation.size() != 3U )
          {

            throw std::runtime_error(
                              "bad gradient orientation: missing coordinates" );

          }
          gradientOrientations[ o ].x = ( float )
                                       golGradientOrientation[ 0 ]->getScalar();
          gradientOrientations[ o ].y = ( float )
                                       golGradientOrientation[ 1 ]->getScalar();
          gradientOrientations[ o ].z = ( float )
                                       golGradientOrientation[ 2 ]->getScalar();

        }

        _gradientOrientationSets[ s ] = gkg::OrientationSet(
                                                         gradientOrientations );

        if ( verbose )
        {

          if ( osLog )
          {
 
            *osLog << "gradient orientation set " << s + 1 << " : "
                   << std::endl;
 
          }
          else
          {
 
            std::cout << "gradient orientation set " << s + 1 << " : "
                      << std::endl;
 
          }
          int32_t o = 0;
          for ( o = 0; o < gradientOrientationCount; o++ )
          {

            if ( osLog )
            {
 
              *osLog << _gradientOrientationSets[ s ].getOrientation( o )
                     << " "
                     << std::endl;
 
            }
            else
            {
 
              std::cout << _gradientOrientationSets[ s ].getOrientation( o )
                        << " "
                        << std::endl;
 
            }

          }

        }

      }

    }

    _totalGradientMagnitudeCount = 0;
    for ( s = 0; s < _setCount; s++ )
    {

      _totalGradientMagnitudeCount += _gradientMagnitudeCounts[ s ];

    }

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "total gradient magnitude count : "
               << _totalGradientMagnitudeCount
               << std::endl;
 
      }
      else
      {
 
        std::cout << "total gradient magnitude count : "
                  << _totalGradientMagnitudeCount
                  << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting echo time in ms and converting it to us
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "echo_time_in_ms" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'echo_time_in_ms' item is required" );

    }
    _echoTimeInUs = ( float )i->second->getScalar() * 1000.0f;

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "echo time in us : " << _echoTimeInUs << std::endl;
 
      }
      else
      {
 
        std::cout << "echo time in us : " << _echoTimeInUs << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting time offset to diffusion module in ms and converting it to us
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "time_offset_to_diffusion_module_in_ms" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error(
                 "a 'time_offset_to_diffusion_module_in_ms' item is required" );

    }
    _timeOffsetToDiffusionModuleInUs = ( float )i->second->getScalar() *
                                       1000.0f;

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "time offset to diffusion module in us : "
               << _timeOffsetToDiffusionModuleInUs << std::endl;
 
      }
      else
      {
 
        std::cout << "time offset to diffusion module in us : "
                  << _timeOffsetToDiffusionModuleInUs << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting maximum gradient slew rate in T/m/s
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "maximum_gradient_slew_rate_in_tesla_per_m_per_s" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error(
       "a 'maximum_gradient_slew_rate_in_tesla_per_m_per_s' item is required" );

    }
    _maximumGradientSlewRateInTeslaPerMeterPerSecond =
                                               ( float )i->second->getScalar();
    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "max. slewrate in T/m/s : "
               << _maximumGradientSlewRateInTeslaPerMeterPerSecond
               << std::endl;
 
      }
      else
      {
 
        std::cout << "max. slewrate in T/m/s : "
                  << _maximumGradientSlewRateInTeslaPerMeterPerSecond
                  << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting the noise standard deviation
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "noise_standard_deviation" );
    if ( i == parameters.end() )
    {

      _noiseStandardDeviation = 0.0f;

    }
    else
    {

      _noiseStandardDeviation = ( float )i->second->getScalar();
      if ( _noiseStandardDeviation < 0.0f )
      {

        throw std::runtime_error(
                         "noise standard deviation must be strictly positive" );

      }

    }
    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "noise standard deviation : "
               << _noiseStandardDeviation
               << std::endl;
 
      }
      else
      {
 
        std::cout << "noise standard deviation : "
                  << _noiseStandardDeviation
                  << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the ramp time; it corresponds to the minimum rise time for
    // the maximum gradient magnitude under the constraint of the maximum
    // slew-rate
    ////////////////////////////////////////////////////////////////////////////

    _rampTimeInUs = _maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter /
                    _maximumGradientSlewRateInTeslaPerMeterPerSecond * 1e3f;

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "ramp time in us : "
               << _rampTimeInUs
               << std::endl;
 
      }
      else
      {
 
        std::cout << "ramp time in us : "
                  << _rampTimeInUs
                  << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing time step count
    ////////////////////////////////////////////////////////////////////////////

    _timeStepInUs = timeStepInUs;
    _timeStepCount = ( int32_t )( _echoTimeInUs / timeStepInUs );
    if ( _echoTimeInUs / timeStepInUs - ( float )_timeStepCount > 0.0f )
    {

      ++ _timeStepCount;

    }

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "time step count required : "
               << _timeStepCount
               << std::endl;
 
      }
      else
      {
 
        std::cout << "time step count required : "
                  << _timeStepCount
                  << std::endl;
 
      }

    }

  }
  GKG_CATCH( "gkg::MRISequence::MRISequence( "
             "const gkg::Dictionary& parameters, "
             "float timeStep, "
             "bool verbose, "
             "std::ofstream* osLog )" )

}


gkg::MRISequence::~MRISequence()
{
}


float gkg::MRISequence::getEchoTimeInUs() const
{

  try
  {

    return _echoTimeInUs;

  }
  GKG_CATCH( "float gkg::MRISequence::getEchoTimeInUs() const" );

}



float
gkg::MRISequence::getMaximumGradientSlewRateInTeslaPerMeterPerSecond() const
{

  try
  {

    return _maximumGradientSlewRateInTeslaPerMeterPerSecond;

  }
  GKG_CATCH( "float gkg::MRISequence::"
             "getMaximumGradientSlewRateInTeslaPerMeterPerSecond() const" );

}


int32_t gkg::MRISequence::getTimeStepCount() const
{

  try
  {

    return _timeStepCount;

  }
  GKG_CATCH( "int32_t gkg::MRISequence::getTimeStepCount() const" );

}


int32_t gkg::MRISequence::getGradientMagnitudeSetCount() const
{

  try
  {

    return _setCount;

  }
  GKG_CATCH( "int32_t gkg::MRISequence::getGradientMagnitudeSetCount() const" );

}


int32_t gkg::MRISequence::getGradientMagnitudeCount( int32_t setIndex ) const
{

  try
  {

    return _gradientMagnitudeCounts[ setIndex ];

  }
  GKG_CATCH( "int32_t gkg::MRISequence::getGradientMagnitudeCount( "
             "int32_t setIndex ) const" );

}


float gkg::MRISequence::getGradientMagnitude(
                                          int32_t setIndex,
                                          int32_t gradientMagnitudeIndex ) const
{

  try
  {

    return _gradientMagnitudeSets[ setIndex ][ gradientMagnitudeIndex ];

  }
  GKG_CATCH( "float gkg::MRISequence::getGradientMagnitude( "
             "int32_t setIndex, "
             "int32_t gradientMagnitudeIndex ) const" );

}


int32_t gkg::MRISequence::getGradientOrientationCount( int32_t setIndex ) const
{

  try
  {

    return _gradientOrientationCounts[ setIndex ];

  }
  GKG_CATCH( "int32_t gkg::MRISequence::getGradientOrientationCount( "
             "int32_t setIndex ) const" );

}


const gkg::OrientationSet& 
gkg::MRISequence::getGradientOrientationSet( int32_t setIndex ) const
{

  try
  {

    return _gradientOrientationSets[ setIndex ];

  }
  GKG_CATCH( "const gkg::OrientationSet& "
             "gkg::MRISequence::getGradientOrientationSet( "
             "int32_t setIndex ) const" );

}


int32_t gkg::MRISequence::getTotalGradientMagnitudeCount() const
{

  try
  {

    return _totalGradientMagnitudeCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::MRISequence::getTotalGradientMagnitudeCount() const" );

}


int32_t gkg::MRISequence::getTotalGradientOrientationCount(
                                   int32_t totalGradientOrientationIndex ) const
{

  try
  {

    return _totalGradientOrientationCounts[ totalGradientOrientationIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::MRISequence::getTotalGradientOrientationCount( "
             "int32_t totalGradientOrientationIndex ) const" );

}


float gkg::MRISequence::getNoiseStandardDeviation() const
{

  try
  {

    return _noiseStandardDeviation;

  }
  GKG_CATCH( "float "
             "gkg::MRISequence::getNoiseStandardDeviation() const" );

}


const gkg::Vector3d< float >& 
gkg::MRISequence::getPhaseShift( int32_t timeStepIndex,
                                 int32_t gradientMagnitudeIndex,
                                 int32_t orientationIndex ) const
{

  try
  {

    return _phaseShifts[ timeStepIndex ]
                         [ gradientMagnitudeIndex ]
                         [ orientationIndex ];

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::MRISequence::getPhaseShift( "
             "int32_t timeStepIndex, "
             "int32_t gradientMagnitudeIndex, "
             "int32_t orientationIndex ) const" );

}


void gkg::MRISequence::getNormalizedIntegralValueAt(
                                  float timeInUs,
                                  float& normalizedIntegralGradientValue ) const
{

  try
  {

    normalizedIntegralGradientValue = ( float )_factory->getQAGSIntegral(
                                                    _integrableFunction,
                                                    0.0,
                                                    ( double )timeInUs,
                                                    1e-6,
                                                    1e-6,
                                                    0,
                                                    1000 );

  }
  GKG_CATCH( "void gkg::MRISequence::getNormalizedIntegralValueAt( "
             "float tmeInUs, "
             "float& normalizedIntegralGradientValue ) const" );

}



void gkg::MRISequence::computePhaseShifts()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing phase shifts
    ////////////////////////////////////////////////////////////////////////////

    int32_t timeStepIndex  = 0;
    int32_t setIndex = 0;
    int32_t gradientMagnitudeIndex  = 0;
    int32_t totalGradientMagnitudeIndex = 0;
    int32_t gradientOrientationIndex  = 0;
    float timeInUs = 0.0f;

    float normalizedGradientValue = 0.0f;

    // timeStep -> us ( 1 us = 1e-6 s )
    // gradient -> mT / m ( 1 mT/m = 1e-3 T/m )
    // position -> um ( 1 um = 1e-6 m )
    float timeStepInUsTimesFactor = 1e-15 * 2 * M_PI * GYROMAGNETIC_RATIO *
                                    _timeStepInUs;

    _phaseShifts.resize( _timeStepCount );
    gkg::Vector3d< float > zeroShift( 0.0f, 0.0f, 0.0f );
    std::list< int32_t > totalGradientOrientationCounts;

    for ( timeStepIndex = 0;
          timeStepIndex < _timeStepCount;
          timeStepIndex++ )
    {

      _phaseShifts[ timeStepIndex ].resize( _totalGradientMagnitudeCount );
      timeInUs = _timeStepInUs * ( float )timeStepIndex;

      totalGradientMagnitudeIndex = 0;
      for ( setIndex = 0;
            setIndex < _setCount;
            setIndex++ )
      {

        const int32_t& 
          gradientMagnitudeCount = _gradientMagnitudeCounts[ setIndex ];

        for ( gradientMagnitudeIndex = 0;
              gradientMagnitudeIndex < gradientMagnitudeCount;
              gradientMagnitudeIndex++ )
        {

          const float& gradientMagnitude =
                               _gradientMagnitudeSets[ setIndex ]
                                                     [ gradientMagnitudeIndex ];
          const int32_t& gradientOrientationCount = 
                                        _gradientOrientationCounts[ setIndex ];

          _phaseShifts[ timeStepIndex ][ totalGradientMagnitudeIndex ].resize(
                                          gradientOrientationCount, zeroShift );

          for ( gradientOrientationIndex = 0;
                 gradientOrientationIndex < gradientOrientationCount;
                 gradientOrientationIndex++ )
          {


            gkg::Vector3d< float >&
              phaseShift = _phaseShifts[ timeStepIndex ]
                                       [ totalGradientMagnitudeIndex ]
                                       [ gradientOrientationIndex ];

            this->getNormalizedValueAt( timeInUs, normalizedGradientValue );

            const gkg::Vector3d< float >&
              orientation = _gradientOrientationSets[ setIndex ].
                                     getOrientation( gradientOrientationIndex );


            phaseShift.x = normalizedGradientValue *
                           gradientMagnitude *
                           orientation.x *
                           timeStepInUsTimesFactor;
            phaseShift.y = normalizedGradientValue *
                           gradientMagnitude *
                           orientation.y *
                           timeStepInUsTimesFactor;
            phaseShift.z = normalizedGradientValue *
                           gradientMagnitude *
                           orientation.z *
                           timeStepInUsTimesFactor;

          }

          totalGradientOrientationCounts.push_back( gradientOrientationCount );

          ++ totalGradientMagnitudeIndex;

        }

      }

    }

    _totalGradientOrientationCounts =
                 std::vector< int32_t >( totalGradientOrientationCounts.begin(),
                                         totalGradientOrientationCounts.end() );

  }
  GKG_CATCH( "void gkg::MRISequence::computePhaseShifts()" )

}


//
// the b-value follows the equation:
//
//           /TE  ->       ->
//      b = |    k(t)^T x k(t) dt
//         /0
//
// with
//
//       ->             /t   ->
//      k(t) = GAMMA x |    G(u) du
//                    /0
//
// GAMMA is given in radian/Tesla, and can be replaced by 2 x Pi x ~GAMMA~
// where  ~GAMMA~ is provided in Hz/Tesla (typically 42.57 MHz/T for the water
// proton)
//

float gkg::MRISequence::getBValueInSecondPerMm2(
                                                int32_t setIndex,
                                                int32_t gradientMagnitudeIndex,
                                                int32_t orientationIndex ) const
{

  try
  {


    // collecting effective gradient magnitude in mT/m
    float gradientMagnitudeInMtPerMeter = getGradientMagnitude(
                                                       setIndex,
                                                       gradientMagnitudeIndex );

    // collecting the gradient orientation
    const gkg::Vector3d< float > &
      gradientOrientation = getGradientOrientationSet( setIndex ).
                                             getOrientation( orientationIndex );

    // computing the b-value
    gkg::Vector3d< float > Kt;
    float normalizedGradientIntegralValue = 0.0f;
    float bValue = 0.0f;
    float timeInUs = 0.0f;
    int32_t t = 0;
    for ( t = 0; t < _timeStepCount; t++ )
    {

      // collecting the normalized gradient integral value in us
      getNormalizedIntegralValueAt( timeInUs, normalizedGradientIntegralValue );

      // computing the gradient integral value
      Kt.x = 2.0 * M_PI * GYROMAGNETIC_RATIO *
             gradientMagnitudeInMtPerMeter *
             normalizedGradientIntegralValue *
             gradientOrientation.x;
      Kt.y = 2.0 * M_PI * GYROMAGNETIC_RATIO *
             gradientMagnitudeInMtPerMeter *
             normalizedGradientIntegralValue *
             gradientOrientation.y;
      Kt.z = 2.0 * M_PI * GYROMAGNETIC_RATIO *
             gradientMagnitudeInMtPerMeter *
             normalizedGradientIntegralValue *
             gradientOrientation.z;

      bValue += Kt.getNorm2() * _timeStepInUs;
      timeInUs += _timeStepInUs;

    }

    // convertion to s/mm^2 knowing that:
    //     - gradient -> mT / m ( 1 mT/m = 1e-6 T/m )
    //     -  time     -> us     ( 1 us = 1e-6 s )
    bValue *= 1e-30;

    return bValue;


  }
  GKG_CATCH( "float gkg::MRISequence::getBValueInSecondPerMm2( "
             "int32_t setIndex, "
             "int32_t gradientMagnitudeIndex, "
             "int32_t orientationIndex ) const" );

}


void gkg::MRISequence::displayInformation( std::ofstream* /*osLog*/ ) const
{

  try
  {

    throw std::runtime_error( "should not enter this method" );

  }
  GKG_CATCH( "void gkg::MRISequence::displayInformation( "
             "std::ofstream* /*osLog*/ ) const" );

}



void gkg::MRISequence::saveNormalizedProfile( const std::string& fileName ) const
{

  try
  {
  
    std::ofstream os( fileName.c_str() );


    std::vector< float > times( _timeStepCount );
    std::vector< float > normalizedValues( _timeStepCount );
    
    int32_t t = 0;
    for ( t = 0; t < _timeStepCount; t++ )
    {

      times[ t ] = ( float )t * _timeStepInUs;
      this->getNormalizedValueAt( times[ t ],
                                  normalizedValues[ t ] );

    }
    
    if ( !os )
    {

      throw std::runtime_error( std::string( "unable to open file '" ) +
                                fileName + "'" );

    }
    
    os << "import matplotlib.pyplot as plt"
       << std::endl
       << std::endl;
    
    os << "times = [ " << std::flush;
    for ( t = 0; t < _timeStepCount - 1; t++ )
    {
    
      os << times[ t ] << ", ";
    
    }
    os << times[ t ] << " ]"
       << std::endl
       << std::endl;
    
    os << "normalizedValues = [ " << std::flush;
    for ( t = 0; t < _timeStepCount - 1; t++ )
    {
    
      os << normalizedValues[ t ] << ", ";
    
    }
    os << normalizedValues[ t ] << " ]"
       << std::endl
       << std::endl;

    os << "plt.plot( times, normalizedValues, linestyle='-' )"
       << std::endl;
    os << "plt.grid( True )" << std::endl;
    os << "plt.xlabel( 'time (us)' )" << std::endl;
    os << "plt.ylabel( 'normalized gradient magnitude' )" << std::endl;

    os << "plt.show()" << std::endl;    
    
    os.close();
  
  }
  GKG_CATCH( "void gkg::MRISequence::saveNormalizedProfile( "
             "const std::string& fileName ) const" );

}


void gkg::MRISequence::saveNormalizedIntegralProfile(
                                             const std::string& fileName ) const
{

  try
  {
  
    std::ofstream os( fileName.c_str() );


    std::vector< float > times( _timeStepCount );
    std::vector< float > normalizedIntegralValues( _timeStepCount );
    
    int32_t t = 0;
    for ( t = 0; t < _timeStepCount; t++ )
    {

      times[ t ] = ( float )t * _timeStepInUs;
      this->getNormalizedIntegralValueAt( times[ t ],
                                          normalizedIntegralValues[ t ] );

    }
    
    if ( !os )
    {
    
      throw std::runtime_error( std::string( "unable to open file '" ) +
                                fileName + "'" );
    
    }
    
    os << "import matplotlib.pyplot as plt"
       << std::endl
       << std::endl;
    
    os << "times = [ " << std::flush;
    for ( t = 0; t < _timeStepCount - 1; t++ )
    {
    
      os << times[ t ] << ", ";
    
    }
    os << times[ t ] << " ]"
       << std::endl
       << std::endl;
    
    os << "normalizedIntegralValues = [ " << std::flush;
    for ( t = 0; t < _timeStepCount - 1; t++ )
    {
    
      os << normalizedIntegralValues[ t ] << ", ";
    
    }
    os << normalizedIntegralValues[ t ] << " ]"
       << std::endl
       << std::endl;

    os << "plt.plot( times, normalizedIntegralValues, linestyle='-' )"
       << std::endl;
    os << "plt.grid( True )" << std::endl;
    os << "plt.xlabel( 'time (us)' )" << std::endl;
    os << "plt.ylabel( 'normalized integral gradient magnitude' )" << std::endl;

    os << "plt.show()" << std::endl;    
    
    os.close();
  
  }
  GKG_CATCH( "void gkg::MRISequence::saveNormalizedIntegralProfile( "
             "const std::string& fileName ) const" );

}


void gkg::MRISequence::saveNormalizedDiffusionGradientProfile(
                                             const std::string& fileName,
                                             bool withBrukerHeader ) const
{

  try
  {
  
    std::ofstream os( fileName.c_str() );

    if ( !os )
    {
    
      throw std::runtime_error( std::string( "unable to open file '" ) +
                                fileName + "'" );
    
    }


    if ( withBrukerHeader )
    {

      os << "##TITLE= Waveform" << std::endl;
      os << "##JCAMP-DX= 5.00 Bruker JCAMP library" << std::endl;
      os << "##DATA TYPE= Shape Data" << std::endl;
      os << "##ORIGIN= Bruker Analytik GmbH" << std::endl;
      os << "##OWNER= <nmrsu>" << std::endl;
      os << "##DATE= xx" << std::endl;
      os << "##TIME= xx" << std::endl;
      os << "##MINX= 0" << std::endl;
      os << "##MAXX= 1" << std::endl;
      os << "##MINY= 0" << std::endl;
      os << "##MAXY= 1" << std::endl;
      os << "##$SHAPE_EXMODE= Gradient" << std::endl;
      os << "##$SHAPE_TOTROT= 0" << std::endl;
      os << "##$SHAPE_BWFAC= 0" << std::endl;
      os << "##$SHAPE_INTEGFAC= 0" << std::endl;
      os << "##$SHAPE_MODE= 0" << std::endl;
      os << "##BFACTOR= 6344.945991" << std::endl;
      os << "##DURATION= "
         << ( float )( _diffusionGradientWaveformEndingIndex -
                       _diffusionGradientWaveformStartingIndex )
                            * _timeStepInUs / 1000.0f
         << std::endl;
      os << "##DIRECTIONVEC= 1 0 0" << std::endl;
      os << "##NPOINTS= " << _diffusionGradientWaveformEndingIndex - 
                             _diffusionGradientWaveformStartingIndex + 1
                          << std::endl;
      os << "##XYDATA= (T X Y Z)" << std::endl;

    }

    float normalizedValue = 0.0f;
    
    int32_t t = 0;
    for ( t = _diffusionGradientWaveformStartingIndex;
          t <= _diffusionGradientWaveformEndingIndex; t++ )
    {
    
      this->getNormalizedValueAt( ( float )t * _timeStepInUs, normalizedValue );
      if ( withBrukerHeader )
      {

        os << normalizedValue << " 0.0 0.0" << std::endl;

      }
      else
      {

        os << normalizedValue << std::endl;

      }

    }
    
    os.close();
  
  }
  GKG_CATCH( "void gkg::MRISequence::saveNormalizedProfile( "
             "const std::string& fileName ) const" );

}


void gkg::MRISequence::saveDiffusionInformation(
                                             const std::string& fileName ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // saving *.bval file and collecting b-vector coordinates
    ////////////////////////////////////////////////////////////////////////////

    std::ofstream os( ( fileName + ".bval" ).c_str() );

    int32_t setIndex = 0;
    int32_t gradientMagnitudeIndex = 0;
    int32_t gradientMagnitudeCount = 0;
    int32_t orientationIndex = 0;
    int32_t orientationCount = 0;
    float bValueInSecondPerMm2 = 0.0;

    std::list< float > bVectorCoordinateX;
    std::list< float > bVectorCoordinateY;
    std::list< float > bVectorCoordinateZ;

    for ( setIndex = 0; setIndex < _setCount; setIndex++ )
    {

      gradientMagnitudeCount = _gradientMagnitudeCounts[ setIndex ];

      for ( gradientMagnitudeIndex = 0;
            gradientMagnitudeIndex < gradientMagnitudeCount;
            gradientMagnitudeIndex++ )
      {

        // all orientations share the same b-value, so compute
        // only the first and replicate it
        bValueInSecondPerMm2 = this->getBValueInSecondPerMm2(
                                                         setIndex,
                                                         gradientMagnitudeIndex,
                                                         0 );

        orientationCount = _gradientOrientationCounts[ setIndex ];

        const gkg::OrientationSet& 
          orientationSet = _gradientOrientationSets[ setIndex ];

        for ( orientationIndex = 0;
              orientationIndex < orientationCount;
              orientationIndex++ )
        {

          const gkg::Vector3d< float >& 
            orientation = orientationSet.getOrientation( orientationIndex );

          bVectorCoordinateX.push_back( orientation.x );
          bVectorCoordinateY.push_back( orientation.y );
          bVectorCoordinateZ.push_back( orientation.z );

          os << bValueInSecondPerMm2 << " ";

        }

      }

    }
    os << std::endl;
    os.close();


    ////////////////////////////////////////////////////////////////////////////
    // saving *.bvec file
    ////////////////////////////////////////////////////////////////////////////

    os.open( ( fileName + ".bvec" ).c_str() );

    std::list< float >::const_iterator
      ox = bVectorCoordinateX.begin(),
      oxe = bVectorCoordinateX.end();
    while ( ox != oxe )
    {

      os << *ox << " ";
      ++ ox;

    }
    os << std::endl;

    std::list< float >::const_iterator
      oy = bVectorCoordinateY.begin(),
      oye = bVectorCoordinateY.end();
    while ( oy != oye )
    {

      os << *oy << " ";
      ++ oy;

    }
    os << std::endl;

    std::list< float >::const_iterator
      oz = bVectorCoordinateZ.begin(),
      oze = bVectorCoordinateZ.end();
    while ( oz != oze )
    {

      os << *oz << " ";
      ++ oz;

    }
    os << std::endl;

    os.close();

  }
  GKG_CATCH( "void gkg::MRISequence::saveNormalizedProfile( "
             "const std::string& fileName ) const" );

}


#undef GYROMAGNETIC_RATIO
