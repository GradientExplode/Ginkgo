#ifndef _gkg_simulation_mri_sequence_MRISequenceFactory_h_
#define _gkg_simulation_mri_sequence_MRISequenceFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-mri-sequence/MRISequence.h>
#include <gkg-core-object/Dictionary.h>
#include <map>
#include <set>
#include <fstream>


namespace gkg
{


class MRISequenceFactory : public Singleton< MRISequenceFactory >
{

  public:

    typedef MRISequence* ( *Creator )( const Dictionary&,
                                       float,
                                       bool,
                                       std::ofstream* );

    ~MRISequenceFactory();

    bool registerMRISequenceCreator( const std::string& name,
                                     Creator creator );

    MRISequence* create( const Dictionary& dictionary,
                         float timeStepInUs,
                         bool verbose,
                         std::ofstream* osLog = 0 ) const;

    std::set< std::string > getMRISequenceTypeNames() const; 

  protected:

    friend class Singleton< MRISequenceFactory >;

    MRISequenceFactory();

    std::map< std::string, Creator > _creators;


};


}


#define RegisterMRISequenceCreator( NameSpace, ConcreteMRISequence )           \
static bool                                                                    \
registeredMRISequenceCreator_##NameSpace##_##ConcreteMRISequence               \
  __attribute__((unused)) =                                                    \
  gkg::MRISequenceFactory::getInstance().registerMRISequenceCreator(           \
               gkg::ConcreteMRISequence::getStaticName(),                      \
               gkg::ConcreteMRISequence::createInstance )


#endif
