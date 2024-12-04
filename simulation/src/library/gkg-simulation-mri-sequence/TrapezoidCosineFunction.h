#ifndef _gkg_simulation_mri_sequence_TrapezoidCosineFunction_h_
#define _gkg_simulation_mri_sequence_TrapezoidCosineFunction_h_


#include <gkg-simulation-mri-sequence/TrapezoidGradientPulse.h>


namespace gkg
{

class TrapezoidCosineFunction
{

  public:
  
    TrapezoidCosineFunction( float rampTimeInUs,
                             float periodInUs );
    virtual ~TrapezoidCosineFunction();
    
    float getValueAt( float timeInUs ) const;
    float getIntegralValueAt( float timeInUs ) const;
    
  protected:
  
    float _quarterPeriodInUs;
    float _halfPeriodInUs;
    TrapezoidGradientPulse _trapezoidGradientPulse;

};


}


#endif

