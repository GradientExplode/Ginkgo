#include <gkg-simulation-mri-sequence/TrapezoidGradientPulse.h>
#include <gkg-core-exception/Exception.h>
#include <string>


#define SQ( x ) ( ( x ) * ( x ) )


/*
              ^
              |
    magnitude +- - - - - -  +-------------+
              |            /|             |\ 
              |           / |             | \ 
              |          /  |             |  \ 
              |         /   |             |   \
              |        /    |             |    \
              +-------+-----+-------------+-----+------->time
                    _t0   _t1           _t2   _t3

              _t0: _startingTime 
              _t1: += _startingTimeInUs
              _t2: += _plateauDuration
              _t3: += _endingRampDuration
*/


gkg::TrapezoidGradientPulse::TrapezoidGradientPulse(
                                           float startingTimeInUs,
                                           float startingRampDurationInUs,
                                           float plateauDurationInUs,
                                           float endingRampDurationInUs,
                                           float magnitudeInMilliTeslaPerMeter )
                            : _startingRampDurationInUs(
                                                     startingRampDurationInUs ),
                              _plateauDurationInUs( plateauDurationInUs ),
                              _endingRampDurationInUs( endingRampDurationInUs ),
                              _magnitudeInMilliTeslaPerMeter( 
                                                 magnitudeInMilliTeslaPerMeter )
{

  try
  {
    
    _t0 = startingTimeInUs;
    _t1 = _t0 + _startingRampDurationInUs;
    _t2 = _t1 + _plateauDurationInUs;
    _t3 = _t2 + _endingRampDurationInUs;

  }
  GKG_CATCH( "gkg::TrapezoidGradientPulse::TrapezoidGradientPulse( "
             "float startingTimeInUs, "
             "float startingRampDurationInUs, "
             "float plateauDurationInUs, "
             "float endingRampDurationInUs, "
             "float magnitudeInMilliTeslaPerMeter );" );

}


gkg::TrapezoidGradientPulse::~TrapezoidGradientPulse()
{
}


float gkg::TrapezoidGradientPulse::getStartingRampDurationInUs() const
{

  try
  {

    return _startingRampDurationInUs;

  }
  GKG_CATCH( "float "
             "gkg::TrapezoidGradientPulse::"
             "getStartingRampDurationInUs() const" );

}


float gkg::TrapezoidGradientPulse::getPlateauDurationInUs() const
{

  try
  {

    return _plateauDurationInUs;

  }
  GKG_CATCH( "float gkg::TrapezoidGradientPulse::"
             "getPlateauDurationInUs() const" );

}


float gkg::TrapezoidGradientPulse::getEndingRampDurationInUs() const
{

  try
  {

    return _endingRampDurationInUs;

  }
  GKG_CATCH( "float "
             "gkg::TrapezoidGradientPulse::getEndingRampDurationInUs() const" );

}


float gkg::TrapezoidGradientPulse::getMagnitudeInMilliTeslaPerMeter() const
{

  try
  {

    return _magnitudeInMilliTeslaPerMeter;

  }
  GKG_CATCH( "float gkg::TrapezoidGradientPulse::"
             "getMagnitudeInMilliTeslaPerMeter() const" );

}


float gkg::TrapezoidGradientPulse::getValueAt( float timeInUs ) const
{

  try
  {

    float gradientValue = 0.0f;


    if ( timeInUs <= _t0 )
    {

      gradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {

      gradientValue = _magnitudeInMilliTeslaPerMeter *
                      ( timeInUs - _t0 ) / ( _t1 - _t0 );

    }
    else if ( timeInUs <= _t2 )
    {

      gradientValue = _magnitudeInMilliTeslaPerMeter;

    }
    else if ( timeInUs <= _t3 )
    {

      gradientValue = _magnitudeInMilliTeslaPerMeter * 
                      ( 1.0f - ( timeInUs - _t2 ) / ( _t3 - _t2 ) );

    }

    return gradientValue;

  }
  GKG_CATCH( "float gkg::TrapezoidGradientPulse::getValueAt( "
             "float timeInUs ) const" );

}


float gkg::TrapezoidGradientPulse::getIntegralValueAt( float timeInUs ) const
{

  try
  {

    float accumulatedIntegral = 0.0f;
    float integralGradientValue = 0.0f;

    if ( timeInUs <= _t0 )
    {
    
      accumulatedIntegral = 0.0f;
      integralGradientValue = 0.0f;

    }
    else if ( timeInUs <= _t1 )
    {

      accumulatedIntegral = 0.0f;
      integralGradientValue = _magnitudeInMilliTeslaPerMeter *
                              SQ( timeInUs - _t0 ) /
                              ( 2.0f * ( _t1 - _t0 ) );

    }
    else if ( timeInUs <= _t2 )
    {

      accumulatedIntegral = this->getIntegralValueAt( _t1 );
      integralGradientValue = _magnitudeInMilliTeslaPerMeter *
                              ( timeInUs - _t1 );

    }
    else if ( timeInUs <= _t3 )
    {

      accumulatedIntegral = this->getIntegralValueAt( _t2 );
      integralGradientValue = - ( _magnitudeInMilliTeslaPerMeter / 
                                  ( _t3 - _t2 ) ) *
                              ( SQ( timeInUs ) / 2.0f - SQ( _t2 ) / 2.0f - 
                                _t3 * timeInUs +  _t2 * _t3 );

    }
    else
    {

      accumulatedIntegral = this->getIntegralValueAt( _t3 );
      integralGradientValue = 0.0f;

    }

    integralGradientValue += accumulatedIntegral;

    return integralGradientValue;

  }
  GKG_CATCH( "float gkg::TrapezoidGradientPulse::getIntegralValueAt( "
             "float timeInUs ) const" );

}
