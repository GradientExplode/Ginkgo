#include <gkg-simulation-mri-sequence/TrapezoidSineFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::TrapezoidSineFunction::TrapezoidSineFunction( float rampTimeInUs,
                                                   float periodInUs )
                           : _halfPeriodInUs( periodInUs / 2.0 ),
                             _trapezoidGradientPulse(
                                     0.0f,
                                     rampTimeInUs,
                                     _halfPeriodInUs - 2 * rampTimeInUs,
                                     rampTimeInUs,
                                     1.0f )
{
}


gkg::TrapezoidSineFunction::~TrapezoidSineFunction()
{
}
    

float gkg::TrapezoidSineFunction::getValueAt( float timeInUs ) const
{

  try
  {

    float gradientValue = 0.0f;
    if ( ( int32_t )( timeInUs / _halfPeriodInUs ) % 2 )
    {
    
      gradientValue = -_trapezoidGradientPulse.getValueAt(
                                       std::fmod( timeInUs, _halfPeriodInUs ) );
                        
    }
    else
    {
    
      gradientValue = _trapezoidGradientPulse.getValueAt(
                                       std::fmod( timeInUs, _halfPeriodInUs ) );
                        
    }
    return gradientValue;

  }
   GKG_CATCH( "float gkg::TrapezoidSineFunction::getValueAt( "
              "float timeInUs ) const" );

}



float gkg::TrapezoidSineFunction::getIntegralValueAt( float timeInUs ) const
{

  try
  {

    float accumulatedIntegral = 0.0f;
    float integralGradientValue = 0.0f;

    // if we are on a negative lobe, then the integral is equal to the area of
    // the former positive lobe (eg the first half period, since the previous
    // periods compensate each other ) plus the current part of the negative
    // lobe
    if ( ( int32_t )( timeInUs / _halfPeriodInUs ) % 2 )
    {
    
      accumulatedIntegral = _trapezoidGradientPulse.getIntegralValueAt(
                                                              _halfPeriodInUs );
      integralGradientValue = -_trapezoidGradientPulse.getIntegralValueAt(
                                       std::fmod( timeInUs, _halfPeriodInUs ) );
                        
    }
    // if we are on a positive, then the integral is equal to the current part 
    // of the positive lobe (since the previous lobes compensate each other )
    else
    {
    
      accumulatedIntegral = 0.0f;
      integralGradientValue = _trapezoidGradientPulse.getIntegralValueAt(
                                       std::fmod( timeInUs, _halfPeriodInUs ) );
                        
    }

    integralGradientValue += accumulatedIntegral;

    return integralGradientValue;

  }
   GKG_CATCH( "float gkg::TrapezoidSineFunction::getIntegralValueAt( "
              "float timeInUs ) const" );

}

