#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_CylinderStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_CylinderStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>


namespace gkg
{


class VoiCylinderStrategy : public VoiModeStrategy
{

  public:

    VoiCylinderStrategy();

    VoiModeStrategy::ModeId id();

    void pressed( VoiDrawA&, VoiData&, Vector3d< int32_t >&, int32_t,
                  QPoint&, Qt::KeyboardModifiers modifiers = Qt::NoModifier );

};


}


#endif
