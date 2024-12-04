#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_StrategyFactory_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_StrategyFactory_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-core-pattern/Singleton.h>

#include <map>


namespace gkg
{


class StrategyFactory : public Singleton< StrategyFactory >
{

  public:

    bool add( VoiStrategy* strategy );

    VoiStrategy* getStrategy( VoiStrategy::StgyType type );

  protected:

    StrategyFactory();
    virtual ~StrategyFactory();

    friend class Singleton< StrategyFactory >;

  private:

    std::map< VoiStrategy::StgyType, VoiStrategy* > _strategies;

};


}


#define RegisterStrategy( IMPLEMENTATION )                                     \
static bool init_##IMPLEMENTATION __attribute__((unused)) =                    \
  gkg::StrategyFactory::getInstance().add( new gkg::IMPLEMENTATION() )


#endif
