#include <gkg-simulation-mri-sequence/TrapezoidCosineFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::TrapezoidCosineFunction::TrapezoidCosineFunction( float rampTimeInUs,
                                                       float periodInUs )
                           : _quarterPeriodInUs( periodInUs / 4.0f ),
                             _halfPeriodInUs( periodInUs / 2.0 ),
                             _trapezoidGradientPulse(
                                     0.0f,
                                     rampTimeInUs,
                                     _halfPeriodInUs - 2 * rampTimeInUs,
                                     rampTimeInUs,
                                     1.0f )
{
}


gkg::TrapezoidCosineFunction::~TrapezoidCosineFunction()
{
}
    

float gkg::TrapezoidCosineFunction::getValueAt( float timeInUs ) const
{

  try
  {

    float gradientValue = 0.0f;
    if ( ( int32_t )( timeInUs / _quarterPeriodInUs ) % 4 == 0 )
    {
    
      gradientValue = _trapezoidGradientPulse.getValueAt(
                  _quarterPeriodInUs + std::fmod( timeInUs, _quarterPeriodInUs ) );
                       
    }
    else if ( ( int32_t )( timeInUs / _quarterPeriodInUs ) % 4 == 1 )
    {

      gradientValue = -_trapezoidGradientPulse.getValueAt(
                                       std::fmod( timeInUs, _quarterPeriodInUs ) );

    }    
    else if ( ( int32_t )( timeInUs / _quarterPeriodInUs ) % 4 == 2 )
    {

      gradientValue = -_trapezoidGradientPulse.getValueAt(
                  _quarterPeriodInUs + std::fmod( timeInUs, _quarterPeriodInUs ) );

    }    
    else
    {

      gradientValue = _trapezoidGradientPulse.getValueAt(
                                       std::fmod( timeInUs, _quarterPeriodInUs ) );

    }    

    return gradientValue;

  }
   GKG_CATCH( "float gkg::TrapezoidCosineFunction::getValueAt( "
              "float timeInUs ) const" );

}


float gkg::TrapezoidCosineFunction::getIntegralValueAt( float timeInUs ) const
{

  try
  {

    float accumulatedIntegral = 0.0f;
    float integralGradientValue = 0.0f;

    if ( ( int32_t )( timeInUs / _quarterPeriodInUs ) % 4 == 0 )
    {
    
      accumulatedIntegral = -_trapezoidGradientPulse.getIntegralValueAt(
                                                           _quarterPeriodInUs );
      integralGradientValue = _trapezoidGradientPulse.getIntegralValueAt(
               _quarterPeriodInUs + std::fmod( timeInUs, _quarterPeriodInUs ) );
                        
    }
    else if ( ( int32_t )( timeInUs / _quarterPeriodInUs ) % 4 == 1 )
    {

      accumulatedIntegral = _trapezoidGradientPulse.getIntegralValueAt(
                                                           _quarterPeriodInUs );
      integralGradientValue = - _trapezoidGradientPulse.getIntegralValueAt(
                                    std::fmod( timeInUs, _quarterPeriodInUs ) );

    }    
    else if ( ( int32_t )( timeInUs / _quarterPeriodInUs ) % 4 == 2 )
    {

      accumulatedIntegral = +_trapezoidGradientPulse.getIntegralValueAt(
                                                           _quarterPeriodInUs );
      integralGradientValue = -_trapezoidGradientPulse.getIntegralValueAt(
               _quarterPeriodInUs + std::fmod( timeInUs, _quarterPeriodInUs ) );

    }    
    else
    {

      accumulatedIntegral = -_trapezoidGradientPulse.getIntegralValueAt(
                                                           _quarterPeriodInUs );
      integralGradientValue = +_trapezoidGradientPulse.getIntegralValueAt(
                                    std::fmod( timeInUs, _quarterPeriodInUs ) );

    }    

    integralGradientValue += accumulatedIntegral;

    return integralGradientValue;

  }
   GKG_CATCH( "float gkg::TrapezoidCosineFunction::getIntegralValueAt( "
              "float timeInUs ) const" );

}

