#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_SplineStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_SplineStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>


namespace gkg
{


class VoiSplineStrategy : public VoiModeStrategy
{

  public:

    VoiSplineStrategy();

    VoiModeStrategy::ModeId id();

};


}


#endif
