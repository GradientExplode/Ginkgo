#include <gkg-simulation-mri-sequence/PGSESequence.h>
#include <gkg-simulation-mri-sequence/MRISequenceFactory.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>


#define SQ( x ) ( x * x )


gkg::PGSESequence::PGSESequence( const gkg::Dictionary& parameters,
                                 float timeStepInUs,
                                 bool verbose,
                                 std::ofstream* osLog )
                  : gkg::MRISequence( parameters, timeStepInUs, verbose, osLog )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting big delta in ms and converting it into us
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary::const_iterator
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
    // collecting little delta in ms and converting it into us
    ////////////////////////////////////////////////////////////////////////////

    i = parameters.find( "little_delta_in_ms" );
    if ( i == parameters.end() )
    {

      throw std::runtime_error( "a 'little_delta_in_ms' item is required" );

    }
    _littleDeltaInUs = ( float )i->second->getScalar() * 1000.0f;

    if ( verbose )
    {

      if ( osLog )
      {
 
        *osLog << "little delta in us : " << _littleDeltaInUs << std::endl;
 
      }
      else
      {
 
        std::cout << "little delta in us : " << _littleDeltaInUs << std::endl;
 
      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////
    
    if ( _littleDeltaInUs <= 2 * _rampTimeInUs )
    {
    
      throw std::runtime_error(
        "little delta is chosen too short with respect to the "
        "maximum gradient magnitude and prescribed slewrate" );
    
    }
    
    ////////////////////////////////////////////////////////////////////////////
    // precomputing singular profile time point(s)
    ////////////////////////////////////////////////////////////////////////////
    _t0 = _timeOffsetToDiffusionModuleInUs;
    _t1 = _t0 + _rampTimeInUs;
    _t2 = _t0 + _littleDeltaInUs - _rampTimeInUs;
    _t3 = _t0 + _littleDeltaInUs;
    _t4 = _t0 + _bigDeltaInUs;
    _t5 = _t4 + _rampTimeInUs;
    _t6 = _t4 + _littleDeltaInUs - _rampTimeInUs;
    _t7 = _t4 + _littleDeltaInUs;

//     std::cout << "_t0=" << _t0 << std::endl;
//     std::cout << "_t1=" << _t1 << std::endl;
//     std::cout << "_t2=" << _t2 << std::endl;
//     std::cout << "_t3=" << _t3 << std::endl;
//     std::cout << "_t4=" << _t4 << std::endl;
//     std::cout << "_t5=" << _t5 << std::endl;
//     std::cout << "_t6=" << _t6 << std::endl;
//     std::cout << "_t7=" << _t7 << std::endl;

    if ( _t7 > this->_echoTimeInUs )
    {
    
      throw std::runtime_error( "the end of the PGSE diffusion module goes "
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
  GKG_CATCH( "gkg::PGSESequence::PGSESequence( "
             "const gkg::Dictionary& parameters, "
             "float timeStepInUs, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::PGSESequence::~PGSESequence()
{
}


void gkg::PGSESequence::getNormalizedValueAt(
                                       float timeInUs, 
                                       float& normalizedGradientValue ) const
{

  try
  {

    /*
       input parameters:
        - _timeOffsetToDiffusionModuleInUs
        - _bigDeltaInUs
        - _littleDeltaInUs
        - _rampTimeInUs
        - _echoTimeInUs
 
          ^
          |
      1.0 + - - -  +-----------+  - - - - - - -   +-----------+
          |       /|            \                /             \
          |      / |          t2 \t3          t4/ t5         t6 \
          +-----+--+-----------+--+------------+--+-----------+--+----+-> t
          0     |  | _rampTimeInUs|            |                 t7   |
               >+--+<-------------|            |                      |
                |  t1             |  _littleDeltaInUs                 |
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
    
      normalizedGradientValue = ( timeInUs - _t0 ) /
                                _rampTimeInUs;

    }
    else if ( timeInUs <= _t2 )
    {
    
      normalizedGradientValue = 1.0f;

    
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
    
      normalizedGradientValue = -1.0f;
    
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
  GKG_CATCH( "void gkg::PGSESequence::getNormalizedValueAt( "
             "float timeInUs, "
             "float& normalizedGradientValue ) const" );


}


void gkg::PGSESequence::getNormalizedIntegralValueAt(
                                  float timeInUs, 
                                  float& normalizedIntegralGradientValue ) const
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

      this->getNormalizedIntegralValueAt( _t1, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( timeInUs - _t1 );

    
    }
    else if ( timeInUs <= _t3 )
    {
    
      this->getNormalizedIntegralValueAt( _t2, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( -1.0f / ( _t3 - _t2 ) ) *
        ( SQ( timeInUs ) / 2.0f - SQ( _t2 ) / 2.0f - _t3 * timeInUs + 
          _t2 * _t3 );
    
    }
    else if ( timeInUs <= _t4 )
    {
    
      this->getNormalizedIntegralValueAt( _t3, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        0.0f;
    
    }
    else if ( timeInUs <= _t5 )
    {
    
      this->getNormalizedIntegralValueAt( _t4, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( -1.0f / ( _t5 - _t4 ) ) *
        ( SQ( timeInUs ) / 2.0f - _t4 * timeInUs + SQ( _t4 ) / 2.0f );

    }
    else if ( timeInUs <= _t6 )
    {
    
      this->getNormalizedIntegralValueAt( _t5, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        - ( timeInUs - _t5 );
    
    }
    else if ( timeInUs <= _t7 )
    {
    
      this->getNormalizedIntegralValueAt( _t6, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        ( 1.0f / ( _t7 - _t6 ) ) *
        ( SQ( timeInUs ) / 2.0f - SQ( _t6 ) / 2.0f - _t7 * timeInUs + 
          _t6 * _t7 );
    
    }
    else
    {
    
      this->getNormalizedIntegralValueAt( _t7, accumulatedIntegral );    
      normalizedIntegralGradientValue =
        0.0f;
    
    }

    normalizedIntegralGradientValue += accumulatedIntegral;

  }
  GKG_CATCH( "void gkg::PGSESequence::getNormalizedIntegralValueAt( "
             "float timeInUs, "
             "float& normalizedIntegralGradientValue ) const" );


}


void gkg::PGSESequence::displayInformation( std::ofstream* osLog ) const
{

  try
  {

    if ( osLog )
    {
 
      *osLog << "diffusion scheme: PGSE" << std::endl;
      *osLog << "big delta in us : " << _bigDeltaInUs << std::endl;
      *osLog << "little delta in us : " << _littleDeltaInUs << std::endl;
      *osLog << "echo time in us : " << this->_echoTimeInUs << std::endl;
 
    }
    else
    {
 
      std::cout << "diffusion scheme: PGSE" << std::endl;
      std::cout << "big delta in us : " << _bigDeltaInUs << std::endl;
      std::cout << "little delta in us : " << _littleDeltaInUs << std::endl;
      std::cout << "echo time in us : " << this->_echoTimeInUs << std::endl;
 
    }

  }
  GKG_CATCH( "void gkg::OGSESequence::displayInformation( "
             "std::ofstream* osLog ) const" );


}


void gkg::PGSESequence::saveDiffusionInformation(
                                             const std::string& fileName ) const
{

  try
  {

    // calling method from the mother class
    this->gkg::MRISequence::saveDiffusionInformation( fileName );

    // saving further information specific to the PGSE sequence scheme
    std::ofstream os( ( fileName + ".pgse" ).c_str() );

    os << "little_delta_in_us " <<  _littleDeltaInUs << std::endl;
    os << "big_delta_in_us " <<  _bigDeltaInUs << std::endl;
    os << "echo_time_in_us " <<  this->_echoTimeInUs << std::endl;

    os.close();


  }
  GKG_CATCH( "void gkg::PGSESequence::saveDiffusionInformation( "
             "const std::string& fileName ) const" );

}


std::string gkg::PGSESequence::getStaticName()
{

  try
  {

    return "pgse";

  }
  GKG_CATCH( "std::string gkg::PGSESequence::getStaticName()" );

}



//
// adding the PGSESequence creator to the MRISequence factory
//

RegisterMRISequenceCreator( gkg, PGSESequence );


#undef SQ
