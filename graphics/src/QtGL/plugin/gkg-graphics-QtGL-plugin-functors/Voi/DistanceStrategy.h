#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_DistanceStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_DistanceStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>


namespace gkg
{


class VoiDistanceStrategy : public VoiModeStrategy
{

  public:

    VoiDistanceStrategy();

    VoiModeStrategy::ModeId id();

};


}


#endif
