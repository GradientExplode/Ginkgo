#ifndef _gkg_qmri_plugin_functors_MultiCompartmentRelaxometryMapper_MultiCompartmentRelaxometryNLPFunction_h_
#define _gkg_qmri_plugin_functors_MultiCompartmentRelaxometryMapper_MultiCompartmentRelaxometryNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCFunction.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class MultiCompartmentRelaxometryNLPFunction: 
                                             public NonLinearProgrammingFunction
{

  public:
    
    MultiCompartmentRelaxometryNLPFunction( 
      RCPointer< MultiCompartmentRelaxometryMCMCFunction > 
                                    multiCompartmentRelaxometryMCMCFunction ); 
    
    virtual ~MultiCompartmentRelaxometryNLPFunction(); 
    
    void setRealMeasurements( const Vector& realMeasurements ); 
    
    double getValueAt( const Vector& parameters ) const; 
    
  protected: 
  
    double getLogLikelihood( const Vector& currentMeasurements ) const; 
    
    RCPointer< MultiCompartmentRelaxometryMCMCFunction > 
                                       _multiCompartmentRelaxometryMCMCFunction; 
    Vector _realMeasurements; 
    
    int32_t _t1WeightedVolumeCount; 
    int32_t _t2WeightedVolumeCount; 
    int32_t _globalT1WeightedCount;
    int32_t _globalT2WeightedCount;
    std::vector< int32_t > _t1WeightedCounts; 
    std::vector< int32_t > _t2WeightedCounts; 
    std::vector< double > _t1NoiseVariances; 
    std::vector< double > _t2NoiseVariances;
    std::vector< double > _twiceT1NoiseVariances;
    std::vector< double > _twiceT2NoiseVariances;
    double _twoPi;
    int32_t _measurementCount;
    mutable Vector _currentMeasurements;

};


}


#endif
