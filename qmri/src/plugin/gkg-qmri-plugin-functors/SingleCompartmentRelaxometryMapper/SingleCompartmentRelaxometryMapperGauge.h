#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMapperGauge_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMapperGauge_h_


#include <gkg-communication-thread/ThreadGauge.h> 


namespace gkg
{


class SingleCompartmentRelaxometryMapperGauge : public ThreadGauge< int32_t >
{

  public: 
  
    SingleCompartmentRelaxometryMapperGauge( int32_t maximumCount ); 
    
    void add( int32_t value ); 
    void reset(); 
    
  private: 
  
    void display(); 
    
    int32_t _count; 
    int32_t _maximumCount; 
    bool _siteIndexAlreadyDisplayed; 

};


}


#endif
