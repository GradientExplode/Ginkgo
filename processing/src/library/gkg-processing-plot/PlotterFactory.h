#ifndef _gkg_processing_plot_PlotterFactory_h_
#define _gkg_processing_plot_PlotterFactory_h_


#include <string>
#include <map>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-plot/CurvePlotterImplementation.h>


namespace gkg
{


class PlotterFactory : public Singleton< PlotterFactory >
{

  public:

    typedef CurvePlotterImplementation* ( *PlotterCreator )();

    ~PlotterFactory();

    bool registerFactory( const std::string& name,
                          PlotterCreator plotterCreator,
                          bool isDefault = false );

    void select( const std::string& name );
    void selectDefault();
    CurvePlotterImplementation* createCurvePlotterImplementation() const;

  private:

    friend class Singleton< PlotterFactory >;

    PlotterFactory();

    std::map< std::string, PlotterCreator > _plotterCreators;
    PlotterCreator _currentPlotterCreator;
    PlotterCreator _defaultPlotterCreator;

};


}


#define RegisterPlotter( NameSpace, Implementation )                       \
static bool init_##NameSpace##_Implementation __attribute__((unused)) =    \
                  gkg::PlotterFactory::getInstance().registerFactory(      \
                       NameSpace::Implementation::getStaticName(),         \
                       &NameSpace::Implementation::createInstance,         \
                       false )

#define RegisterDefaultPlotter( NameSpace, Implementation )                \
static bool init_##NameSpace##_Implementation __attribute__((unused)) =    \
                  gkg::PlotterFactory::getInstance().registerFactory(      \
                       NameSpace::Implementation::getStaticName(),         \
                       &NameSpace::Implementation::createInstance,         \
                       true )

#endif
