#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T2MSMEAcquisitionParameterSet_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T2MSMEAcquisitionParameterSet_h_


#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class T2MSMEAcquisitionParameterSet :
                         public AcquisitionParameterSet,
                         public Creator2Arg< T2MSMEAcquisitionParameterSet,
                                             AcquisitionParameterSet,
                                             const std::vector< int32_t >&,
                                             const std::vector< std::string >& >
{

  public:

    T2MSMEAcquisitionParameterSet(
                           const std::vector< int32_t >& measurementCounts,
                           const std::vector< std::string >& stringParameters );
    virtual ~T2MSMEAcquisitionParameterSet();

    const std::vector< Vector >& getEchoTimes() const;

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< T2MSMEAcquisitionParameterSet,
                              AcquisitionParameterSet,
                              const std::vector< int32_t >&,
                              const std::vector< std::string >& >;

    std::vector< Vector > _echoTimes;

};


}


#endif

