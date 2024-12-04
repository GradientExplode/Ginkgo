#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_MorphologyStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_MorphologyStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Indicator.h>


namespace gkg
{


class VoiMorphologyStrategy : public VoiModeStrategy
{

  public:

    VoiMorphologyStrategy();

    VoiModeStrategy::ModeId id();
    void pressed( VoiDrawA&, VoiData&, 
                  Vector3d< int32_t >&, int32_t, QPoint&,
                  Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    void setIndicator( QPainter* painter, QPoint& pos );

  private:

    VoiIndicator indicator;

};


}


#endif
