#ifndef _gkg_processing_machinelearning_MachineLearningSelector_h_
#define _gkg_processing_machinelearning_MachineLearningSelector_h_


#include <gkg-processing-machinelearning/MachineLearningImplementationFactory.h>
#include <gkg-core-pattern/Singleton.h>

#include <string>
#include <map>


namespace gkg
{


class MachineLearningSelector : public Singleton< MachineLearningSelector >
{

  public:

    typedef MachineLearningImplementationFactory& ( *Accessor )();

    virtual ~MachineLearningSelector();

    void select( const std::string& name );
    MachineLearningImplementationFactory* getImplementationFactory();

    void registerFactory( const std::string& name, Accessor accessor );

  protected:

    friend class Singleton< MachineLearningSelector >;

    MachineLearningSelector();

    void selectDefaultFactory();

    std::map< std::string, Accessor > _accessors;
    MachineLearningImplementationFactory* _implementationFactory;
    bool _ready;

};


}


#endif
