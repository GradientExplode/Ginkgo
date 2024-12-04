#include <gkg-simulation-mri-sequence/OGSESequence.h>
#include <gkg-simulation-mri-sequence/MRISequenceFactory.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>


#define SQ( x ) ( x * x )


gkg::OGSESequence::OGSESequence( const gkg::Dictionary& parameters,
                                 float timeStepInUs,
                                 bool verbose,
                                 std::ofstream* osLog )
                  : gkg::MRISequence( parameters, timeStepInUs, verbose,
                                      osLog ),
                    _t4( 0.0f ),
                    _t5( 0.0f ),
                    _t6( 0.0f ),
                    _t7( 0.0f )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting waveform type
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary::const_iterator
      i = parameters.find( "waveform_type" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'waveform_type' item is required" );

    }
    _waveformType = i->second->getString();

    if ( !( ( _waveformType == "trapezoid-sine" ) ||
            ( _waveformType == "trapezoid-cosine" ) ||
            ( _waveformType == "sine" ) ||
            ( _waveformType == "cosine" ) ) )
    {

      throw std::runtime_error( "not a valid waveform type" );

    }

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "sequence type : OGSE / " << _waveformType << std::endl;
 
      }
      else
      {
 
        std::cout << "sequence type : OGSE / " << _waveformType << std::endl;
 
      }

    }
            

    ////////////////////////////////////////////////////////////////////////////
    // collecting big delta in ms and converting it into us
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "big_delta_in_ms" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'big_delta_in_ms' item is required" );

    }
    _bigDeltaInUs = ( float )i->second->getScalar() * 1000.0f;


    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "big delta in us : " << _bigDeltaInUs << std::endl;
 
      }
      else
      {
 
        std::cout << "big delta in us : " << _bigDeltaInUs << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting period in ms and converting it into us
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "period_in_ms" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'period_in_ms' item is required" );

    }
    _periodInUs = ( float )i->second->getScalar() * 1000.0f;

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "period in us : " << _periodInUs << std::endl;
 
      }
      else
      {
 
        std::cout << "period in us : " << _periodInUs << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting lobe count
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "period_count" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'period_count' item is required" );

    }
    _periodCount = ( int32_t )( i->second->getScalar() + 0.5 );

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "period count : " << _periodCount << std::endl;
 
      }
      else
      {
 
        std::cout << "period count : " << _periodCount << std::endl;
 
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////
    
    if ( _periodInUs / 2.0 <= 2 * _rampTimeInUs )
    {
    
      throw std::runtime_error(
        "period is chosen too short with respect to the "
        "maximum gradient magnitude and prescribed slewrate" );
    
    }

    if ( _waveformType == "sine" )
    {
    
      // g(t) = sin( w * t )
      // w = 2 * M_PI / _periodInUs
      // g'(t) = w * cos( w * t )
      // g'(0) = w = 2 * M_PI / _periodInUs

      _angularFrequency = 2.0f * M_PI / _periodInUs;      
      if ( ( _angularFrequency *
             this->_maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter ) >= 
           1000 * this->_maximumGradientSlewRateInTeslaPerMeterPerSecond )
      {
    
        throw std::runtime_error(
          "period is chosen too low with respect to the "
          "maximum gradient magnitude and prescribed slewrate" );
    
      }

      _t0 = _timeOffsetToDiffusionModuleInUs;
      _t1 = _t0 + _periodInUs * ( float )_periodCount;
      _t2 = _t0 + _bigDeltaInUs;
      _t3 = _t2 + _periodInUs * ( float )_periodCount;

      if ( _t3 > this->_echoTimeInUs )
      {
    
        throw std::runtime_error(
                       "the end of the Sine-OGSE diffusion module goes "
                       "beyond the echo time" );

      }

      _diffusionGradientWaveformStartingIndex = ( int32_t )( _t0 /
                                                             _timeStepInUs );
      _diffusionGradientWaveformEndingIndex = ( int32_t )( _t1 /
                                                             _timeStepInUs );
      if ( ( float )_diffusionGradientWaveformEndingIndex !=
           ( _t1 / _timeStepInUs ) )
      {

        ++ _diffusionGradientWaveformEndingIndex;

      }
      
    }
    else if ( _waveformType == "cosine" )
    {
    
      // g(t) = cos( w * t )
      // w = 2 * M_PI / _periodInUs
      // g'(t) = -w * sin( w * t )
      // g'(_periodInUs / 4) = -w = -2 * M_PI / _periodInUs
  
      _angularFrequency = 2.0f * M_PI / _periodInUs;
      if ( ( _angularFrequency *
             this->_maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter ) >= 
           1000 * this->_maximumGradientSlewRateInTeslaPerMeterPerSecond )
      {
    
        throw std::runtime_error(
          "period is chosen too low with respect to the "
          "maximum gradient magnitude and prescribed slewrate" );
    
      }
      _t0 = _timeOffsetToDiffusionModuleInUs;
      _t1 = _t0 + _rampTimeInUs;
      _t2 = _t1 + _periodInUs * ( float )_periodCount;
      _t3 = _t2 + _rampTimeInUs;
      _t4 = _t0 + _bigDeltaInUs;
      _t5 = _t4 + _rampTimeInUs;
      _t6 = _t5 + _periodInUs * ( float )_periodCount;
      _t7 = _t6 + _rampTimeInUs;

      if ( _t7 > this->_echoTimeInUs )
      {
    
        throw std::runtime_error(
                       "the end of the Cosine-OGSE diffusion module goes "
                       "beyond the echo time" );

      }

      _diffusionGradientWaveformStartingIndex = ( int32_t )( _t0 /
                                                             _timeStepInUs );
      _diffusionGradientWaveformEndingIndex = ( int32_t )( _t3 /
                                                             _timeStepInUs );
      if ( ( float )_diffusionGradientWaveformEndingIndex !=
           ( _t3 / _timeStepInUs ) )
      {

        ++ _diffusionGradientWaveformEndingIndex;

      }

    }
    else if ( _waveformType == "trapezoid-sine" )
    {

      if ( _periodInUs - 4 * _rampTimeInUs < 0 ) 
      {

        throw std::runtime_error(
          "period is chosen too low with respect to the "
          "maximum gradient magnitude and prescribed slewrate" );

      }   

      _t0 = _timeOffsetToDiffusionModuleInUs;
      _t1 = _t0 + _periodInUs * ( float )_periodCount;
      _t2 = _t0 + _bigDeltaInUs;
      _t3 = _t2 + _periodInUs * ( float )_periodCount;

      if ( _t3 > this->_echoTimeInUs )
      {
    
        throw std::runtime_error(
                "the end of the Trapezoid-Sine-OGSE diffusion module goes "
                "beyond the echo time" );

      }

      _trapezoidSineFunction.reset( new gkg::TrapezoidSineFunction( 
                                             _rampTimeInUs,
                                             _periodInUs ) );

      _diffusionGradientWaveformStartingIndex = ( int32_t )( _t0 /
                                                             _timeStepInUs );
      _diffusionGradientWaveformEndingIndex = ( int32_t )( _t1 /
                                                             _timeStepInUs );
      if ( ( float )_diffusionGradientWaveformEndingIndex !=
           ( _t1 / _timeStepInUs ) )
      {

        ++ _diffusionGradientWaveformEndingIndex;

      }


    }

    else if ( _waveformType == "trapezoid-cosine" )
    {

      if ( _periodInUs - 4 * _rampTimeInUs < 0 ) 
      {

        throw std::runtime_error(
          "period is chosen too low with respect to the "
          "maximum gradient magnitude and prescribed slewrate" );

      }   
      _t0 = _timeOffsetToDiffusionModuleInUs;
      _t1 = _t0 + _rampTimeInUs;
      _t2 = _t1 + _periodInUs * ( float )_periodCount;
      _t3 = _t2 + _rampTimeInUs;
      _t4 = _t0 + _bigDeltaInUs;
      _t5 = _t4 + _rampTimeInUs;
      _t6 = _t5 + _periodInUs * ( float )_periodCount;
      _t7 = _t6 + _rampTimeInUs;

      if ( _t7 > this->_echoTimeInUs )
      {
    
        throw std::runtime_error(
                "the end of the Trapezoid-Cosine-OGSE diffusion module goes "
                "beyond the echo time" );

      }

      _trapezoidCosineFunction.reset( new gkg::TrapezoidCosineFunction( 
                                             _rampTimeInUs,
                                             _periodInUs ) );

      _diffusionGradientWaveformStartingIndex = ( int32_t )( _t0 /
                                                             _timeStepInUs );
      _diffusionGradientWaveformEndingIndex = ( int32_t )( _t3 /
                                                             _timeStepInUs );
      if ( ( float )_diffusionGradientWaveformEndingIndex !=
           ( _t3 / _timeStepInUs ) )
      {

        ++ _diffusionGradientWaveformEndingIndex;

      }

    }

  }
  GKG_CATCH( "gkg::OGSESequence::OGSESequence( "
             "const gkg::Dictionary& parameters, "
             "float timeStepInUs, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::OGSESequence::~OGSESequence()
{
}


void gkg::OGSESequence::getNormalizedValueAt(
                                       float timeInUs, 
                                       float& normalizedGradientValue ) const
{

  try
  {

    if ( _waveformType == "trapezoid-sine" )
    {
    
      this->getNormalizedTrapezoidSineValueAt( timeInUs,
                                               normalizedGradientValue );
      
    }
    else if ( _waveformType == "trapezoid-cosine" )
    {
    
      this->getNormalizedTrapezoidCosineValueAt( timeInUs,
                                                 normalizedGradientValue );
      
    }
    else if ( _waveformType == "sine" )
    {
    
      this->getNormalizedSineValueAt( timeInUs,
                                      normalizedGradientValue );
      
    }
    else if ( _waveformType == "cosine" )
    {
    
      this->getNormalizedCosineValueAt( timeInUs,
                                        normalizedGradientValue );
      
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::getNormalizedValueAt( "
             "float timeInUs, "
             "float& normalizedGradientValue ) const" );


}


void gkg::OGSESequence::getNormalizedIntegralValueAt(
                                  float timeInUs, 
                                  float& normalizedIntegralGradientValue ) const
{

  try
  {

    if ( _waveformType == "trapezoid-sine" )
    {
    
      this->getNormalizedTrapezoidSineIntegralValueAt(
                                              timeInUs,
                                              normalizedIntegralGradientValue );
      
    }
    else if ( _waveformType == "trapezoid-cosine" )
    {

      this->getNormalizedTrapezoidCosineIntegralValueAt(
                                              timeInUs,
                                              normalizedIntegralGradientValue );

    }
    else if ( _waveformType == "sine" )
    {
    
      this->getNormalizedSineIntegralValueAt( timeInUs,
                                              normalizedIntegralGradientValue );
      
    }
    else if ( _waveformType == "cosine" )
    {
    
      this->getNormalizedCosineIntegralValueAt(
                                              timeInUs,
                                              normalizedIntegralGradientValue );
      
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::getNormalizedIntegralValueAt( "
             "float timeInUs, "
             "float& normalizedIntegralGradientValue ) const" );


}


void gkg::OGSESequence::displayInformation( std::ofstream* osLog ) const
{

  try
  {

    if ( osLog )
    {
 
      *osLog << "diffusion scheme: OGSE" << std::endl;
      *osLog << "waveform type : " << _waveformType << std::endl;
      *osLog << "big delta in us : " << _bigDeltaInUs << std::endl;
      *osLog << "period in us : " << _periodInUs << std::endl;
      *osLog << "period count : " << _periodCount << std::endl;
      *osLog << "echo time in us : " << this->_echoTimeInUs << std::endl;
 
    }
    else
    {
 
      std::cout << "diffusion scheme: OGSE" << std::endl;
      std::cout << "waveform type : " << _waveformType << std::endl;
      std::cout << "big delta in us : " << _bigDeltaInUs << std::endl;
      std::cout << "period in us : " << _periodInUs << std::endl;
      std::cout << "period count : " << _periodCount << std::endl;
      std::cout << "echo time in us : " << this->_echoTimeInUs << std::endl;
 
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::displayInformation( "
             "std::ofstream* osLog ) const" );


}


void gkg::OGSESequence::saveDiffusionInformation(
                                             const std::string& fileName ) const
{

  try
  {

    // calling method from the mother class
    this->gkg::MRISequence::saveDiffusionInformation( fileName );

    // saving further information specific to the PGSE sequence scheme
    std::ofstream os( ( fileName + ".ogse" ).c_str() );

    os << "waveform_type " <<  _waveformType << std::endl;
    os << "big_delta_in_us " <<  _bigDeltaInUs << std::endl;
    os << "period_in_us " <<  _periodInUs << std::endl;
    os << "period_count " <<  _periodCount << std::endl;
    os << "echo_time_in_us " <<  this->_echoTimeInUs << std::endl;

    os.close();

  }
  GKG_CATCH( "void gkg::OGSESequence::saveDiffusionInformation( "
             "const std::string& fileName ) const" );

}


std::string gkg::OGSESequence::getStaticName()
{

  try
  {

    return "ogse";

  }
  GKG_CATCH( "std::string gkg::OGSESequence::getStaticName()" );

}


void gkg::OGSESequence::getNormalizedTrapezoidSineValueAt(
                                       float timeInUs,
                                       float& normalizedGradientValue  ) const
{

  try
  {

   /*
       input parameters:
        - _timeOffsetToDiffusionModuleInUs
        - _bigDeltaInUs
        - _periodInUs;
        - _periodCount;
        - _echoTimeInUs
 
          ^
          |
      1.0 + - - ||||||||||||||||||| - - - - - -|||||||||||||||||||
          |     |||||||||||||||||||            |||||||||||||||||||
          |     |||||||||||||||||||            |||||||||||||||||||
          +-----+-----------------+------------+-----------------+----+-> t
          0     |                 t1           t2               t3    |
                |                 |            |                 |    |
                |     _periodInUs * _periodCount                 |    |
               >+-----------------+<-----------------                 |
                |                              |   _bigDeltaInUs      |
               >+------------------------------+<-------------------  |
                v                                                     v
              _timeOffsetToDiffusionModuleInUs=t0            _echoTimeInUs
    */

    if ( timeInUs <= _t0 )
    {
    
      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs < _t1 )
    {

      normalizedGradientValue =  _trapezoidSineFunction->getValueAt( 
                                                  timeInUs - _t0 );
      
    }
    else if ( timeInUs < _t2 )
    {
    
      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs < _t3 )
    {

      normalizedGradientValue =  -_trapezoidSineFunction->getValueAt( 
                                                  timeInUs - _t2 );

    }
    else
    {
    
      normalizedGradientValue = 0.0f;
    
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::getNormalizedTrapezoidSineValueAt( "
             "float timeInUs, "
             "float& normalizedGradientValue ) const" );

}


void gkg::OGSESequence::getNormalizedTrapezoidSineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue  ) const
{

  try
  {

    float accumulatedIntegral = 0.0f;

    if ( timeInUs <= _t0 )
    {
    
      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {
    
      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue =
        _trapezoidSineFunction->getIntegralValueAt( timeInUs - _t0 );

    }
    else if ( timeInUs <= _t2 )
    {

      this->getNormalizedTrapezoidSineIntegralValueAt( _t1,
                                                       accumulatedIntegral );    
      normalizedIntegralGradientValue = 0.0f;

    
    }
    else if ( timeInUs <= _t3 )
    {
    
      this->getNormalizedTrapezoidSineIntegralValueAt( _t2,
                                                       accumulatedIntegral );    
      normalizedIntegralGradientValue =
        -_trapezoidSineFunction->getIntegralValueAt( timeInUs - _t2 );
    
    }

    normalizedIntegralGradientValue += accumulatedIntegral;

  }
  GKG_CATCH( "void "
             "gkg::OGSESequence::getNormalizedTrapezoidSineIntegralValueAt( "
             "float timeInUs, "
             "float& normalizedIntegralGradientValue  ) const" );

}


void gkg::OGSESequence::getNormalizedTrapezoidCosineValueAt(
                                       float timeInUs,
                                       float& normalizedGradientValue  ) const
{

  try
  {

    /*
       input parameters:
        - _timeOffsetToDiffusionModuleInUs
        - _bigDeltaInUs
        - _periodInUs;
        - _periodCount;
        - _echoTimeInUs
 
          ^
          |
      1.0 + - - - /||||||||||||||||\ - - - - - - /||||||||||||||||\
          |      / |||||||||||||||| \           / |||||||||||||||| \
          |     /  ||||||||||||||||  \         /  ||||||||||||||||  \
          +-----+--+--------------+---+--------+-----------------+---+-+-> t
          0     |  t1             t2  t3      t4  t5             t6 t7 |
                |<-> _rampTimeInUs <->                                 |
                |     _periodInUs * _periodCount                       |
               >+--+--------------+<-----------------                  |
                |                              |   _bigDeltaInUs       |
               >+------------------------------+<-------------------   |
                v                                                      v
              _timeOffsetToDiffusionModuleInUs=t0            _echoTimeInUs
    */

    if ( timeInUs <= _t0 )
    {
    
      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs < _t1 )
    {

      normalizedGradientValue = ( timeInUs - _t0 ) /
                                _rampTimeInUs;

      
    }
    else if ( timeInUs < _t2 )
    {
    
      normalizedGradientValue =  _trapezoidCosineFunction->getValueAt( 
                                                  timeInUs - _t1 );
                                           
    }
    else if ( timeInUs < _t3 )
    {
    
      normalizedGradientValue = ( _t3 - timeInUs ) / _rampTimeInUs;

    }
    else if ( timeInUs < _t4 )
    {

      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs < _t5 )
    {
    
      normalizedGradientValue = -( timeInUs - _t4 ) / _rampTimeInUs;

    }
    else if ( timeInUs < _t6 )
    {
    
      normalizedGradientValue = -_trapezoidCosineFunction->getValueAt( 
                                                  timeInUs - _t5 );
    
    }
    else if ( timeInUs < _t7 )
    {
    
      normalizedGradientValue = -( _t7 - timeInUs ) / _rampTimeInUs;
    
    }
    else
    {
    
      normalizedGradientValue = 0.0f;
    
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::getNormalizedTrapezoidSineValueAt( "
             "float timeInUs, "
             "float& normalizedGradientValue ) const" );

}


void gkg::OGSESequence::getNormalizedTrapezoidCosineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue  ) const
{

  try
  {


    float accumulatedIntegral = 0.0f;
    if ( timeInUs <= _t0 )
    {

      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {

      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue =
        ( 1.0f / ( _t1 - _t0 ) ) *
        ( SQ( timeInUs ) / 2.0f - _t0 * timeInUs + SQ( _t0 ) / 2.0f );
      
    }
    else if ( timeInUs <= _t2 )
    {
    
      this->getNormalizedTrapezoidCosineIntegralValueAt( _t1,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        _trapezoidCosineFunction->getIntegralValueAt( timeInUs - _t1 );
                                           
    }
    else if ( timeInUs <= _t3 )
    {
    
      this->getNormalizedTrapezoidCosineIntegralValueAt( _t2,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        ( -1.0f / ( _t3 - _t2 ) ) *
        ( SQ( timeInUs ) / 2.0f - SQ( _t2 ) / 2.0f - _t3 * timeInUs + 
          _t2 * _t3 );

    }
    else if ( timeInUs <= _t4 )
    {

      this->getNormalizedTrapezoidCosineIntegralValueAt( _t3,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        0.0f;

    }
    else if ( timeInUs <= _t5 )
    {
    
      this->getNormalizedTrapezoidCosineIntegralValueAt( _t4,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        ( -1.0f / ( _t5 - _t4 ) ) *
        ( SQ( timeInUs ) / 2.0f - _t4 * timeInUs + SQ( _t4 ) / 2.0f );

    }
    else if ( timeInUs <= _t6 )
    {
    
      this->getNormalizedTrapezoidCosineIntegralValueAt( _t5,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        -_trapezoidCosineFunction->getIntegralValueAt( timeInUs - _t5 );
    
    }
    else if ( timeInUs <= _t7 )
    {
    
      this->getNormalizedTrapezoidCosineIntegralValueAt( _t6,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        ( 1.0f / ( _t7 - _t6 ) ) *
        ( SQ( timeInUs ) / 2.0f - SQ( _t6 ) / 2.0f - _t7 * timeInUs + 
          _t6 * _t7 );
    
    }
    else
    {
    
      this->getNormalizedTrapezoidCosineIntegralValueAt( _t7,
                                                         accumulatedIntegral );
      normalizedIntegralGradientValue =
        0.0f;
    
    }

    normalizedIntegralGradientValue += accumulatedIntegral;

  }
  GKG_CATCH( "void "
             "gkg::OGSESequence::getNormalizedTrapezoidCosineIntegralValueAt( "
             "float timeInUs, "
             "float& normalizedIntegralGradientValue  ) const" );

}



void gkg::OGSESequence::getNormalizedSineValueAt(
                                       float timeInUs,
                                       float& normalizedGradientValue  ) const
{

  try
  {

    /*
       input parameters:
        - _timeOffsetToDiffusionModuleInUs
        - _bigDeltaInUs
        - _periodInUs;
        - _periodCount;
        - _echoTimeInUs
 
          ^
          |
      1.0 + - - ||||||||||||||||||| - - - - - -|||||||||||||||||||
          |     |||||||||||||||||||            |||||||||||||||||||
          |     |||||||||||||||||||            |||||||||||||||||||
          +-----+-----------------+------------+-----------------+----+-> t
          0     |                 t1           t2               t3    |
                |                 |            |                 |    |
                |     _periodInUs * _periodCount                 |    |
               >+-----------------+<-----------------                 |
                |                              |   _bigDeltaInUs      |
               >+------------------------------+<-------------------  |
                v                                                     v
              _timeOffsetToDiffusionModuleInUs=t0            _echoTimeInUs
    */

    if ( timeInUs <= _t0 )
    {
    
      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {

      normalizedGradientValue =  ( float )std::sin( _angularFrequency * 
                                                    ( timeInUs - _t0 ) );
      
    }
    else if ( timeInUs <= _t2 )
    {
    
      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t3 )
    {

      normalizedGradientValue =  -( float )std::sin( _angularFrequency *
                                                     ( timeInUs - _t2 ) );

    }
    else
    {
    
      normalizedGradientValue = 0.0f;
    
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::getNormalizedSineValueAt( "
             "float timeInUs, "
             "float& normalizedGradientValue ) const" );

}


void gkg::OGSESequence::getNormalizedSineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue  ) const
{

  try
  {

    float accumulatedIntegral = 0.0f;

    if ( timeInUs <= _t0 )
    {
    
      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {
    
      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue =
        ( 1.0f / _angularFrequency ) *
        ( 1.0f - ( float )std::cos( _angularFrequency * ( timeInUs - _t0 ) ) );

    }
    else if ( timeInUs <= _t2 )
    {

      this->getNormalizedSineIntegralValueAt( _t1, accumulatedIntegral );    
      normalizedIntegralGradientValue = 0.0f;

    
    }
    else if ( timeInUs <= _t3 )
    {
    
      this->getNormalizedSineIntegralValueAt( _t2, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( -1.0f / _angularFrequency ) *
        ( 1.0f - ( float )std::cos( _angularFrequency * ( timeInUs - _t2 ) ) );
    
    }

    normalizedIntegralGradientValue += accumulatedIntegral;

  }
  GKG_CATCH( "void "
             "gkg::OGSESequence::getNormalizedSineIntegralValueAt( "
             "float timeInUs, "
             "float& normalizedIntegralGradientValue  ) const" );

}


void gkg::OGSESequence::getNormalizedCosineValueAt(
                                       float timeInUs,
                                       float& normalizedGradientValue  ) const
{

  try
  {

    /*
       input parameters:
        - _timeOffsetToDiffusionModuleInUs
        - _bigDeltaInUs
        - _periodInUs;
        - _periodCount;
        - _echoTimeInUs
 
          ^
          |
      1.0 + - - - /||||||||||||||||\ - - - - - - /||||||||||||||||\
          |      / |||||||||||||||| \           / |||||||||||||||| \
          |     /  ||||||||||||||||  \         /  ||||||||||||||||  \
          +-----+--+--------------+---+--------+-----------------+---+-+-> t
          0     |  t1             t2  t3      t4  t5             t6 t7 |
                |<-> _rampTimeInUs <->                                 |
                |     _periodInUs * _periodCount                       |
               >+--+--------------+<-----------------                  |
                |                              |   _bigDeltaInUs       |
               >+------------------------------+<-------------------   |
                v                                                      v
              _timeOffsetToDiffusionModuleInUs=t0            _echoTimeInUs
    */

    if ( timeInUs <= _t0 )
    {
    
      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {

      normalizedGradientValue = ( timeInUs - _t0 ) /
                                _rampTimeInUs;

      
    }
    else if ( timeInUs <= _t2 )
    {
    
      normalizedGradientValue =  ( float )std::cos( _angularFrequency * 
                                                    ( timeInUs - _t1 ) );
                                           
    }
    else if ( timeInUs <= _t3 )
    {
    
      normalizedGradientValue = ( _t3 - timeInUs ) / _rampTimeInUs;

    }
    else if ( timeInUs <= _t4 )
    {

      normalizedGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t5 )
    {
    
      normalizedGradientValue = -( timeInUs - _t4 ) / _rampTimeInUs;

    }
    else if ( timeInUs <= _t6 )
    {
    
      normalizedGradientValue = -( float )std::cos( _angularFrequency * 
                                                   ( timeInUs - _t5 ) );
    
    }
    else if ( timeInUs <= _t7 )
    {
    
      normalizedGradientValue = -( _t7 - timeInUs ) / _rampTimeInUs;
    
    }
    else
    {
    
      normalizedGradientValue = 0.0f;
    
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::getNormalizedCosineValueAt( "
             "float timeInUs, "
             "float& ) const" );

}


void gkg::OGSESequence::getNormalizedCosineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue  ) const
{

  try
  {

    float accumulatedIntegral = 0.0f;
    if ( timeInUs <= _t0 )
    {
    
      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {

      accumulatedIntegral = 0.0f;
      normalizedIntegralGradientValue =
        ( 1.0f / ( _t1 - _t0 ) ) *
        ( SQ( timeInUs ) / 2.0f - _t0 * timeInUs + SQ( _t0 ) / 2.0f );

      
    }
    else if ( timeInUs <= _t2 )
    {
    
      this->getNormalizedCosineIntegralValueAt( _t1, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( 1.0f / _angularFrequency ) *
        ( float )std::sin( _angularFrequency * ( timeInUs - _t1 ) );
                                           
    }
    else if ( timeInUs <= _t3 )
    {
    
      this->getNormalizedCosineIntegralValueAt( _t2, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( -1.0f / ( _t3 - _t2 ) ) *
        ( SQ( timeInUs ) / 2.0f - SQ( _t2 ) / 2.0f - _t3 * timeInUs + 
          _t2 * _t3 );

    }
    else if ( timeInUs <= _t4 )
    {

      this->getNormalizedCosineIntegralValueAt( _t3, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        0.0f;

    }
    else if ( timeInUs <= _t5 )
    {
    
      this->getNormalizedCosineIntegralValueAt( _t4, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( -1.0f / ( _t5 - _t4 ) ) *
        ( SQ( timeInUs ) / 2.0f - _t4 * timeInUs + SQ( _t4 ) / 2.0f );

    }
    else if ( timeInUs <= _t6 )
    {
    
      this->getNormalizedCosineIntegralValueAt( _t5, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( -1.0f / _angularFrequency ) *
        ( float )std::sin( _angularFrequency * ( timeInUs - _t5 ) );
    
    }
    else if ( timeInUs <= _t7 )
    {
    
      this->getNormalizedCosineIntegralValueAt( _t6, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( 1.0f / ( _t7 - _t6 ) ) *
        ( SQ( timeInUs ) / 2.0f - SQ( _t6 ) / 2.0f - _t7 * timeInUs + 
          _t6 * _t7 );
    
    }
    else
    {
    
      this->getNormalizedCosineIntegralValueAt( _t7, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        0.0f;
    
    }

    normalizedIntegralGradientValue += accumulatedIntegral;

  }
  GKG_CATCH( "void "
             "gkg::OGSESequence::getNormalizedCosineIntegralValueAt( "
             "float timeInUs, "
             "float& normalizedIntegralGradientValue  ) const" );

}


//
// adding the OGSESequence creator to the MRISequence factory
//

RegisterMRISequenceCreator( gkg, OGSESequence );


#undef SQ
