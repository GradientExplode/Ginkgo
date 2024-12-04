#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T2StarMGRERelaxometryMCMCFunction_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T2StarMGRERelaxometryMCMCFunction_h_


#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T2StarMGREAcquisitionParameterSet.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class T2StarMGRERelaxometryMCMCFunction :
               public SingleCompartmentRelaxometryMCMCFunction,
               public Creator3Arg< T2StarMGRERelaxometryMCMCFunction,
                                   SingleCompartmentRelaxometryMCMCFunction,
                                   const std::vector< int32_t >&,
                                   const std::vector< double >&,
                                   const RCPointer< AcquisitionParameterSet >& >
{

  public:

    
    T2StarMGRERelaxometryMCMCFunction( 
            const std::vector< int32_t >& measurementCounts,
            const std::vector< double >& noiseStandardDeviations,
            const RCPointer< AcquisitionParameterSet >& acquisitionParameters );
    virtual ~T2StarMGRERelaxometryMCMCFunction();

    void getValuesAt( const Vector& parameters, Vector& values ) const; 

    static std::string getStaticName();

  protected:

    friend class Creator3Arg< T2StarMGRERelaxometryMCMCFunction,
                              SingleCompartmentRelaxometryMCMCFunction,
                              const std::vector< int32_t >&,
                              const std::vector< double >&,
                              const RCPointer< AcquisitionParameterSet >& >;

    const T2StarMGREAcquisitionParameterSet* _acquisitionParameterSet;

};


}



#endif
