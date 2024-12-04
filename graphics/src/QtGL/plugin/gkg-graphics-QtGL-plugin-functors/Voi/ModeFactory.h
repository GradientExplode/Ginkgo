#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ModeFactory_h_
#define _gkg_graphics_QtGL_plufin_functors_Voi_ModeFactory_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>


namespace gkg
{


class VoiModeFactory
{

  public:

    VoiModeFactory();
    virtual ~VoiModeFactory();

    VoiModeStrategy *create( VoiModeStrategy::ModeId );

};


}


#endif
