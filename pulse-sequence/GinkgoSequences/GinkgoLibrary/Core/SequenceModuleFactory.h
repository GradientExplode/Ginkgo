#ifndef _gkg_pulse_sequence_GinkgoLibrary_SequenceModuleFactory_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SequenceModuleFactory_h_

#include <Core/Singleton.h>
#include <Core/SequenceModule.h>
#include <Core/RCPointer.h>
#include <map>
#include <set>


namespace gkg
{


class SequenceModuleFactory : public Singleton< SequenceModuleFactory >
{

  public:

    typedef SequenceModule* ( *Creator )(
                  const std::vector< double >&,
                  const std::vector< std::string >&,
                  const std::map< std::string, RCPointer< SequenceModule > >& );

    ~SequenceModuleFactory();

    bool 
      registerSequenceModuleCreator( const std::string& sequenceModuleTypeName,
                                     Creator creator );

    RCPointer< SequenceModule >
      create( const std::string& sequenceModuleTypeName,
              const std::vector< double >& doubleParameters,
              const std::vector< std::string >& stringParameters,
              const std::map< std::string, RCPointer< SequenceModule > >&
                                                   otherSequenceModules ) const;

    std::set< std::string > getSequenceModuleTypeNames() const;

  protected:

    friend class Singleton< SequenceModuleFactory >;

    SequenceModuleFactory();

    std::map< std::string, Creator > _creators;


};


}



#define RegisterSequenceModuleCreator( NameSpace, ConcreteSequenceModule )     \
static bool                                                                    \
  registeredSequenceModuleCreator_##NameSpace##_##ConcreteSequenceModule       \
  /*__attribute__((unused))*/ =                                                \
  gkg::SequenceModuleFactory::getInstance().registerSequenceModuleCreator(     \
               gkg::ConcreteSequenceModule::getStaticName(),                   \
               gkg::ConcreteSequenceModule::createInstance )


#endif

