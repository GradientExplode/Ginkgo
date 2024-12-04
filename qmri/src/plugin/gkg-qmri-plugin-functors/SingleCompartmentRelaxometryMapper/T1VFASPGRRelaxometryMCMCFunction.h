#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1VFASPGRRelaxometryMCMCFunction_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1VFASPGRRelaxometryMCMCFunction_h_


#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T1VFASPGRAcquisitionParameterSet.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class T1VFASPGRRelaxometryMCMCFunction :
              public SingleCompartmentRelaxometryMCMCFunction,
              public Creator3Arg< T1VFASPGRRelaxometryMCMCFunction,
                                  SingleCompartmentRelaxometryMCMCFunction,
                                  const std::vector< int32_t >&,
                                  const std::vector< double >&,
                                  const RCPointer< AcquisitionParameterSet >& >
{

  public:

    
    T1VFASPGRRelaxometryMCMCFunction( 
          const std::vector< int32_t >& measurementCounts,
          const std::vector< double >& noiseStandardDeviations,
          const RCPointer< AcquisitionParameterSet >& acquisitionParameterSet );
    virtual ~T1VFASPGRRelaxometryMCMCFunction();

    void setB1Scaling( float b1SCaling );

    void getValuesAt( const Vector& parameters, Vector& values ) const; 

    static std::string getStaticName();

  protected:

    friend class Creator3Arg< T1VFASPGRRelaxometryMCMCFunction,
                              SingleCompartmentRelaxometryMCMCFunction,
                              const std::vector< int32_t >&,
                              const std::vector< double >&,
                              const RCPointer< AcquisitionParameterSet >& >;

    const T1VFASPGRAcquisitionParameterSet* _acquisitionParameterSet;

};


}



#endif
