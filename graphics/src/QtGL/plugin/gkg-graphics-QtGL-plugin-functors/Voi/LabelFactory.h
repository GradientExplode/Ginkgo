#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LabelFactory_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LabelFactory_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>


namespace gkg
{


class VoiLabelFactory
{

  public:

    VoiLabelFactory();
    virtual ~VoiLabelFactory();

    VoiLabelStrategy *create( VoiStrategy::StgyType );

};


}


#endif
