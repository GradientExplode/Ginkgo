#ifndef _gkg_processing_numericalanalysis_NumericalAnalysisSelector_h_
#define _gkg_processing_numericalanalysis_NumericalAnalysisSelector_h_


#include <string>
#include <map>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>


namespace gkg
{


class NumericalAnalysisSelector : public Singleton< NumericalAnalysisSelector >
{

  public:

    typedef NumericalAnalysisImplementationFactory& ( *Accessor )();

    ~NumericalAnalysisSelector();

    void select( const std::string& name );
    NumericalAnalysisImplementationFactory* getImplementationFactory();

    void registerFactory( const std::string& name, Accessor accessor );

  protected:

    friend class Singleton< NumericalAnalysisSelector >;

    NumericalAnalysisSelector();

    void selectDefaultFactory();

    std::map< std::string, Accessor > _accessors;
    NumericalAnalysisImplementationFactory* _implementationFactory;
    bool _ready;

};





}


#endif
