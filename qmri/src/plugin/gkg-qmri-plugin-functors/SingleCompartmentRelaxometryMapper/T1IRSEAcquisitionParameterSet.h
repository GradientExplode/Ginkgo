#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1IRSEAcquisitionParameterSet_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1IRSEAcquisitionParameterSet_h_


#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class T1IRSEAcquisitionParameterSet :
                         public AcquisitionParameterSet,
                         public Creator2Arg< T1IRSEAcquisitionParameterSet,
                                             AcquisitionParameterSet,
                                             const std::vector< int32_t >&,
                                             const std::vector< std::string >& >
{

  public:

    T1IRSEAcquisitionParameterSet(
                           const std::vector< int32_t >& measurementCounts,
                           const std::vector< std::string >& stringParameters );
    virtual ~T1IRSEAcquisitionParameterSet();

    const std::vector< Vector >& getInversionTimes() const;

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< T1IRSEAcquisitionParameterSet,
                              AcquisitionParameterSet,
                              const std::vector< int32_t >&,
                              const std::vector< std::string >& >;

    std::vector< Vector > _inversionTimes;

};


}


#endif

