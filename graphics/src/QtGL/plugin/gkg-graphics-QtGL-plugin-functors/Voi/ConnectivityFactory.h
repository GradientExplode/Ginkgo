#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ConnectivityFactory_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ConnectivityFactory_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelStrategy.h>
#include <gkg-processing-morphology/Neighborhood3d.h>


namespace gkg
{


class VoiConnectivityFactory
{

  public:

    virtual ~VoiConnectivityFactory();

    std::list< Vector3d< int32_t > > create( VoiEvolutionInfo::EvolConnex, 
                                             VoiLabelStrategy::TypeId );

};


}


#endif
