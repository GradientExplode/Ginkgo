#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMCMCFunctionFactory_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMCMCFunctionFactory_h_



#include <gkg-core-pattern/Singleton.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <gkg-core-pattern/RCPointer.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


class SingleCompartmentRelaxometryMCMCFunctionFactory :
             public Singleton< SingleCompartmentRelaxometryMCMCFunctionFactory >
{

  public:


    typedef SingleCompartmentRelaxometryMCMCFunction*
          ( *RelaxometryMCMCFunctionCreator )(
          const std::vector< int32_t >& measurementCounts,
          const std::vector< double >& noiseStandardDeviations,
          const RCPointer< AcquisitionParameterSet >& acquisitionParameterSet );

    ~SingleCompartmentRelaxometryMCMCFunctionFactory();

    bool registerCreator(
                const std::string& name,
                RelaxometryMCMCFunctionCreator relaxometryMCMCFunctionCreator );


    SingleCompartmentRelaxometryMCMCFunction*
      create(
        const std::string& name,
        const std::vector< int32_t >& measurementCounts,
        const std::vector< double >& noiseStandardDeviations,
        const RCPointer< AcquisitionParameterSet >& acquisitionParameterSet ) const;

    bool hasRelaxometryMCMCFunction( const std::string& name ) const;
    std::list< std::string > getNames() const;


  protected:

    friend class Singleton< SingleCompartmentRelaxometryMCMCFunctionFactory >;

    SingleCompartmentRelaxometryMCMCFunctionFactory();

    std::map< std::string, 
              RelaxometryMCMCFunctionCreator > _relaxometryMCMCFunctionCreators;



};


}


#define RegisterSingleCompartmentRelaxometryMCMCFunction( NameSpace,       \
                                                          Implementation ) \
static bool init_##NameSpace##_##Implementation                            \
    __attribute__((unused)) =                                              \
    gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::getInstance().   \
      registerCreator( NameSpace::Implementation::getStaticName(),         \
                       &NameSpace::Implementation::createInstance )


#endif
