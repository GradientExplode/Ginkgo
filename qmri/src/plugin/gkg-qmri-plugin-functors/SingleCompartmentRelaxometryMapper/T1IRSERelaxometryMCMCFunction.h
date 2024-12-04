#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1IRSERelaxometryMCMCFunction_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1IRSERelaxometryMCMCFunction_h_


#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T1IRSEAcquisitionParameterSet.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class T1IRSERelaxometryMCMCFunction :
               public SingleCompartmentRelaxometryMCMCFunction,
               public Creator3Arg< T1IRSERelaxometryMCMCFunction,
                                   SingleCompartmentRelaxometryMCMCFunction,
                                   const std::vector< int32_t >&,
                                   const std::vector< double >&,
                                   const RCPointer< AcquisitionParameterSet >& >
{

  public:

    
    T1IRSERelaxometryMCMCFunction( 
          const std::vector< int32_t >& measurementCounts,
          const std::vector< double >& noiseStandardDeviations,
          const RCPointer< AcquisitionParameterSet >& acquisitionParameterSet );
    virtual ~T1IRSERelaxometryMCMCFunction();

    void getValuesAt( const Vector& parameters, Vector& values ) const; 

    static std::string getStaticName();

  protected:

    friend class Creator3Arg< T1IRSERelaxometryMCMCFunction,
                              SingleCompartmentRelaxometryMCMCFunction,
                              const std::vector< int32_t >&,
                              const std::vector< double >&,
                              const RCPointer< AcquisitionParameterSet >& >;

    const T1IRSEAcquisitionParameterSet* _acquisitionParameterSet;

};


}



#endif
