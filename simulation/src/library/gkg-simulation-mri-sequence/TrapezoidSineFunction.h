#ifndef _gkg_simulation_mri_sequence_TrapezoidSineFunction_h_
#define _gkg_simulation_mri_sequence_TrapezoidSineFunction_h_


#include <gkg-simulation-mri-sequence/TrapezoidGradientPulse.h>


namespace gkg
{

class TrapezoidSineFunction
{

  public:
  
    TrapezoidSineFunction( float rampTimeInUs,
                           float periodInUs );
    virtual ~TrapezoidSineFunction();
    
    float getValueAt( float timeInUs ) const;
    float getIntegralValueAt( float timeInUs ) const;
    
  protected:
  
    float _halfPeriodInUs;
    TrapezoidGradientPulse _trapezoidGradientPulse;

};


}


#endif

