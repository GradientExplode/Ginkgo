#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1VFASPGRAcquisitionParameterSet_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_T1VFASPGRAcquisitionParameterSet_h_


#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class T1VFASPGRAcquisitionParameterSet :
                         public AcquisitionParameterSet,
                         public Creator2Arg< T1VFASPGRAcquisitionParameterSet,
                                             AcquisitionParameterSet,
                                             const std::vector< int32_t >&,
                                             const std::vector< std::string >& >
{

  public:

    T1VFASPGRAcquisitionParameterSet(
                           const std::vector< int32_t >& measurementCounts,
                           const std::vector< std::string >& stringParameters );
    virtual ~T1VFASPGRAcquisitionParameterSet();

    const std::vector< Vector >& getRepetitionTimes() const;
    const std::vector< Vector >& getFlipAngles() const;

    bool hasB1Mapping() const;
    const RCPointer< Volume< float > >& getB1MappingVolumePointer() const;

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< T1VFASPGRAcquisitionParameterSet,
                              AcquisitionParameterSet,
                              const std::vector< int32_t >&,
                              const std::vector< std::string >& >;

    std::vector< Vector > _repetitionTimes;
    std::vector< Vector > _flipAngles; 

    bool _hasB1Mapping;
    RCPointer< Volume< float > > _b1MappingVolumePointer; 

};


}


#endif

