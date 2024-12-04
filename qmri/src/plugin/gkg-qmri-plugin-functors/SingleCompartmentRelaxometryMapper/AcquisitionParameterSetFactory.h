#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_AcquisitionParameterSetFactory_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_AcquisitionParameterSetFactory_h_



#include <gkg-core-pattern/Singleton.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


class AcquisitionParameterSetFactory :
                              public Singleton< AcquisitionParameterSetFactory >
{

  public:


    typedef AcquisitionParameterSet*
              ( *AcquisitionParameterSetCreator )(
              const std::vector< int32_t >& measurementCounts,
              const std::vector< std::string >& stringParameters );

    ~AcquisitionParameterSetFactory();

    bool registerCreator(
                const std::string& name,
                AcquisitionParameterSetCreator acquisitionParameterSetCreator );


    AcquisitionParameterSet*
      create(
        const std::string& name,
        const std::vector< int32_t >& measurementCounts,
        const std::vector< std::string >& stringParameters ) const;

    bool hasAcquisitionParameterSet( const std::string& name ) const;
    std::list< std::string > getNames() const;


  protected:

    friend class Singleton< AcquisitionParameterSetFactory >;

    AcquisitionParameterSetFactory();

    std::map< std::string, 
              AcquisitionParameterSetCreator > _acquisitionParameterSetCreators;



};


}


#define RegisterAcquisitionParameterSet( NameSpace, Implementation )         \
static bool init_##NameSpace##_##Implementation                              \
    __attribute__((unused)) =                                                \
    gkg::AcquisitionParameterSetFactory::getInstance().                      \
      registerCreator( NameSpace::Implementation::getStaticName(),           \
                       &NameSpace::Implementation::createInstance )


#endif
