#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryNLPFunction_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class SingleCompartmentRelaxometryNLPFunction: 
                                             public NonLinearProgrammingFunction
{

  public:
    
    SingleCompartmentRelaxometryNLPFunction( 
      RCPointer< SingleCompartmentRelaxometryMCMCFunction > 
                                     singleCompartmentRelaxometryMCMCFunction ); 
    
    virtual ~SingleCompartmentRelaxometryNLPFunction(); 
    
    void setRealMeasurements( const Vector& realMeasurements ); 
    
    double getValueAt( const Vector& parameters ) const; 
    
  protected: 
  
    double getLogLikelihood( const Vector& currentMeasurements ) const; 
    
    RCPointer< SingleCompartmentRelaxometryMCMCFunction > 
                                      _singleCompartmentRelaxometryMCMCFunction; 

    Vector _realMeasurements; 
    mutable Vector _currentMeasurements;

};


}


#endif
